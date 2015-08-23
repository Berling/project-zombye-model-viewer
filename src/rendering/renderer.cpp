#include <GL/glew.h>
#include <glcw/shader.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <utils/logger.hpp>

#include <core/engine.hpp>
#include <graphics/graphics_system.hpp>
#include <graphics/shader_manager.hpp>
#include <graphics/texture_manager.hpp>
#include <rendering/point_light.hpp>
#include <rendering/renderer.hpp>
#include <rendering/screen_quad.hpp>
#include <rendering/vertex.hpp>

namespace rendering {
	renderer::renderer(core::engine& engine, const std::string& file_name)
	: engine_{engine}, mesh_manager_{engine_, *this}, debug_render_{false}, camera_position_{-10.f, 20.f, 0.f} {
		staticmesh_layout_.emplace_back("_position", 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
		staticmesh_layout_.emplace_back("_texcoord", 2, GL_FLOAT, GL_FALSE, sizeof(vertex), 3 * sizeof(float));
		staticmesh_layout_.emplace_back("_normal", 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 5 * sizeof(float));

		auto width = static_cast<float>(engine.graphics_system().width());
		auto height = static_cast<float>(engine.graphics_system().height());
		projection_ = glm::perspectiveFov(
			glm::radians(45.f),
			width,
			height,
			0.1f, 1000.f);

		viewport_ = glm::vec4{0.f, 0.f, width, height};

		ortho_projection_ = glm::ortho(0.f, width, height, 0.f);

		g_buffer_.attach(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, GL_DEPTH_COMPONENT24, width, height, GL_DEPTH_COMPONENT, GL_FLOAT);
		g_buffer_.attach(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GL_RGBA32F, width, height, GL_RGBA, GL_FLOAT);
		g_buffer_.attach(GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, GL_RGBA32F, width, height, GL_RGBA, GL_FLOAT);
		g_buffer_.attach(GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, GL_RGBA32F, width, height, GL_RGBA, GL_FLOAT);

		g_buffer_.bind();
		glClearColor(0.4f, 0.6f, 0.9f, 1.f);
		GLenum buf[5] = {
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2,
			GL_NONE
		};
		glDrawBuffers(5, buf);
		g_buffer_.bind_default();

		auto vertex_shader = engine_.graphics_system().shader_manager().load("shaders/screen_quad.vert", GL_VERTEX_SHADER);
		if (!vertex_shader) {
			throw std::runtime_error{"could not load screen_quad.vert"};
		}
		screen_quad_program_.attach_shader(vertex_shader);
		auto fragment_shader = engine_.graphics_system().shader_manager().load("shaders/screen_quad.frag", GL_FRAGMENT_SHADER);
		if (!fragment_shader) {
			throw std::runtime_error{"could not load screen_quad.frag"};
		}
		screen_quad_program_.attach_shader(fragment_shader);
		staticmesh_layout_.setup_program(screen_quad_program_, "frag_color");
		screen_quad_program_.link();

		for (auto i = 0; i < 4; ++i) {
			debug_screen_quads_.emplace_back(std::make_unique<screen_quad>(
				*this,
				glm::vec2{(i * width / 4.f) + 0.01f * width, (3 * height / 4) + 0.01f * height},
				glm::vec2{((i + 1) * width / 4.f) - 0.01f * width, height - 0.01f * height}
			));
		}

		vertex_shader = engine_.graphics_system().shader_manager().load("shaders/world.vert", GL_VERTEX_SHADER);
		if (!vertex_shader) {
			throw std::runtime_error{"could not load world.vert"};
		}
		world_program_.attach_shader(vertex_shader);
		fragment_shader = engine_.graphics_system().shader_manager().load("shaders/world.frag", GL_FRAGMENT_SHADER);
		if (!fragment_shader) {
			throw std::runtime_error{"could not load world.frag"};
		}
		world_program_.attach_shader(fragment_shader);
		staticmesh_layout_.setup_program(world_program_, "albedo_color");
		world_program_.bind_frag_data_location("normal_color", 1);
		world_program_.bind_frag_data_location("specular_color", 2);
		world_program_.bind_frag_data_location("position_color", 3);
		world_program_.link();

		suzanne_ = mesh_manager_.load(file_name);
		if (!suzanne_) {
			throw std::runtime_error{"could not load " + file_name};
		}

		vertex_shader = engine_.graphics_system().shader_manager().load("shaders/deferred.vert", GL_VERTEX_SHADER);
		if (!vertex_shader) {
			throw std::runtime_error{"could not load deferred.vert"};
		}
		composition_program_.attach_shader(vertex_shader);
		fragment_shader = engine_.graphics_system().shader_manager().load("shaders/deferred.frag", GL_FRAGMENT_SHADER);
		if (!fragment_shader) {
			throw std::runtime_error{"could not load deferred.frag"};
		}
		composition_program_.attach_shader(fragment_shader);
		staticmesh_layout_.setup_program(composition_program_, "frag_color");
		composition_program_.link();

		screen_quad_ = std::make_unique<screen_quad>(*this, glm::vec2(0.f), glm::vec2(width, height));

		point_light l = {
			glm::vec3{-2.f, 5.f, 5.f},
			glm::vec3{1.f},
			20.f
		};
		point_lights_.emplace_back(l);

		l = {
			glm::vec3{5.f, 5.f, -10.f},
			glm::vec3{1.f},
			30.f
		};
		point_lights_.emplace_back(l);

		view_ = glm::lookAt(
			glm::vec3{0.f},
			glm::vec3{0.f},
			glm::vec3{0.f, 1.f, 0.f}
		);
	}

	renderer::~renderer() {}

	void renderer::render(float delta_time) {
		glEnable(GL_DEPTH_TEST);
		g_buffer_.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		world_program_.use();
		world_program_.uniform("projection", false, projection_);
		world_program_.uniform("view", false, view_);
		auto model = glm::mat4{1.f};
		world_program_.uniform("model", false, model);
		world_program_.uniform("normal", false, glm::mat3{glm::inverseTranspose(model)});
		world_program_.uniform("color_texture", 0);
		world_program_.uniform("normal_texture", 1);
		world_program_.uniform("specular_texture", 2);
		suzanne_->draw();

		glDisable(GL_DEPTH_TEST);
		g_buffer_.bind_default();

		static GLenum attachments[4] = {
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2,
			GL_DEPTH_ATTACHMENT
		};

		std::vector<glm::vec3> point_light_positions;
		std::vector<glm::vec3> point_light_colors;
		std::vector<float> point_light_radii;
		for (auto& p : point_lights_) {
			point_light_positions.emplace_back(p.position);
			point_light_colors.emplace_back(p.color);
			point_light_radii.emplace_back(p.radius);
		}

		composition_program_.use();
		composition_program_.uniform("projection", false, ortho_projection_);
		composition_program_.uniform("albedo_texture", 0);
		composition_program_.uniform("normal_texture", 1);
		composition_program_.uniform("specular_texture", 2);
		composition_program_.uniform("depth_texture", 3);
		composition_program_.uniform("inv_view_projection", false, glm::inverse(projection_ * view_));
		composition_program_.uniform("view_vector", camera_position_);
		composition_program_.uniform("point_light_num", static_cast<int32_t>(point_lights_.size()));
		composition_program_.uniform("point_light_positions", point_lights_.size(), point_light_positions);
		composition_program_.uniform("point_light_colors", point_lights_.size(), point_light_colors);
		composition_program_.uniform("point_light_radii", point_lights_.size(), point_light_radii);

		for (auto i = 0; i < 4; ++i) {
			g_buffer_.attachment(attachments[i]).bind(i);
		}
		screen_quad_->draw();

		if (debug_render_) {
			screen_quad_program_.use();
			screen_quad_program_.uniform("projection", false, ortho_projection_);
			screen_quad_program_.uniform("near_plane", 0.1f);
			screen_quad_program_.uniform("far_plane", 1000.f);
			screen_quad_program_.uniform("color_texture", 0);

			for (auto i = 0; i < debug_screen_quads_.size(); ++i) {
				g_buffer_.attachment(attachments[i]).bind(0);

				screen_quad_program_.uniform("linearize", false);
				if (attachments[i] == GL_DEPTH_ATTACHMENT) {
					screen_quad_program_.uniform("linearize", true);
				}

				debug_screen_quads_[i]->draw();
			}
		}
		glEnable(GL_DEPTH_TEST);
	}
}

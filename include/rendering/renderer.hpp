#ifndef __RENDERING_RENDERER_HPP__
#define __RENDERING_RENDERER_HPP__

#include <memory>

#include <glcw/framebuffer.hpp>
#include <glcw/program.hpp>
#include <glcw/texture.hpp>
#include <glcw/vertex_layout.hpp>
#include <glm/glm.hpp>

#include <rendering/mesh_manager.hpp>
#include <rendering/skeleton_manager.hpp>
#include <rendering/skinned_mesh_manager.hpp>

namespace core {
	class engine;
}

namespace rendering {
	struct point_light;
	class screen_quad;
	class animated_mesh;
}

namespace rendering {
	class renderer {
	private:
		core::engine& engine_;
		glcw::vertex_layout staticmesh_layout_;
		glcw::vertex_layout skinnedmesh_layout_;
		glm::mat4 projection_;
		glm::mat4 view_;
		glm::vec3 camera_position_;
		glm::vec4 viewport_;
		glm::mat4 ortho_projection_;
		glcw::framebuffer g_buffer_;
		glcw::program screen_quad_program_;
		std::vector<std::unique_ptr<screen_quad>> debug_screen_quads_;
		mesh_manager mesh_manager_;
		skeleton_manager skeleton_manager_;
		skinned_mesh_manager skinned_mesh_manager_;
		glcw::program world_program_;
		glcw::program animation_program_;
		mesh_ptr suzanne_;
		std::unique_ptr<animated_mesh> animation_;
		std::shared_ptr<const glcw::texture> dummy_texture_;
		std::shared_ptr<const glcw::texture> dummy_specular_;
		std::shared_ptr<const glcw::texture> dummy_normal_;
		glcw::program composition_program_;
		std::unique_ptr<screen_quad> screen_quad_;
		bool debug_render_;
		std::vector<point_light> point_lights_;

	public:
		renderer(core::engine& engine, const std::string& mesh, const std::string& skeleton);
		~renderer();

		renderer(const renderer& rhs) = delete;
		renderer& operator=(const renderer& rhs) = delete;

		renderer(renderer&& rhs) = delete;
		renderer& operator=(renderer&& rhs) = delete;

		void render(float delta_time);

		auto& staticmesh_layout() {
			return staticmesh_layout_;
		}

		auto& skinnedmesh_layout() {
			return skinnedmesh_layout_;
		}

		auto& get_mesh_manager() {
			return mesh_manager_;
		}

		auto& skeleton_manager() {
			return skeleton_manager_;
		}

		auto& skinned_mesh_manager() {
			return skinned_mesh_manager_;
		}

		auto& camera_position() {
			return camera_position_;
		}

		auto& view_matrix() const noexcept {
			return view_;
		}

		void view_matrix(const glm::mat4& view_matrix) {
			view_ = view_matrix;
		}

		void debug_mode(bool value) {
			debug_render_ = value;
		}
	};
}

#endif

#include <stdexcept>
#include <string>

#include <GL/glew.h>
#include <utils/logger.hpp>

#include <core/engine.hpp>
#include <graphics/graphics_system.hpp>
#include <graphics/shader_manager.hpp>
#include <graphics/texture_manager.hpp>
#include <utils/logger.hpp>

namespace graphics {
	graphics_system::graphics_system(core::engine& engine)
	: engine_{engine}, window_{nullptr, [](SDL_Window*){}}, context_{nullptr}, width_{800}, height_{600},
	fullscreen_{false}, clear_color_{glm::vec4{0.4f, 0.6f, 0.9f, 1.f}},
	shader_manager_{std::make_unique<graphics::shader_manager>(engine_)},
	texture_manager_{std::make_unique<graphics::texture_manager>(engine_)} {
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,8);

		auto window_mask = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | (fullscreen_ ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
		name_ = "ZMDL Viewer";
		window_ = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>(
			SDL_CreateWindow(name_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width_, height_, window_mask),
			SDL_DestroyWindow);

		if (!window_) {
			throw std::runtime_error{"could not create window"};
		}

		auto width = 0;
		auto height = 0;
		SDL_GetWindowSize(window_.get(), &width, &height);
		width_ = width;
		height_ = height;

		try {
			context_ = SDL_GL_CreateContext(window_.get());
		} catch (const std::runtime_error& ex) {
			throw std::runtime_error{std::string{"could not create opengl context with version 4.3. "} + ex.what()};
		}

		SDL_GL_SetSwapInterval(1);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			throw std::runtime_error("requested opengl 4.3 features are not available");
		}

		utils::log << "OpenGL version " << std::string{reinterpret_cast<const char*>(glGetString(GL_VERSION))} << std::endl;

		if(GLEW_KHR_debug){
			glDebugMessageCallback(
				+[](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void*) {
					std::cerr << std::string(message,length) << " (source: " << source << ", type: " <<type<< ", id: "
						<< id <<", severity: " << severity << ")" <<std::endl;
				}, nullptr);
		}
		else{
			utils::log(utils::LOG_WARNING) << "no opengL debug log available." << std::endl;
		}
		clear_color(clear_color_);
	}

	graphics_system::~graphics_system() {
		SDL_GL_DeleteContext(context_);
	}

	void graphics_system::begin() noexcept {
		glClearColor(clear_color_.r, clear_color_.g, clear_color_.b, clear_color_.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void graphics_system::end(float delta_time) noexcept {
		const auto smooth_factor = 0.1f;
		static auto delta_time_smoothed = 0.f;
		static auto time_since_last_fps_output = 0.f;

		delta_time_smoothed = ( 1.0f - smooth_factor) * delta_time_smoothed + smooth_factor * delta_time;

		time_since_last_fps_output += delta_time;
		if(time_since_last_fps_output >= 1.0f){
			time_since_last_fps_output = 0.0f;
			std::ostringstream osstr;
			osstr << name_ << " (" << (int((1.0f / delta_time_smoothed) * 10.0f) / 10.0f) << " FPS, ";
			osstr << (int(delta_time_smoothed * 10000.0f) / 10.0f) << " ms / frame)";
			SDL_SetWindowTitle(window_.get(), osstr.str().c_str());
		}
		SDL_GL_SwapWindow(window_.get());
	}

	void graphics_system::clear_color(const glm::vec4& color) {
		clear_color_ = color;
		glClearColor(clear_color_.r, clear_color_.g, clear_color_.b, clear_color_.a);
	}
}

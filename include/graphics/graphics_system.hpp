#ifndef __GRAPHICS_GRAPHICS_SYSTEM_HPP__
#define __GRAPHICS_GRAPHICS_SYSTEM_HPP__

#include <memory>

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

namespace core {
	class engine;
}

namespace graphics {
	class graphics_system {
	private:
		core::engine& engine_;
		std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window_;
		SDL_GLContext context_;
		std::string name_;
		size_t width_;
		size_t height_;
		bool fullscreen_;
		glm::vec4 clear_color_;

	public:
		graphics_system(core::engine& engine);
		~graphics_system();

		graphics_system(const graphics_system& rhs) = delete;
		graphics_system& operator=(const graphics_system& rhs) = delete;

		graphics_system(graphics_system&& rhs) = delete;
		graphics_system& operator=(graphics_system&& rhs) = delete;

		void begin() noexcept;
		void end(float delta_time) noexcept;
		void clear_color(const glm::vec4& color);

		size_t width() const noexcept {
			return width_;
		}

		size_t height() const noexcept {
			return height_;
		}
	};
}

#endif

#ifndef __RENDERING_SCREEN_QUAD_HPP__
#define __RENDERING_SCREEN_QUAD_HPP__

#include <glcw/vertex_array.hpp>
#include <glcw/buffer.hpp>

namespace rendering {
	class renderer;
}

namespace rendering {
	class screen_quad {
	private:
		glcw::vertex_array vao_;
		glcw::vertex_buffer vbo_;

	public:
		screen_quad(rendering::renderer& renderer, const glm::vec2& top_left, const glm::vec2& bottom_right);
		~screen_quad() = default;

		screen_quad(const screen_quad& rhs) = delete;
		screen_quad& operator=(const screen_quad& rhs) = delete;

		screen_quad(screen_quad&& rhs) = delete;
		screen_quad& operator=(screen_quad&& rhs) = delete;

		void draw() const;
	};
}

#endif
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <utils/logger.hpp>

#include <core/engine.hpp>
#include <rendering/renderer.hpp>
#include <rendering/screen_quad.hpp>
#include <rendering/vertex.hpp>

namespace rendering {
	screen_quad::screen_quad(rendering::renderer& renderer, const glm::vec2& top_left, const glm::vec2& bottom_right)
	: vbo_{GL_STATIC_DRAW} {
		glm::vec3 zero;
		vertex vertices[6] = {
			{glm::vec3{top_left, 0.f},						glm::vec2{0.f, 1.f}, zero, zero},
			{glm::vec3{top_left.x, bottom_right.y, 0.f},	glm::vec2{0.f, 0.f}, zero, zero},
			{glm::vec3{bottom_right.x, top_left.y, 0.f}, 	glm::vec2{1.f, 1.f}, zero, zero},
			{glm::vec3{top_left.x, bottom_right.y, 0.f}, 	glm::vec2{0.f, 0.f}, zero, zero},
			{glm::vec3{bottom_right, 0.f},					glm::vec2{1.f, 0.f}, zero, zero},
			{glm::vec3{bottom_right.x, top_left.y, 0.f}, 	glm::vec2{1.f, 1.f}, zero, zero}
		};
		vbo_.data(6 * sizeof(vertex), vertices);
		renderer.staticmesh_layout().setup_layout(vao_, &vbo_);
	}

	void screen_quad::draw() const {
		vao_.bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}

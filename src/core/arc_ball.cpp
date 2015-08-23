#include <core/arc_ball.hpp>

namespace core {
	arc_ball::arc_ball() noexcept
	: screen_radius_{1.f}, center_{0.f}, position_now_{0.f}, position_down_{0.f},
	rotation_now_{glm::angleAxis(0.f, glm::vec3{0.f})}, rotation_down_{glm::angleAxis(0.f, glm::vec3{0.f})},
	rotation_drag_{glm::angleAxis(0.f, glm::vec3{0.f})}, dragging_{false} {}

	glm::mat4 arc_ball::rotation_matrix() noexcept {
		auto from = convert_mouse_to_sphere(position_down_);
		auto to = convert_mouse_to_sphere(position_now_);

		if (dragging_) {
			rotation_drag_ = create_quaternion(from, to);
			rotation_now_ = rotation_drag_ * rotation_down_;
		}

		return glm::transpose(glm::toMat4(rotation_now_));
	}

	glm::vec3 arc_ball::convert_mouse_to_sphere(glm::vec2 mouse_position) const noexcept {
		auto mouse_unit = (mouse_position - center_) / screen_radius_;
		auto drag_radius = glm::dot(mouse_unit, mouse_unit);

		auto mouse_ball = glm::vec3{mouse_unit, 0.f};

		if (drag_radius > 1.f) {
			mouse_ball /= sqrtf(drag_radius);
			mouse_ball.z = 0.f;
		} else {
			mouse_ball.z = sqrtf(1.f - drag_radius);
		}

		return mouse_ball;
	}

	glm::quat arc_ball::create_quaternion(glm::vec3 from, glm::vec3 to) const noexcept {
		return glm::quat{glm::dot(from, to), glm::cross(from, to)};
	}
}

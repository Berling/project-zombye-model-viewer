#ifndef __CORE_ARC_BALL_HPP__
#define __CORE_ARC_BALL_HPP__

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace core {
	class arc_ball {
	private:
		float screen_radius_;
		glm::vec2 center_;
		glm::vec2 position_now_;
		glm::vec2 position_down_;
		glm::quat rotation_now_;
		glm::quat rotation_down_;
		glm::quat rotation_drag_;
		bool dragging_;

		glm::vec3 convert_mouse_to_sphere(glm::vec2 mouse_position) const noexcept;
		glm::quat create_quaternion(glm::vec3 from, glm::vec3 to) const noexcept;

	public:
		arc_ball() noexcept;
		~arc_ball() = default;

		arc_ball(const arc_ball& rhs) = default;
		arc_ball& operator=(const arc_ball& rhs) = default;

		arc_ball(arc_ball&& rhs) = default;
		arc_ball& operator=(arc_ball&& rhs) = default;

		void place(const glm::vec2& center, float radius) noexcept {
			center_ = center;
			screen_radius_ = radius;
		}

		void reset() noexcept {
			dragging_ = false;
			rotation_down_ = glm::angleAxis(0.f, glm::vec3{0.f});
			rotation_now_ = glm::angleAxis(0.f, glm::vec3{0.f});
			position_now_ = center_;
			position_down_ = center_;
		}

		void start_dragging() noexcept {
			dragging_ = true;
			position_down_ = position_now_;
		}

		void finish_dragging() noexcept {
			dragging_ = false;
			rotation_down_ = rotation_now_;
		}

		void mouse_position(const glm::vec2& mouse_position) noexcept {
			position_now_ = mouse_position;
		}

		glm::mat4 rotation_matrix() noexcept;
	};
}

#endif

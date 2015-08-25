#ifndef __CORE_ENGINE_HPP__
#define __CORE_ENGINE_HPP__

#include <memory>
#include <string>

#include <asset/asset_manager.hpp>
#include <core/arc_ball.hpp>

namespace graphics {
	class graphics_system;
}

namespace rendering {
	class renderer;
}

namespace core {
	class engine {
	private:
		bool quit_;
		asset::asset_manager asset_manager_;
		std::unique_ptr<graphics::graphics_system> graphics_system_;
		std::unique_ptr<rendering::renderer> renderer_;
		float center_;
		float scene_radius_;
		float zoom_;
		arc_ball camera_;
		glm::vec2 mouse_down_;
		bool dragging_;
		bool zooming_;
		bool rotating_;
		glm::vec3 camera_position_;
		glm::vec2 translate_;

	public:
		engine(const std::string& mesh, const std::string& skeleton) noexcept;
		~engine();

		engine(const engine& rhs) = delete;
		engine& operator=(const engine& rhs) = delete;

		engine(engine&& rhs) = delete;
		engine& operator=(engine&& rhs) = delete;

		void update(float delta_time);
		void run();
		void quit() noexcept;

		auto& asset_manager() {
			return asset_manager_;
		}

		auto& graphics_system() {
			return *graphics_system_;
		}

		auto& renderer() {
			return *renderer_;
		}
	};
}

#endif

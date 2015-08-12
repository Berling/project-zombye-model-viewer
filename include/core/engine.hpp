#ifndef __CORE_ENGINE_HPP__
#define __CORE_ENGINE_HPP__

#include <memory>
#include <string>

#include <asset/asset_manager.hpp>

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

	public:
		engine(const std::string& file_name) noexcept;
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

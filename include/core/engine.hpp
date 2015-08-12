#ifndef __CORE_ENGINE_HPP__
#define __CORE_ENGINE_HPP__

#include <memory>
#include <string>

namespace graphics {
	class graphics_system;
}

namespace core {
	class engine {
	private:
		bool quit_;
		std::unique_ptr<graphics::graphics_system> graphics_system_;

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

		auto& graphics_system() const noexcept {
			return *graphics_system_;
		}
	};
}

#endif

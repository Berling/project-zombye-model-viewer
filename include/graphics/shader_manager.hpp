#ifndef __GRAPHICS_SHADER_MANAGER_HPP__
#define __GRAPHICS_SHADER_MANAGER_HPP__

#include <memory>
#include <string>

#include <glcw/shader.hpp>

#include <utils/cached_resource_manager.hpp>

namespace core {
	class engine;
}

namespace graphics {
	using shader_ptr = std::shared_ptr<const glcw::shader>;

	class shader_manager : public utils::cached_resource_manager<const glcw::shader, shader_manager> {
	private:
		core::engine& engine_;

		friend class utils::cached_resource_manager<const glcw::shader, shader_manager>;

	public:
		shader_manager(core::engine& engine) noexcept;
		~shader_manager() = default;

	protected:
		shader_ptr load_new(const std::string& name, GLenum type);
	};
}

#endif
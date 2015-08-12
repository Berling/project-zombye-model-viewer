#ifndef __GRAPHICS_TEXTURE_MANAGER_HPP__
#define __GRAPHICS_TEXTURE_MANAGER_HPP__

#include <memory>
#include <string>

#include <glcw/texture.hpp>

#include <utils/cached_resource_manager.hpp>

namespace core {
	class engine;
}

namespace graphics {
	using texture_ptr = std::shared_ptr<const glcw::texture>;

	class texture_manager : public utils::cached_resource_manager<const glcw::texture, texture_manager> {
	private:
		core::engine& engine_;

		friend class utils::cached_resource_manager<const glcw::texture, texture_manager>;

	public:
		texture_manager(core::engine& engine) noexcept;
		~texture_manager() = default;

	protected:
		texture_ptr load_new(const std::string& name);
	};
}

#endif
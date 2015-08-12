#include <gli/gli.hpp>

#include <asset/asset.hpp>
#include <core/engine.hpp>
#include <graphics/texture_manager.hpp>
#include <utils/load_dds_from_memory.hpp>

namespace graphics {
	texture_manager::texture_manager(core::engine& engine) noexcept
	: engine_{engine} {}

	texture_ptr texture_manager::load_new(const std::string& name) {
		auto asset = engine_.asset_manager().load(name);
		if (!asset) {
			return nullptr;
		}
		auto stoarge = gli::load_dds(asset->content());
		if (stoarge.empty()) {
			return nullptr;
		}
		return std::make_shared<const glcw::texture>(gli::texture2D{stoarge});
	}
}
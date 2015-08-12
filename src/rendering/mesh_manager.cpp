#include <asset/asset.hpp>
#include <core/engine.hpp>
#include <rendering/mesh_manager.hpp>
#include <rendering/renderer.hpp>

namespace rendering {
	mesh_manager::mesh_manager(core::engine& engine, rendering::renderer& renderer) noexcept
	: engine_{engine}, renderer_{renderer} {}

	mesh_ptr mesh_manager::load_new(const std::string& name) {
		auto asset = engine_.asset_manager().load(name);
		if (!asset) {
			return nullptr;
		}
		return std::make_unique<const mesh>(engine_, renderer_, asset->content(), name);
	}
}

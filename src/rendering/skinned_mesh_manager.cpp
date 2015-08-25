#include <asset/asset.hpp>
#include <asset/asset_manager.hpp>
#include <core/engine.hpp>
#include <rendering/skinned_mesh.hpp>
#include <rendering/skinned_mesh_manager.hpp>

namespace rendering {
    skinned_mesh_manager::skinned_mesh_manager(core::engine& engine, renderer& renderer) noexcept
    : engine_{engine}, renderer_{renderer} { }

    skinned_mesh_ptr skinned_mesh_manager::load_new(const std::string& name) {
        auto asset = engine_.asset_manager().load(name);
        if (!asset) {
            return nullptr;
        }
        return std::make_shared<const skinned_mesh>(engine_, renderer_, asset->content(), name);
    }
}

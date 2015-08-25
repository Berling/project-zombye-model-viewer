#include <asset/asset.hpp>
#include <asset/asset_manager.hpp>
#include <core/engine.hpp>
#include <rendering/skeleton.hpp>
#include <rendering/skeleton_manager.hpp>

namespace rendering {
    skeleton_manager::skeleton_manager(core::engine& engine) noexcept
    : engine_{engine} { }

    skeleton_ptr skeleton_manager::load_new(const std::string& name) {
        auto asset = engine_.asset_manager().load(name);
        if (!asset) {
            return nullptr;
        }
        return std::make_shared<const skeleton>(engine_.renderer(), asset->content(), name);
    }
}

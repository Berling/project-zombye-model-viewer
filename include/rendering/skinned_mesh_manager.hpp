#ifndef __RENDERING_SKINNED_MESH_MANAGER_HPP__
#define __RENDERING_SKINNED_MESH_MANAGER_HPP__

#include <memory>
#include <string>

#include <utils/cached_resource_manager.hpp>

namespace core {
    class engine;
}

namespace rendering {
    class skinned_mesh;
    class renderer;
}

namespace rendering {
    using skinned_mesh_ptr = std::shared_ptr<const skinned_mesh>;

    class skinned_mesh_manager : public utils::cached_resource_manager<const skinned_mesh, skinned_mesh_manager> {
        friend class utils::cached_resource_manager<const skinned_mesh, skinned_mesh_manager>;

        core::engine& engine_;
        renderer& renderer_;
    public:
        skinned_mesh_manager(core::engine& engine, renderer& renderer) noexcept;
        ~skinned_mesh_manager() noexcept = default;
    protected:
        skinned_mesh_ptr load_new(const std::string& name);
    };
}

#endif

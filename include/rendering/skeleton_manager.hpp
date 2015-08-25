#ifndef __RENDERING_SKELETON_MANAGER_HPP__
#define __RENDERING_SKELETON_MANAGER_HPP__

#include <memory>
#include <string>

#include <utils/cached_resource_manager.hpp>

namespace core {
    class engine;
}

namespace rendering {
    class skeleton;
}

namespace rendering {
    using skeleton_ptr = std::shared_ptr<const skeleton>;

    class skeleton_manager : public utils::cached_resource_manager<const skeleton, skeleton_manager> {
        friend class utils::cached_resource_manager<const skeleton, skeleton_manager>;

        core::engine& engine_;
    public:
        skeleton_manager(core::engine& engine) noexcept;
        ~skeleton_manager() noexcept = default;
    protected:
        skeleton_ptr load_new(const std::string& name);
    };
}

#endif

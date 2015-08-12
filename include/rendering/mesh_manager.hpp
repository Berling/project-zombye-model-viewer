#ifndef __RENDERING_MESH_MANAGER_HPP__
#define __RENDERING_MESH_MANAGER_HPP__

#include <memory>
#include <string>

#include <rendering/mesh.hpp>
#include <utils/cached_resource_manager.hpp>

namespace rendering {
	class renderer;
}

namespace core {
	class engine;
}

namespace rendering {
	using mesh_ptr = std::shared_ptr<const mesh>;

	class mesh_manager : public utils::cached_resource_manager<const mesh, mesh_manager> {
	private:
		core::engine& engine_;
		rendering::renderer& renderer_;

		friend class utils::cached_resource_manager<const mesh, mesh_manager>;

	public:
		mesh_manager(core::engine& engine, rendering::renderer& renderer) noexcept;
		~mesh_manager() = default;

	protected:
		mesh_ptr load_new(const std::string& name);
	};
}

#endif
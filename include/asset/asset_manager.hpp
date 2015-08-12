#ifndef __ASSET_ASSET_MANAGER_HPP__
#define __ASSET_ASSET_MANAGER_HPP__

#include <memory>
#include <string>
#include <vector>

#include <utils/cached_resource_manager.hpp>

namespace asset {
	class asset;
	class asset_loader;
}

namespace asset {
	using asset_ptr = std::shared_ptr<const asset>;

	class asset_manager : public utils::cached_resource_manager<const asset, asset_manager> {
	private:
		std::vector<std::unique_ptr<asset_loader>> loaders_;

		friend class utils::cached_resource_manager<const asset, asset_manager>;

	public:
		asset_manager();
		~asset_manager();

		std::string native_name(const std::string& name) const;

	protected:
		asset_ptr load_new(const std::string& name);
	};
}

#endif
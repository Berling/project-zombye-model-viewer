#ifndef __ASSET_ASSET_LOADER_HPP__
#define __ASSET_ASSET_LOADER_HPP__

#include <memory>
#include <string>

namespace asset {
	class asset;
}

namespace asset {
	class asset_loader {
	public:
		asset_loader() noexcept = default;
		virtual ~asset_loader() = default;

		asset_loader(const asset_loader& rhs) = delete;
		asset_loader& operator=(const asset_loader& rhs) = delete;

		asset_loader(asset_loader&& rhs) = delete;
		asset_loader& operator=(asset_loader&& rhs) = delete;

		virtual std::shared_ptr<const asset> load_asset(const std::string& name) const = 0;

		virtual std::string native_name(const std::string& name) const {
			return std::string{};
		}
	};
}

#endif
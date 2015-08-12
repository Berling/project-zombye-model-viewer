#include <asset/asset.hpp>
#include <asset/asset_loader.hpp>
#include <asset/asset_manager.hpp>
#include <asset/native_asset_loader.hpp>

namespace asset {
	asset_manager::asset_manager() {
		loaders_.emplace_back(std::make_unique<native_asset_loader>(""));
		loaders_.emplace_back(std::make_unique<native_asset_loader>("assets/"));
		loaders_.emplace_back(std::make_unique<native_asset_loader>("../assets/"));
	}

	asset_manager::~asset_manager() {}

	std::string asset_manager::native_name(const std::string& name) const {
		for (auto& loader : loaders_) {
			auto tmp_name = loader->native_name(name);
			if (!tmp_name.empty()) {
				return tmp_name;
			}
		}
		return std::string{};
	}

	asset_ptr asset_manager::load_new(const std::string& name) {
		for (auto& loader : loaders_) {
			auto ptr = loader->load_asset(name);
			if (ptr) {
				return ptr;
			}
		}
		return nullptr;
	}
}
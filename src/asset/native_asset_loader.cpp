#include <fstream>

#include <asset/asset.hpp>
#include <asset/asset_loader.hpp>
#include <asset/native_asset_loader.hpp>

namespace asset {
	native_asset_loader::native_asset_loader(const std::string& prefix) noexcept
	: prefix_{prefix} {}

	native_asset_loader::~native_asset_loader() {}

	std::shared_ptr<const asset> native_asset_loader::load_asset(const std::string& name) const {
		auto full_name = prefix_ + name;
	#ifdef WINDOWS
		for(auto& character : full_name){
			if(character == '/')
				character = '\\';
		}
	#endif
		std::ifstream stream(full_name, std::ios::binary);
		if(stream) {
			stream.seekg(0, std::ios::end);
			auto size = stream.tellg();
			stream.seekg(0, std::ios::beg);
			auto a = std::make_shared<asset>(name, size);
			stream.read(a->content().data(), size);
			stream.close();
			return a;
		}
		return nullptr;
	}

	std::string native_asset_loader::native_name(const std::string& name) const {
		auto full_name = prefix_ + name;
	#ifdef GDW_WINDOWS
		for(auto& character : full_name){
			if(character=='/')
				character='\\';
		}
	#endif
		std::ifstream is(full_name, std::ios::binary);
		if(is) {
			return full_name;
		}
		return std::string{};
	}
}
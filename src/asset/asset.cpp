#include <asset/asset.hpp>

namespace asset {
	asset::asset(const std::string& name, size_t size)
	: name_{name}, content_(size) {}
}
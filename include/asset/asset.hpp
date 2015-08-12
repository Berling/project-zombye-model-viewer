#ifndef __ASSET_ASSET_HPP__
#define __ASSET_ASSET_HPP__

#include <string>
#include <vector>

namespace asset {
	class asset {
	private:
		std::string name_;
		std::vector<char> content_;

	public:
		asset(const std::string& name, size_t size);
		~asset() = default;

		asset(const asset& rhs) = delete;
		asset& operator=(const asset& rhs) = delete;

		asset(asset&& rhs) = delete;
		asset& operator=(asset&& rhs) = delete;

		std::string name() const noexcept {
			return name_;
		}

		const std::vector<char>& content() const noexcept {
			return content_;
		}

		std::vector<char>& content() noexcept {
			return content_;
		}
	};
}

#endif
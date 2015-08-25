#include <stdexcept>

#include <core/engine.hpp>

int main(int argc, char const* argv[]) {
	if (argc == 1) {
		throw std::runtime_error("argument missing from command line");
	} else if (argc > 3) {
		throw std::runtime_error("unrecognized option");
	}

	if (argc == 2) {
		core::engine e{std::string{argv[1]}, ""};
		e.run();
	} else if (argc == 3) {
		core::engine e{std::string{argv[1]}, std::string{argv[2]}};
		e.run();
	}
	return 0;
}

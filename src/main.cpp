#include <stdexcept>

#include <core/engine.hpp>

int main(int argc, char const* argv[]) {
	if (argc == 1) {
		throw std::runtime_error("option missing from command line");
	} else if (argc > 2) {
		throw std::runtime_error("unrecognized option");
	}

	core::engine e{std::string{argv[1]}};
	e.run();
	return 0;
}

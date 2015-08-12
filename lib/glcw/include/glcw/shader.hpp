#ifndef __GLCW_SHADER_HPP__
#define __GLCW_SHADER_HPP__

#include <memory>
#include <string>
#include <vector>

#include <GL/glew.h>

namespace glcw {
	class shader {
	public:
		shader(const std::string& name, GLenum type, const std::string& source);
		~shader();

		shader(const shader& rhs) = delete;
		shader& operator=(const shader& rhs) = delete;

		shader(shader&& rhs) noexcept;
		shader& operator&(shader&& rhs) noexcept;

	private:
		GLenum type_;
		GLuint id_;

		friend class program;
	};

	using shader_ptr = std::shared_ptr<const shader>;
}

#endif
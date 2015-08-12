#include <glcw/shader.hpp>
#include <utils/logger.hpp>

using namespace utils;

namespace glcw {
	shader::shader(const std::string& name, GLenum type, const std::string& source)
	: type_{type}, id_{glCreateShader(type_)} {
		auto source_ptr = source.c_str();
		glShaderSource(id_, 1, &source_ptr, nullptr);

		auto source_length = 0;
		glGetShaderiv(id_, GL_SHADER_SOURCE_LENGTH, &source_length);
		if (source_length == 0) {
			utils::log(LOG_FATAL) << "length of shader source is zero" << std::endl;
		}

		glCompileShader(id_);

		auto log_length = 0;
		glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &log_length);
		if (log_length > 1) {
			auto log_buffer = std::vector<char>(log_length);
			glGetShaderInfoLog(id_, log_length, nullptr, log_buffer.data());
			utils::log(LOG_DEBUG) << "compilation log of " << name << ":" << std::endl;
			utils::log(LOG_DEBUG) << std::string{log_buffer.begin(), log_buffer.end() - 2} << std::endl;
		}

		auto compile_status = 0;
		glGetShaderiv(id_, GL_COMPILE_STATUS, &compile_status);
		if (!compile_status) {
			glDeleteShader(id_);
			utils::log(LOG_FATAL) << "an error occured during compilation of shader " << name << std::endl;
		}
	}

	shader::~shader() {
		glDeleteShader(id_);
	}

	shader::shader(shader&& rhs) noexcept
	: id_{rhs.id_} {
		rhs.id_ = 0;
	}

	shader& shader::operator&(shader&& rhs) noexcept {
		id_ = rhs.id_;
		rhs.id_ = 0;
		return *this;
	}
}
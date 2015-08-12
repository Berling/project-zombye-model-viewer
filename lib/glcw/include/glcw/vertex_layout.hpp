#ifndef __GLCW_VERTEX_LAYOUT_HPP__
#define __GLCW_VERTEX_LAYOUT_HPP__

#include <memory>
#include <string>
#include <vector>

#include <GL/glew.h>

#include <glcw/buffer.hpp>
#include <glcw/program.hpp>
#include <glcw/vertex_array.hpp>

namespace glcw {
	struct vertex_attribute {
		std::string name;
		uint32_t size;
		GLenum type;
		bool normalized;
		size_t stride;
		intptr_t offset;
		int index;
	};

	class vertex_layout {
	private:
		std::vector<vertex_attribute> vertex_attributes_;

	public:
		vertex_layout() = default;
		~vertex_layout() = default;

		vertex_layout(const vertex_layout& rhs) = delete;
		vertex_layout& operator=(const vertex_layout& rhs) = delete;

		vertex_layout(vertex_layout&& rhs) noexcept = default;
		vertex_layout& operator=(vertex_layout&& rhs) noexcept = default;

		void emplace_back(const std::string& name, uint32_t size, GLenum type, bool normalized, size_t stride, intptr_t offset, int index = 0);
		void setup_layout(vertex_array& vertex_array, const std::unique_ptr<vertex_buffer>* buffers) noexcept;
		void setup_layout(vertex_array& vertex_array, vertex_buffer* buffers) noexcept;
		void setup_program(program& program, const std::string& fragcolor_name) noexcept;
	};
}

#endif
#ifndef __GLCW_VERTEX_ARRAY_HPP__
#define __GLCW_VERTEX_ARRAY_HPP__

#include <GL/glew.h>

#include <glcw/buffer.hpp>

namespace glcw {
	class vertex_array {
	public:
		vertex_array() noexcept;
		~vertex_array();

		vertex_array(const vertex_array& rhs) = delete;
		vertex_array& operator=(const vertex_array& rhs) = delete;

		vertex_array(vertex_array&& rhs) noexcept;
		vertex_array& operator=(vertex_array&& rhs) noexcept;

		void bind() const noexcept {
			glBindVertexArray(id_);
		}

		void bind_index_buffer(const index_buffer& buffer) const noexcept {
			bind();
			buffer.bind();
		}

	private:
		void bind_vertex_attribute(const vertex_buffer& buffer, uint32_t index, int32_t size, GLenum type,
		bool normalized, size_t stride, intptr_t offset) noexcept;
		void bind_vertex_attributei(const vertex_buffer& buffer, uint32_t index, int32_t size, GLenum type,
		size_t stride, intptr_t offset) noexcept;

	private:
		GLuint id_;

		friend class vertex_layout;
	};
}

#endif
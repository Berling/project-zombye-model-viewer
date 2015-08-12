#ifndef __RENDERING_VERTEX_HPP__
#define __RENDERING_VERTEX_HPP__

#include <glm/glm.hpp>

namespace rendering {
	struct vertex {
		glm::vec3 position;
        glm::vec2 texcoord;
        glm::vec3 normal;
        glm::vec3 tangent;
	};
}

#endif

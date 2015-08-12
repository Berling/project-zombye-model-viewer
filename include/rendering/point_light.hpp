#ifndef __RENDERING_POINT_LIGHT__
#define __RENDERING_POINT_LIGHT__

#include <glm/glm.hpp>

namespace rendering {
	struct point_light {
		glm::vec3 position;
		glm::vec3 color;
		float radius;
	};
}

#endif
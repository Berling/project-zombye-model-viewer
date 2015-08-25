#version 330

in vec3 _position;
in vec2 _texcoord;
in vec3 _normal;
in vec3 _tangent;
in ivec4 _index;
in vec4 _weight;

out vec3 normal_;
out vec3 position_world_;
out vec2 texcoord_;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normal;
uniform mat4 pose[250];

void main() {
	texcoord_ = _texcoord;

	vec4 pos = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 nor = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 tan = vec4(0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < 4; ++i) {
		pos += _weight[i] * pose[_index[i]] * vec4(_position, 1.0);
		nor += _weight[i] * pose[_index[i]] * vec4(_normal, 0.0);
	}

	normal_ = nor.xyz;
	position_world_ = (model * vec4(pos.xyz, 1.0)).xyz;
	gl_Position = projection * view * model * vec4(pos.xyz, 1.0);
}

#version 330

in vec3 _position;
in vec2 _texcoord;
in vec3 _normal;

out vec3 normal_;
out vec3 position_world_;
out vec2 texcoord_;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normal;

void main() {
	normal_ = _normal;
	texcoord_ = _texcoord;
	position_world_ = (model * vec4(_position, 1.0)).xyz;
	gl_Position = projection * view * model * vec4(_position, 1.0);
}

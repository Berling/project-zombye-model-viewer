#version 330

in vec3 normal_;
in vec3 position_world_;
in vec2 texcoord_;

out vec4 albedo_color;
out vec3 normal_color;
out vec4 specular_color;
out vec4 position_color;

uniform sampler2D color_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_texture;

void main() {
	normal_color = normalize(normal_);
	albedo_color = texture(color_texture, texcoord_);
	specular_color = texture(specular_texture, texcoord_);
	position_color = vec4(position_world_, 1.0);
}

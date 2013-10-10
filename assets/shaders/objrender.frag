#version 430 core

in vec2 vs_uv;
in vec3 vs_normal;
in vec3 vs_color;

out vec4 color;

uniform sampler2D Texture1;

void main(){
  color = texture(Texture1, vec2(vs_uv.x, 1.0 - vs_uv.y));
}


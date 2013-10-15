#version 430 core

in vec3 vs_pos;
in vec3 vs_normal;
in vec2 vs_uv;

layout (location = 0) out vec3 fs_position;
layout (location = 1) out vec3 fs_normal;
layout (location = 2) out vec3 fs_color;

uniform sampler2D Texture1;

void main(){
  fs_position = vs_pos;
  fs_normal = vs_normal;
  fs_color = texture(Texture1, vec2(vs_uv.x, 1.0 - vs_uv.y)).rgb;
}


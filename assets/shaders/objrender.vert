#version 430 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform vec3 DiffuseColor;

out vec2 vs_uv;
out vec3 vs_normal;
out vec3 vs_color;

void main(){
  gl_Position =  MVP * vec4(pos, 1);
  vs_uv = uv;
  vs_normal = normalize((inverse(MVP)*vec4(normalize(normal), 1))).xyz;
  vs_color = DiffuseColor;
}


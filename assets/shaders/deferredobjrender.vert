#version 430 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat3 NormalMat;
uniform mat4 ModelViewMat;

uniform float TextureRepeat = 1.0f;

out vec3 vs_pos;
out vec3 vs_normal;
out vec2 vs_uv;

void main(){
  gl_Position = MVP * vec4(pos, 1);
  vs_pos    = vec3(ModelViewMat * vec4(pos, 1.0f));
  vs_normal = normalize(NormalMat * normal);
  vs_uv     = TextureRepeat * uv;
}


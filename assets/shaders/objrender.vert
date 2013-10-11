#version 430 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat3 NormalMat;
uniform mat4 ModelViewMat;

out vec2 vs_uv;
out vec3 vs_light;

void main(){
  gl_Position = MVP * vec4(pos, 1);
  vs_uv = uv;

  // Normals to eye space
  vec3 tnorm =  normalize(NormalMat * normal);
  // Eye coordinate in eye space
  vec4 eyeCoord = ModelViewMat * vec4(pos, 1.0);
  // Put light in eye space
  vec4 lightPos = ModelViewMat * vec4(8,10, 8,1);

  // Light vector
  vec3 s = normalize(vec3(lightPos - eyeCoord));
  vs_light = vec3(0.07, 0.08, 0.09) + vec3(1.0f) * max(dot(s, tnorm), 0.0);
}


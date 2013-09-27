#version 430 core

layout(location = 0) in vec3 vpos;
layout(location = 1) in vec2 tpos;

uniform ivec2 Viewport;

out vec2 vs_texpos;

void main(){
	gl_Position = vec4(vpos.x / Viewport.x,
                     vpos.y / Viewport.y, -1.0, 1);
  vs_texpos = tpos;
}


#version 430 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vpos;

// Output data ; will be interpolated for each fragment.
out vec2 vs_texpos;

void main(){
	gl_Position =  vec4(vpos,1);
	vs_texpos = (vpos.xy + vec2(1,1)) / 2.0;
}


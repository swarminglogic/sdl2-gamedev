#version 430 core

out vec4 color;
// in vec4 vs_color;
in vec2 vs_texpos;

uniform sampler2D Tex1;

void main()
{
  //  color = vec4(vs_texpos.x, 0, vs_texpos.y, 1.0);
  vec4 imgcol = texture(Tex1, vs_texpos);
  // if (imgcol.a < 0.016)
  //   discard;
  color = imgcol;
}

#version 430 core

in vec2 vs_texpos;

out vec4 color;

uniform sampler2D Tex1;
uniform float time;

float luma(vec3 color) {
  return dot(vec3(0.2126, 0.7152, 0.0722), color);
}

vec4 test00SimplePass() {
  return texture(Tex1, vs_texpos).rgba;
}

vec4 test01EdgeDetection() {
  float dx = 0.00125; // Pixel width
  float dy = 0.001666667; // Pixel height

  float s00 = luma(texture(Tex1, vs_texpos + vec2(-dx,  dy)).rgb);
  float s10 = luma(texture(Tex1, vs_texpos + vec2(-dx, 0.0)).rgb);
  float s20 = luma(texture(Tex1, vs_texpos + vec2(-dx, -dy)).rgb);
  float s01 = luma(texture(Tex1, vs_texpos + vec2(0.0,  dy)).rgb);
  float s21 = luma(texture(Tex1, vs_texpos + vec2(0.0, -dy)).rgb);
  float s02 = luma(texture(Tex1, vs_texpos + vec2( dx,  dy)).rgb);
  float s12 = luma(texture(Tex1, vs_texpos + vec2( dx, 0.0)).rgb);
  float s22 = luma(texture(Tex1, vs_texpos + vec2( dx, -dy)).rgb);

  float sx = s00 + 2*s10 + s20 - (s02 + 2*s12 + s22);
  float sy = s00 + 2*s01 + s02 - (s20 + 2*s21 + s22);
  float dist = sx * sx + sy * sy;

  if (dist > 0.02)
    return vec4(0.0, 0.0, 0.0, 1.0);
  else
    return vec4(1);;

  // vec4 col = test00SimplePass();
  // float a = 0.1;
  // if (dist > 0.1)
  //   return a * col + (1-a) * vec4(0.0, 0.0, 0.0, 1.0);
  // else
  //   return test00SimplePass();
}

vec4 test02Wobbly() {
  return texture(Tex1,
                  (vs_texpos + 0.005 *
                   vec2( sin( 5*time + 80.0 * vs_texpos.x),
                         cos( 5*time + 60.0 * vs_texpos.y)) ));
}


vec3 gammaCorrect(vec3 color, float gamma) {
  return pow(color, vec3(1.0/gamma));
}

vec4 test03GammaCorrect() {
  return vec4(gammaCorrect(test00SimplePass().rgb, 1.3), 1.0);
}


vec4 test03GammaCorrect(vec4 color) {
  return vec4(gammaCorrect(color.rgb, 1.3), 1.0);
}



void main(){
  // float time = 0;

  // color = test00SimplePass();
  // color = test01EdgeDetection();
	// color = test02Wobbly();
	color = test03GammaCorrect();
	// color = test03GammaCorrect(test01EdgeDetection());
}

#version 430 core

in vec2 vs_texpos;

uniform sampler2D PositionData;
uniform sampler2D NormalData;
uniform sampler2D ColorData;
uniform vec4 lightPos;
uniform ivec2 viewport;
uniform float time;

out vec4 fragment;

vec3 test01EdgeDetection() {
  const vec2 d = 1.0f / viewport;

  const float fac2 = 10.0f;
  const float s00 = log(-texture(PositionData, vs_texpos + vec2(-d.x,  d.y)).z) / fac2;
  const float s10 = log(-texture(PositionData, vs_texpos + vec2(-d.x,  0.0)).z) / fac2;
  const float s20 = log(-texture(PositionData, vs_texpos + vec2(-d.x, -d.y)).z) / fac2;
  const float s01 = log(-texture(PositionData, vs_texpos + vec2( 0.0,  d.y)).z) / fac2;
  const float s21 = log(-texture(PositionData, vs_texpos + vec2( 0.0, -d.y)).z) / fac2;
  const float s02 = log(-texture(PositionData, vs_texpos + vec2( d.x,  d.y)).z) / fac2;
  const float s12 = log(-texture(PositionData, vs_texpos + vec2( d.x,  0.0)).z) / fac2;
  const float s22 = log(-texture(PositionData, vs_texpos + vec2( d.x, -d.y)).z) / fac2;

  const float sx = s00 + 2*s10 + s20 - (s02 + 2*s12 + s22);
  const float sy = s00 + 2*s01 + s02 - (s20 + 2*s21 + s22);
  const float dist = sx * sx + sy * sy;

  if (dist > 0.0004)
    return vec3(0.0, 0.0, 0.0);
  else
    return vec3(1);;
}


vec3 test03GammaCorrect(vec3 color, float gamma) {
  return pow(color, vec3(1.0/gamma));;
}

vec4 wobblySample(sampler2D tex) {
  return texture(tex,
                 (vs_texpos + 0.005 *
                  vec2( sin( 5*time + 80.0 * vs_texpos.x),
                        cos( 5*time + 60.0 * vs_texpos.y)) ));
}


void main(){
  const vec3 pos = vec3(texture(PositionData, vs_texpos));
  const vec3 norm = vec3(texture(NormalData, vs_texpos));
  const vec3 col = vec3(texture(ColorData, vs_texpos));

  const vec3 s = normalize(lightPos.xyz - pos);
  const vec3 light = vec3(0.29, 0.28, 0.27) + vec3(1.0f) * max(dot(s, norm), 0.0);

  fragment = vec4(test03GammaCorrect(test01EdgeDetection() * light * col, 1.3), 1);

  // vec3 depthFog = vec3(log(-pos.z/6.5)/1);
  // fragment = vec4(depthFog, 1);
}

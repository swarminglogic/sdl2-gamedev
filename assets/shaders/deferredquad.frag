#version 430 core

in vec2 vs_texpos;

uniform sampler2D PositionData;
uniform sampler2D NormalData;
uniform sampler2D ColorData;
uniform vec4 lightPos;
uniform ivec2 viewport;
uniform float time;

layout (binding = 0, std140) uniform SSAO_DATA
{
    vec4 pos[64];
    vec4 rvec[64];
} ssao;

out vec4 fragment;

vec3 test01EdgeDetection() {
  const vec2 d = 0.5 / viewport;

  const float fac2 = 10.0f;
  const float s00 =
    log(-texture(PositionData,vs_texpos + vec2(-d.x,  d.y)).z) / fac2;
  const float s10 =
    log(-texture(PositionData, vs_texpos + vec2(-d.x,  0.0)).z) / fac2;
  const float s20 =
    log(-texture(PositionData, vs_texpos + vec2(-d.x, -d.y)).z) / fac2;
  const float s01 =
    log(-texture(PositionData, vs_texpos + vec2( 0.0,  d.y)).z) / fac2;
  const float s21 =
    log(-texture(PositionData, vs_texpos + vec2( 0.0, -d.y)).z) / fac2;
  const float s02 =
    log(-texture(PositionData, vs_texpos + vec2( d.x,  d.y)).z) / fac2;
  const float s12 =
    log(-texture(PositionData, vs_texpos + vec2( d.x,  0.0)).z) / fac2;
  const float s22 =
    log(-texture(PositionData, vs_texpos + vec2( d.x, -d.y)).z) / fac2;

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


float calcSSAO(const vec3 N, const float depth) {
  // Configuration constants
  const float ssao_radius = 0.25;
  const uint  point_count = 10;

  // Accumulation
  float occ = 0.0;

  const int n = (int(gl_FragCoord.x * 139.23f + 120.232f) *
                 int(gl_FragCoord.y * 3137.15f + 234.8f)) ^ int(depth);
  const float r = (ssao.rvec[n & 63].r + 3.0) * 0.1;

  for (int i = 0; i < point_count; i++) {
    vec3 dir = ssao.pos[i].xyz;

    if (dot(N, dir) < 0.0)
      dir = -dir;

    const float z = depth - dir.z * r;
    const float sample_depth =
      -texture(PositionData,
               (vs_texpos + dir.xy * r * ssao_radius)).z;

    const float d = pow(sample_depth - depth, 2);
    if ((z - sample_depth) > 0.0) {
      occ += 2.0 / (1.0 + d);
    }
  }
  return 1.0 - occ / point_count;
}

void main(){

  const vec3 pos  = vec3(texture(PositionData, vs_texpos));
  const vec3 norm = vec3(texture(NormalData, vs_texpos));
  const vec3 col  = vec3(texture(ColorData, vs_texpos));

  const vec3 s = normalize(lightPos.xyz - pos);
  const vec3 light = (2 * vec3(0.19, 0.18, 0.17) +
                      vec3(1.0f) * max(dot(s, norm), 0.0));

  // // Basic color
  // fragment = vec4(test03GammaCorrect(col, 1.3), 1);

  // // + diffuse shading
  // fragment = vec4(test03GammaCorrect(col * light, 1.3), 1);

  // // + edge detection contour
  // fragment = vec4(test03GammaCorrect(test01EdgeDetection() *
  //                                    col *
  //                                    light, 1.3), 1);

  // + SSAO
  const float ssaoFactor = calcSSAO(norm, -pos.z);
  fragment = vec4(test03GammaCorrect(test01EdgeDetection() *
                                     mix(light,vec3(ssaoFactor), 0.3) *
                                     col, 1.3), 1);

  // vec3 depthFog = vec3(log(-pos.z/6.5)/1);
  // fragment = vec4(depthFog, 1);
}

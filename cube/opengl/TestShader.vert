const char* TESTSHADER_VERT = STRINGIFY(
uniform vec3 u_FogDist;
attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texCoord;
attribute vec4 a_color;
varying vec2 v_texCoord;
varying vec4 v_color;
varying float v_factor;
void main(void) {
  vec4 pos_in_world = uc_MVMatrix * vec4(a_position, 1.0);
  vec4 normal_in_world = uc_MVMatrix * vec4(a_normal, 0.0);
  normal_in_world = normalize(normal_in_world);

  if (u_FogDist.z > 0.5) {
      float eyeDist = sqrt((pos_in_world.x - uc_Eye.x) * (pos_in_world.x - uc_Eye.x) +
                       (pos_in_world.y - uc_Eye.y) * (pos_in_world.y - uc_Eye.y) +
                       (pos_in_world.z - uc_Eye.z) * (pos_in_world.z - uc_Eye.z) );
      v_factor = clamp((u_FogDist.x - eyeDist) / (u_FogDist.x - u_FogDist.y), 0.0, 1.0);
  }
  else {
      v_factor = -1.0;
  }

  v_color = doLighting(pos_in_world, normal_in_world.xyz, uc_Eye.xyz) * a_color;
  v_texCoord = a_texCoord;
  gl_Position = uc_MVPMatrix*vec4(a_position, 1.0);
  // gl_Position.y = -gl_Position.y; //enable the code in QT
}
);

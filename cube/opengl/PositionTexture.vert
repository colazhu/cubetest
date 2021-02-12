const char* POSITIONTEXTURE_VERT = STRINGIFY(
attribute vec3 a_position;				
attribute vec2 a_texCoord;
varying vec2 v_texCoord;
varying float v_alpha;
void main(void) {
  v_texCoord = a_texCoord;
  v_alpha = uc_Time.x;
  gl_Position = uc_MVPMatrix*vec4(a_position, 1.0);
  // gl_Position.y = -gl_Position.y; //enable the code in QT
}
);

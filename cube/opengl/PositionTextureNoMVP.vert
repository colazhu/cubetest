const char* POSITIONTEXTURENOMVP_VERT = STRINGIFY(
attribute vec3 a_position;				
attribute vec2 a_texCoord;
varying vec2 v_texCoord;
void main(void) {
  v_texCoord = a_texCoord;
  gl_Position = vec4(a_position.x, a_position.y, 0.0, 1.0);
  // gl_Position.y = -gl_Position.y; //enable the code in QT
}
);

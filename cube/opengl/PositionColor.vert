const char* POSITIONCOLOR_VERT = STRINGIFY(
attribute vec3 a_position;
attribute vec4 a_color;
attribute vec2 a_texCoord;
varying vec4 v_color;
varying vec2 v_texCoord;
void main() {
  v_texCoord = a_texCoord;
  v_color = vec4(a_color.rgb, a_color.a * uc_Time.x);
  // v_color = a_color;
  gl_Position = uc_MVPMatrix*vec4(a_position, 1.0);
  //gl_Position.y = -gl_Position.y; //enable the code in QT
} 
);

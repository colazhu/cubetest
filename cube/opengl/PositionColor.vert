const char* POSITIONCOLOR_VERT = STRINGIFY(
attribute vec3 a_position;
attribute vec4 a_color;
varying vec4 v_color;
void main() {
  v_color = vec4(a_color.rgb, a_color.a * uc_Time.x);
  gl_Position = uc_MVPMatrix*vec4(a_position, 1.0);
  //gl_Position.y = -gl_Position.y; //enable the code in QT
} 
);

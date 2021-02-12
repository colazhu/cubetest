const char* POSITIONCOLORSIZE_VERT = STRINGIFY(
attribute vec3 a_position;
attribute vec4 a_color;
attribute float a_size;
varying vec4 v_color;
void main() {								
  v_color = a_color;
  gl_Position = uc_MVPMatrix * vec4(a_position,1.0);
  //gl_Position.y = -gl_Position.y; //enable the code in QT 
  gl_PointSize = a_size;
} 
);
const char* POSITION_VERT = STRINGIFY(
attribute vec3 a_position;
void										
main() {
  // gl_Position = uc_MVMatrix*vec4(a_position, 1.0);
  gl_Position = uc_MVPMatrix*vec4(a_position, 1.0);
  // gl_Position.y = -gl_Position.y; //enable the code in QT
} 
);

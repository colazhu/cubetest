const char* POSITIONCOLOR_FRAG = STRINGIFY(
varying vec4 v_color;
void main(){
    gl_FragColor = v_color;
}
);

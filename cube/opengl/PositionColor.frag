const char* POSITIONCOLOR_FRAG = STRINGIFY(
varying vec2 v_texCoord;
varying vec4 v_color;
void main(){
    // gl_FragColor = v_color
    gl_FragColor = vec4(v_color.xyz, sqrt(v_texCoord.y));
}
);

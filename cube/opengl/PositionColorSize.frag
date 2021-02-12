const char* POSITIONCOLORSIZE_FRAG = STRINGIFY(
varying vec4 v_color;
void main(){
    vec4 texColor = texture2D(uc_Texture0, gl_PointCoord);
    //gl_FragColor = vec4(texColor.rgb * v_color.rgb, texColor.a);
    gl_FragColor = texColor * v_color;
}
);

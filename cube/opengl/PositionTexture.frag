const char* POSITIONTEXTURE_FRAG = STRINGIFY(
varying vec2 v_texCoord;
varying float v_alpha;
void
main(void) {
    gl_FragColor = texture2D(uc_Texture0, v_texCoord);
    gl_FragColor.a = v_alpha;
}
);

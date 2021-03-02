const char* POSITIONTEXTURENOMVP_FRAG = STRINGIFY(
uniform float u_TextureRatio;
varying vec2 v_texCoord;
void
main(void) {
    gl_FragColor = texture2D(uc_Texture0, v_texCoord);
    // gl_FragColor = vec4(1.0, 0.0,0.0,1.0);
}
);

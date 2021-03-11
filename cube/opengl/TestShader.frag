const char* TESTSHADER_FRAG = STRINGIFY(
uniform vec4 u_FogColor;
uniform float u_TextureRatio;
varying vec2 v_texCoord;
varying vec4 v_color;
varying vec4 v_specular;
varying float v_factor;
void
main(void) {
    // gl_FragColor = v_color + v_specular;
    // gl_FragColor = vec4(v_color.xyz, sqrt(v_texCoord.y)) + v_specular;
    gl_FragColor = texture2D(uc_Texture0, vec2(v_texCoord.x, v_texCoord.y*u_TextureRatio + 1.0 - u_TextureRatio)) * v_color;
    gl_FragColor += vec4(v_specular.xyz, 0.0);
    if (v_factor > 0.0) {
        gl_FragColor = gl_FragColor * v_factor + u_FogColor * (1.0 - v_factor);
    }
    // gl_FragColor = vec4(0.1,0.1,0.1,0.1);
}
);

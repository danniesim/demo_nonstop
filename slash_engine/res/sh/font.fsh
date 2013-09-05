
precision mediump float;

uniform sampler2D texture_1;
uniform vec4 color;

varying vec2 ex_TexCoord;

void main(void)
{
	// Use red channel for alpha
    vec4 tex_color = vec4(color.rgb, texture2D(texture, ex_TexCoord).r);

    gl_FragColor = tex_color;
}

precision mediump float;

uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform vec4 color;

varying vec2 ex_TexCoord;

void main(void)
{
	// Use second texture red channel as alpha
    vec4 tex_color = vec4(texture2D(texture_1, ex_TexCoord).rgb * color.rgb, texture2D(texture_2, ex_TexCoord).r);

    gl_FragColor = tex_color;
}
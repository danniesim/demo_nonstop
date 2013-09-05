//#version 120

precision highp float;

uniform sampler2D texture_1;

varying vec2 ex_TexCoord;
varying vec3 ex_Color;

void main(void)
{
    vec4 tex_color = texture2D(texture_1, ex_TexCoord);

    gl_FragColor = vec4(ex_Color, 1.0) * tex_color;
}
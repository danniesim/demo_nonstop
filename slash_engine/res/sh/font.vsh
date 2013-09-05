
uniform mat4 modelMat;

attribute vec3 in_Position;
attribute vec2 in_TexCoord;

varying vec2 ex_TexCoord;

void main(void)
{
	ex_TexCoord.s = in_TexCoord.s;
    ex_TexCoord.t = in_TexCoord.t;
    
	gl_Position = modelMat * vec4(in_Position, 1.0);

}
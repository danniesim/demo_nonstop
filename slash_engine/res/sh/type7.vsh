//#version 120
  
uniform mat4 modelMatrix;  
uniform mat3 normalMatrix; 
uniform mat4 arrInfMatrix[32];

attribute vec3 in_Position;
attribute vec3 in_Normal;
attribute vec2 in_TexCoord;
attribute vec4 in_Weights;
attribute vec4 in_Infs;

varying vec2 ex_TexCoord;
varying vec3 ex_Color;

void main(void)
{
	// Calculate influences based on weight
    mat4 matTransform = arrInfMatrix[int(in_Infs.x)] * in_Weights.x;
    matTransform += arrInfMatrix[int(in_Infs.y)] * in_Weights.y;
    matTransform += arrInfMatrix[int(in_Infs.z)] * in_Weights.z;
    matTransform += arrInfMatrix[int(in_Infs.w)] * in_Weights.w;

	// put vertex normal into eye coords
    vec3 ec_light_dir = vec3(1, 1, 1);
    vec3 ec_normal = normalize(normalMatrix * mat3(matTransform) * in_Normal);
    // emit diffuse scale factor, texcoord, and position
    vec3 ambient = vec3(0.2, 0.2, 0.3);
    vec3 light_col = vec3(0.8, 0.8, 0.7);
    float diffuse = max(dot(ec_light_dir, ec_normal), 0.0);
    ex_Color = ambient + (diffuse * light_col);
	
	ex_TexCoord.s = in_TexCoord.s;
    ex_TexCoord.t = in_TexCoord.t;  
    
    vec4 objectPos = matTransform * vec4( in_Position, 1.0 );
    //vec4 objectPos = vec4( in_Position, 1.0 );
	gl_Position = modelMatrix * objectPos;

}
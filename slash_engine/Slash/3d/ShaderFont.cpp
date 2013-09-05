/*
 * ShaderFont.cpp
 *
 *  Created on: Sep 14, 2011
 *      Author: prophesci
 */

#include "ShaderFont.h"
#include "Mesh.h"

namespace slh {

ShaderFont::ShaderFont() {
}

ShaderFont::~ShaderFont() {
}

char* ShaderFont::vshFileName() { return (char*)"font.vsh"; }
char* ShaderFont::fshFileName() { return (char*)"font.fsh"; }

void ShaderFont::bindAttribs() {
	glBindAttribLocation(muiProgram, GL_IDX_VERTEX, "in_Position");
	glBindAttribLocation(muiProgram, GL_IDX_UV, "in_TexCoord");
}

void ShaderFont::bindUniforms() {
	mModelMatLoc = glGetUniformLocation(muiProgram, "modelMat"); // Get the location of our model matrix in the shader
    mTextureLoc[0] = glGetUniformLocation(muiProgram, "texture_1");
    mColorLoc = glGetUniformLocation(muiProgram, "color");
}

void ShaderFont::setTransform(Transform* iTransform) {
    glUniformMatrix4fv(mModelMatLoc, 1, GL_FALSE, ((TransformType1*)iTransform)->matGlMvp());
}

void ShaderFont::setColor(GLfloat* iColor) {
    glUniform4fv(mColorLoc, 1, iColor); // Send our projection matrix to the shader
}



}

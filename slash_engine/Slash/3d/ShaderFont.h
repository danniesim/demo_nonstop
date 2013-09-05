/*
 * ShaderFont.h
 *
 *  Created on: Sep 14, 2011
 *      Author: prophesci
 *
 *  GLSL shader interface to render Fonts.
 */

#ifndef SHADERFONT_H_
#define SHADERFONT_H_

#include "Shader.h"
#include "../Library/Library.h"
#include "TransformUi.h"

namespace slh {

class ShaderFont :public Shader {
protected:
	GLint mModelMatLoc;
	GLint mColorLoc;

	char* vshFileName();
	char* fshFileName();
	void bindAttribs();
	void bindUniforms();

public:
	ShaderFont();
	virtual ~ShaderFont();

	void setTransform(Transform* iTransform);
	void setColor(GLfloat* iColor);
};

}

#endif /* SHADERFONT_H_ */

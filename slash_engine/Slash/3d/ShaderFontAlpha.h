/*
 * ShaderFontAlpha.h
 *
 *  Created on: May 11, 2012
 *      Author: prophesci
 *
 * Adds function to support 2nd texture for font alpha.
 * Note override fshFileName() to use a different fragment shader
 */

#ifndef SHADERFONTALPHA_H_
#define SHADERFONTALPHA_H_

#include "ShaderFont.h"
#include "../Library/Library.h"
#include "TransformUi.h"

namespace slh {

class ShaderFontAlpha :public ShaderFont {
protected:
	char* fshFileName();
	void bindUniforms();

public:
	ShaderFontAlpha();
	virtual ~ShaderFontAlpha();
};

}

#endif /* SHADERFONTALPHA_H_ */

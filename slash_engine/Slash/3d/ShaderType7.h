/*
 * ShaderType7.h
 *
 *  Created on: Jul 19, 2011
 *      Author: prophesci
 *
 *  Interface to render animated skeletal skinned mesh.
 */

#ifndef SHADERTYPE7_H_
#define SHADERTYPE7_H_

#include "ShaderType3.h"
#include "../Library/Library.h"
#include "Animation.h"

namespace slh {

class ShaderType7 :public ShaderType3 {
protected:
	GLint mInfMatLoc;

	virtual char* vshFileName();
	virtual char* fshFileName();
	void bindAttribs();
	virtual void bindUniforms();

public:
	ShaderType7();
	virtual ~ShaderType7();
	void setAnimation(Animation* iAnimation);
};

}

#endif /* SHADERTYPE7_H_ */

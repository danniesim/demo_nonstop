/*
 * TextureGroup.h
 *
 *  Created on: Sep 8, 2011
 *      Author: prophesci
 *
 *  Simple interface to manage grouping of textures and switching of texture units
 */

#ifndef TEXTUREGROUP_H_
#define TEXTUREGROUP_H_

#include "RenderDevice.h"
#include "Texture.h"
#include "Shader.h"

namespace slh {

class TextureGroup {
	RenderDevice* mRenderDevice;
	std::vector<Texture*>* mvecTexture;

public:
	TextureGroup(RenderDevice* iRenderDevice);
	virtual ~TextureGroup();
	void destroy();

	void pushTexture(Texture* iTexture);
	void use(Shader* iShader);
};

}

#endif /* TEXTUREGROUP_H_ */

/*
 * TextureGroup.cpp
 *
 *  Created on: Sep 8, 2011
 *      Author: prophesci
 */

#include "TextureGroup.h"

namespace slh {

TextureGroup::TextureGroup(RenderDevice* iRenderDevice) {
	mvecTexture = new std::vector<Texture*>;
	mRenderDevice = iRenderDevice;
}

TextureGroup::~TextureGroup() {
}

void TextureGroup::destroy() {
	for (unsigned int i = 0; i < mvecTexture->size(); ++i) {
		(*mvecTexture)[i]->destroy();
	}
}

void TextureGroup::pushTexture(Texture* iTexture) {
	mvecTexture->push_back(iTexture);
}

void TextureGroup::use(Shader* iShader) {
	for (unsigned int i = 0; i < mvecTexture->size(); ++i) {
		mRenderDevice->setTextureUnit(i);
		iShader->setTexture(i, mRenderDevice->textureUnit());
		(*mvecTexture)[i]->use();
	}
}


}

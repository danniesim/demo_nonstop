/*
 * VisualUi.cpp
 *
 *  Created on: Aug 26, 2011
 *      Author: prophesci
 */

#include "VisualUi.h"
#include "ShaderSquare.h"

#include <glm/gtc/type_ptr.hpp>

namespace slh {

VisualUi::VisualUi(unsigned int iId, unsigned int iOrder, unsigned int iMeshId, unsigned int iShaderId, unsigned int iTextureId, unsigned int iTransformId,
		Bounds* iBounds, bool iSticky) :Visual(iId, iOrder, iMeshId, iShaderId, iTextureId, iTransformId) {
	mSticky = iSticky;
	mBounds = iBounds;
	mValue = 0.0f;
	mTouchId = 0;
}

VisualUi::~VisualUi() {
}

void VisualUi::updateShader(Shader* iShader) {
	glm::vec2 tVec2(value(), 0.0f);
	((ShaderSquare*)iShader)->setUvOffset(glm::value_ptr(tVec2));
}

unsigned int VisualUi::id() {
	return mId;
}

bool VisualUi::isSticky() {
	return mSticky;
}

Bounds* VisualUi::bounds() {
	return mBounds;
}

void VisualUi::setValue(float iValue) {
	mValue = iValue;
}

float VisualUi::value() {
	return mValue;
}

void VisualUi::setTouchId(unsigned int iTouchId) {
	mTouchId = iTouchId;
}

unsigned int VisualUi::touchId() {
	return mTouchId;
}

}

/*
 * VisualUi.h
 *
 *  Created on: Aug 26, 2011
 *      Author: prophesci
 *
 *  User Interface Visual. To be rendered as an interactive mesh.
 *  Collaborates with UserController to notify program of click/taps on UI elements.
 *  Features tap/click bounding area and sticky function (i.e. does button remain down when I slide my finger off the button)
 */

#ifndef VISUALUI_H_
#define VISUALUI_H_

#include "Visual.h"
#include "Bounds.h"
//#include "Shader.h"

namespace slh {

class VisualUi :public Visual {

	bool mSticky;
	Bounds* mBounds;
	float mValue;
	unsigned int mTouchId;

public:
	VisualUi(unsigned int iId, unsigned int iOrderId, unsigned int iMeshId, unsigned int iShaderId, unsigned int iTextureId, unsigned int iTransformId,
			Bounds* iBounds, bool iSticky);
	virtual ~VisualUi();
	virtual void updateShader(Shader* iShader);

	unsigned int id();
	Bounds* bounds();
	bool isSticky();
	void setValue(float iValue);
	float value();
	void setTouchId(unsigned int iTouchId);
	unsigned int touchId();

};

}

#endif /* VISUALUI_H_ */

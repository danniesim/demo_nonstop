/*
 * Animation.h
 *
 *  Created on: Jul 19, 2011
 *      Author: prophesci
 *
 *  Interpolates skeletal animation data based on animation time.
 *  Gives GLFloat array that is passed to the vertex shader for GPU mesh skinning.
 *  Note that the terms Influence and Joint are used interchangeably.
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/compatibility.hpp>

#include <vector>

#include "DataAnimation.h"
#include "RenderDevice.h"
#include "DataMesh.h"
#include "../Util/File.h"

namespace slh {

const float ANIM_FPS = 30.f;
const float ANIM_FLOAT_ERROR_DURATION_COMPENSATION = 0.000001f;

class Animation {
	char *mBuffFile;
	AdHeader* mHeader;
	AdJoint* mJoints;
	glm::mat4 mMatBindPose[MD_MAX_INFS];
	glm::mat4 mMatInvBindPose[MD_MAX_INFS];
	glm::mat4 mMatCreatedInf[MD_MAX_INFS];
	glm::mat4 mMatInvCreatedInf[MD_MAX_INFS];
	glm::mat4 mMatRoot;

	float mDuration;
	float mTime;

	slh::File mFile;

public:
	Animation();
	virtual ~Animation();

	void create(const char* ichFileName);
	void destroy();
	void print();
	AdJoint* getInf(int frame, int infIdx);
	void createInfBindPose();
	void setRootTransform(glm::mat4 iMatrix);
	void createInfPose(float iTime);
	GLfloat* matGlInfs();
	unsigned int numFrames();
	float duration();
	float currentTime();
};

}

#endif /* ANIMATION_H_ */

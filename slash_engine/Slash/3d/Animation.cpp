/*
 * Animation.cpp
 *
 *  Created on: Jul 19, 2011
 *      Author: prophesci
 */

#include "Animation.h"
#include "DataAnimation.h"
#include "../Util/Log.h"

#include <iostream>
#include <math.h>

namespace slh {

Animation::Animation() {
}

Animation::~Animation() {
}

void Animation::create(const char* ichFileName) {
	mFile.loadFile(ichFileName);
	mBuffFile = mFile.getBuffer();

	if (mFile.size() == 0) {
		LOGE("Cannot load animation file.");
		exit(-1);
	}

	mHeader = (AdHeader *)(mBuffFile);
	LOGD("Type: %d, Num Frames: %d", mHeader->type, mHeader->numFrames);

	// Frame data...
	mJoints = (AdJoint*)(mBuffFile + sizeof(AdHeader));

	mDuration = (((float)(mHeader->numFrames - 1))/ANIM_FPS) - ANIM_FLOAT_ERROR_DURATION_COMPENSATION;

	mMatRoot = glm::mat4(1.0f);

}

void Animation::destroy() {
	mHeader = 0;
	mJoints = 0;
	mDuration = 0.f;

	mFile.freeBuffer();
}

// Build a Quaterninon based on Animation Joint
glm::quat buildQuat(AdJoint* adJoint) {
	return glm::quat(adJoint->orient[3], adJoint->orient[0], adJoint->orient[1], adJoint->orient[2]);
}

// Pre-calculates Bind Pose matrices for all Influences in skeleton. Also does the inverse matrix.
void Animation::createInfBindPose() {
	AdJoint* adJoint;
	for (int j = 0; j < mHeader->numInfs; j++) {
		adJoint = getInf(0, j);

		glm::mat4 bindMat = glm::mat4(1.f);
		bindMat = glm::translate(bindMat, glm::vec3(adJoint->translate[0], adJoint->translate[1], adJoint->translate[2]));
		bindMat = bindMat * glm::toMat4(buildQuat(adJoint));

		if (adJoint->parentIdx != -1) {
			mMatBindPose[j] = mMatBindPose[adJoint->parentIdx] * bindMat;
		} else {
			mMatBindPose[j] = bindMat;
		}

		mMatInvBindPose[j] = glm::inverse(mMatBindPose[j]);
	}

}

void Animation::setRootTransform(glm::mat4 iMatrix) {
	mMatRoot = iMatrix;
}

// Where the magic happens
void Animation::createInfPose(float iTime) {

    // Figure out which frame we're on
	float fFrameNum = iTime * ANIM_FPS;
    int iFramNum = (int)floorf(fFrameNum);
    int iFrame0 = iFramNum % (mHeader->numFrames - 1);
    int iFrame1 = iFrame0 + 1;
    assert(iFrame0 >= 0);
    assert(iFrame1 < mHeader->numFrames);

    float fInterpolate = fFrameNum - (float)iFramNum;
    assert(fInterpolate >= 0.f);
    assert(fInterpolate <= 1.0f);

	AdJoint* adJoint0,* adJoint1;
	for (int j = 0; j < mHeader->numInfs; j++) {

		adJoint0 = getInf(iFrame0, j);
		adJoint1 = getInf(iFrame1, j);

		// LERP translation
		glm::vec3 t0 = glm::vec3(adJoint0->translate[0], adJoint0->translate[1], adJoint0->translate[2]);
		glm::vec3 t1 = glm::vec3(adJoint1->translate[0], adJoint1->translate[1], adJoint1->translate[2]);
		glm::mat4 bindMat = glm::translate(glm::mat4(1.f), glm::lerp(t0, t1, fInterpolate));

		// Mix Quaternions
		glm::quat q0 = buildQuat(adJoint0);
		glm::quat q1 = buildQuat(adJoint1);
		glm::quat qm = glm::shortMix(q0, q1, fInterpolate);
		bindMat = bindMat * glm::toMat4(qm);

		// Check if root joint
		if (adJoint0->parentIdx != -1) {
			mMatCreatedInf[j] = mMatCreatedInf[adJoint0->parentIdx] * bindMat;
		} else {
			mMatCreatedInf[j] = mMatRoot * bindMat;
		}

		// Get the finally product to be used by the Vertex Shader
		mMatInvCreatedInf[j] = mMatCreatedInf[j] * mMatInvBindPose[j];
	}

	mTime = iTime;
}

// Debug utility
void Animation::print() {
	AdJoint* adJoint;

	for (int i = 0; i < mHeader->numFrames; i++) {
		LOGD("<Frame %d>", i);
		for (int j = 0; j < mHeader->numInfs; j++) {
			adJoint = getInf(i, j);
			LOGD("<Inf %d, %d> t(%f, %f, %f) o(%f, %f, %f)", adJoint->idx, adJoint->parentIdx,
					adJoint->translate[0], adJoint->translate[1], adJoint->translate[2], adJoint->orient[0], adJoint->orient[1], adJoint->orient[2]);
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

// Return an influence for particular frame of animation and index
AdJoint* Animation::getInf(int frame, int infIdx) {
	if (frame < mHeader->numFrames) {
		if (infIdx < mHeader->numInfs) {
			return &mJoints[(frame * mHeader->numInfs)+ infIdx];
		} else {
			LOGE("Animation inf: %d out of bounds!", infIdx);
		}
	} else {
		LOGE("Animation frame: %d out of bounds!", frame);
	}
	return 0;
}

GLfloat* Animation::matGlInfs() {
	return glm::value_ptr(mMatInvCreatedInf[0]);
}

unsigned int Animation::numFrames() {
	return mHeader->numFrames;
}

float Animation::duration() {
	return mDuration;
}

float Animation::currentTime() {
	return mTime;
}

}

/*
 * IntersectMesh.cpp
 *
 *  Created on: Sep 28, 2011
 *      Author: prophesci
 */

#include "IntersectMesh.h"

namespace slh {

IntersectMesh::IntersectMesh() {
}

IntersectMesh::~IntersectMesh() {
}

void IntersectMesh::create(const char* ichFileName) {
	char *chpBuffFile = NULL;

	mFile = new File();
	mFile->loadFile(ichFileName);
	chpBuffFile = mFile->getBuffer();

	if (mFile->size() == 0) {
		LOGE("Cannot load intersect mesh file.");
		exit(-1);
	}

	MdHeader* header = (MdHeader *)(chpBuffFile);

	mType = header->type;
	mNumVerts = header->numVerts;
	mNumIndices = header->numIndices;

	LOGD("Intersect Mesh Type: %d, Num Verts: %d, Num Indices: %d", mType, mNumVerts, mNumIndices);


	if (mType != 0) {
		LOGE("ERROR: Intersect Mesh needs Type 0!");
		return;
	}

	mvecVertexTransformed.resize(mNumVerts);
	mvecTriangle.resize(mNumIndices);

	unsigned int vertexDataBytes = sizeof(MdMeshType1);

	char* pChar = chpBuffFile + sizeof(MdHeader);
	marrVertex = (MdMeshType1*)pChar;

	pChar += vertexDataBytes * mNumVerts;
	mFace = (GlmVec3Face*)pChar;
	MdIndexC* arrIndex = (MdIndexC*)pChar;

	for (unsigned int i = 0; i < mNumIndices; i++) {
		MdTypeIndexC t0, t1, t2;
		t0 = arrIndex[i][0];
		t1 = arrIndex[i][1];
		t2 = arrIndex[i][2];

		mFace[i][0] = (glm::vec3*)glm::value_ptr(mvecVertexTransformed[t0]);
		mFace[i][1] = (glm::vec3*)glm::value_ptr(mvecVertexTransformed[t1]);
		mFace[i][2] = (glm::vec3*)glm::value_ptr(mvecVertexTransformed[t2]);
	}
}

void IntersectMesh::destroy() {
	mFile->freeBuffer();
}

// The magic method that delivers results
IntersectTriangle* IntersectMesh::intersectRay(glm::vec3 p, glm::vec3 d) {
//	int intersectCount = 0;
	IntersectTriangle* lastIntersect = NULL;

	for (unsigned int i = 0; i < mNumIndices; i++) {
		if (mvecTriangle[i].intersectRay(p, d)) {
			if (lastIntersect != NULL) {
				if (lastIntersect->t() > mvecTriangle[i].t()) {
					lastIntersect = &mvecTriangle[i];
				}
			} else {
				lastIntersect = &mvecTriangle[i];
			}
//			intersectCount++;
		}
	}

	return lastIntersect;
}

// Transform the collision mesh, transform the render mesh correspondingly for best results.
void IntersectMesh::transform(glm::mat4 iMatTransform) {
	for (unsigned int i = 0; i < mvecVertexTransformed.size(); i++) {
		glm::vec3* tVect = (glm::vec3*)marrVertex[i].vertex;
		mvecVertexTransformed[i] = glm::vec3(iMatTransform * glm::vec4(*tVect, 1.0f));
	}

	for (unsigned int i = 0; i < mvecTriangle.size(); i++) {
		mvecTriangle[i].mVector = *mFace[i][0];
		mvecTriangle[i].mE1 = *mFace[i][1] - mvecTriangle[i].mVector;
		mvecTriangle[i].mE2 = *mFace[i][2] - mvecTriangle[i].mVector;
		mvecTriangle[i].mNormal = glm::normalize(glm::cross(mvecTriangle[i].mE1, mvecTriangle[i].mE2));
	}
}

}

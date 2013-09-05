/*
 * IntersectMesh.h
 *
 *  Created on: Sep 28, 2011
 *      Author: prophesci
 *
 *  Returns the polygon in a mesh that intersects given ray.
 *  Mesh must be made entirely of 3-sided polygons.
 *  Mesh data is loaded in .slc format exported by Slash Forge from Autodesk Maya
 */

#ifndef INTERSECTMESH_H_
#define INTERSECTMESH_H_

#include "IntersectTriangle.h"
#include "../3d/DataMesh.h"

#include <vector>

namespace slh {

typedef std::vector<IntersectTriangle> VecIntersectTriangle;
typedef glm::vec3* GlmVec3Ptr;
typedef GlmVec3Ptr GlmVec3Face[3];

class IntersectMesh {
	File* mFile;
	unsigned int mType;
	unsigned int mNumVerts;
	unsigned int mNumIndices;
	MdMeshType1* marrVertex;
	GlmVec3Face* mFace;
	VecIntersectTriangle mvecTriangle;
	std::vector<glm::vec3> mvecVertexTransformed;
public:
	IntersectMesh();
	virtual ~IntersectMesh();

	void create(const char* ichFileName);
	void destroy();

	IntersectTriangle* intersectRay(glm::vec3 p, glm::vec3 d);

	void transform(glm::mat4 iMatTransform);
};

}

#endif /* INTERSECTMESH_H_ */

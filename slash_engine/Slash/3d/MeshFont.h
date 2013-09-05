/*
 * MeshFont.h
 *
 *  Created on: Sep 14, 2011
 *      Author: prophesci
 *
 *  Interfaces with the GPU to render a Font Mesh that displays text.
 *  Note use of OpenGL ES 2.0 Vertex Array Object (VAO) to accelerate rendering.
 */

#ifndef MESHFONT_H_
#define MESHFONT_H_

#include <string>

#include "Mesh.h"
#include "DataFont.h"
#include "Font.h"

using namespace std;

namespace slh {

const int MESH_FONT_MAX_TEXT = 1024;
const int MESH_FONT_GLYPH_NUM_TRIANGLE = 2;
const int MESH_FONT_GLYPH_NUM_VERTEX = 4;
const int MESH_FONT_VERT_SIZE = MESH_FONT_MAX_TEXT * MD_NUM_VERTEX_ELEMENTS * MESH_FONT_GLYPH_NUM_VERTEX;
const int MESH_FONT_VERT_BUFFER_SIZE = MESH_FONT_MAX_TEXT * MD_NUM_VERTEX_ELEMENTS * MESH_FONT_GLYPH_NUM_VERTEX * sizeof(MdTypeVertex);
const int MESH_FONT_UV_SIZE = MESH_FONT_MAX_TEXT * MD_NUM_UV_ELEMENTS * MESH_FONT_GLYPH_NUM_VERTEX;
const int MESH_FONT_IDX_SIZE = MESH_FONT_MAX_TEXT * MD_NUM_INDEX_ELEMENTS * MESH_FONT_GLYPH_NUM_TRIANGLE;

enum FontJustifiction {
	efjLeft,
	efjRight,
	efjCenter
};

class MeshFont :public Mesh {
	struct MeshFontVertexAttrib {
		MdTypeVertex bl_x;
		MdTypeVertex bl_y;
		MdTypeVertex bl_z;
		MdTypeVertex bl_u;
		MdTypeVertex bl_v;

		MdTypeVertex tl_x;
		MdTypeVertex tl_y;
		MdTypeVertex tl_z;
		MdTypeVertex tl_u;
		MdTypeVertex tl_v;

		MdTypeVertex tr_x;
		MdTypeVertex tr_y;
		MdTypeVertex tr_z;
		MdTypeVertex tr_u;
		MdTypeVertex tr_v;

		MdTypeVertex br_x;
		MdTypeVertex br_y;
		MdTypeVertex br_z;
		MdTypeVertex br_u;
		MdTypeVertex br_v;
	};

	char* attribOffsetVert;
	char* attribOffsetUv;

	MeshFontVertexAttrib mVert[MESH_FONT_MAX_TEXT];
	MdTypeIndex* mIdx;

	Font* mFont;

	unsigned int mTextSize;
	FontJustifiction mJustification;

	void setAttrOffsets();
	void enableAttribs();
public:
	MeshFont();
	virtual ~MeshFont();

	void create(Font* iFont, FontJustifiction iJustification);
	void setText(string iText);
};

}

#endif /* MESHFONT_H_ */

/*
 * MeshFont.cpp
 *
 *  Created on: Sep 14, 2011
 *      Author: prophesci
 */

#include "MeshFont.h"

#include "../Util/Log.h"
#include "../Util/File.h"


namespace slh {

MeshFont::MeshFont() {

}

MeshFont::~MeshFont() {

}

void MeshFont::create(Font* iFont, FontJustifiction iJustification) {
	mJustification = iJustification;
	mType = MESHTYPE_FONT;
	mNumVerts = 0;
	mNumIndices = 0;
	mFont = iFont;

	genObjects();
	bindObjects();

	// Pre-make Index buffer
	mIdx = new MdTypeIndex[MESH_FONT_IDX_SIZE];
	for (unsigned short i = 0; i < MESH_FONT_MAX_TEXT; i++) {
		unsigned short mIdxOffset = i * MESH_FONT_GLYPH_NUM_TRIANGLE * MD_NUM_INDEX_ELEMENTS;
		unsigned short mVertOffset = i * MESH_FONT_GLYPH_NUM_VERTEX;
		mIdx[mIdxOffset] = 0 + mVertOffset;
		mIdx[mIdxOffset+1] = 1 + mVertOffset;
		mIdx[mIdxOffset+2] = 2 + mVertOffset;
		mIdx[mIdxOffset+3] = 2 + mVertOffset;
		mIdx[mIdxOffset+4] = 3 + mVertOffset;
		mIdx[mIdxOffset+5] = 0 + mVertOffset;
	}
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, MESH_FONT_IDX_SIZE * sizeof(MdTypeIndex), mIdx, GL_STATIC_DRAW);
	delete mIdx;

	glBufferData(GL_ARRAY_BUFFER, MESH_FONT_VERT_BUFFER_SIZE, mVert, GL_DYNAMIC_DRAW);
}

void MeshFont::setText(string iText) {

	mTextSize = iText.size();

	float tx = 0.0f;
	float ty = 0.0f;

//	LOGD("%d", sizeof(MeshFontVertexAttrib));
//	LOGD("%d", sizeof(MeshFontVertexAttrib[MESH_FONT_MAX_TEXT]));

	unsigned int glyphCount = 0;

	unsigned int j = 0;
	int nextJ = 1;
	if (mJustification == efjRight) {
		j = mTextSize - 1;
		nextJ = -1;
	} else {
		j = 0;
		nextJ = 1;
	}

	// advancer is used to increment left and center justified text
	float advancer = 0.f;

	for (unsigned int i = 0; i < mTextSize; i++) {
		unsigned int tChar = (unsigned int)iText[j];
		j += nextJ;

		std::map<unsigned int, DfGlyph*>::iterator it;

		// Find the font glyph
		it = mFont->mmapDfGlyph.find(tChar);
		if (it != mFont->mmapDfGlyph.end()) {

			DfGlyph* tDfGlyph = (it->second);

			// Advance to next gyphe based on justification of text and glyph metrics
			if (mJustification == efjRight) {
				tx -= tDfGlyph->advance;
			} else {
				tx += advancer;
				advancer = tDfGlyph->advance;
			}

//			LOGD("%d", (unsigned int)&mVert[i]);

			mVert[i].bl_x = tx;
			mVert[i].tl_x = tx;
			mVert[i].tr_x = tx + tDfGlyph->width;
			mVert[i].br_x = mVert[i].tr_x;

			mVert[i].bl_y = ty + tDfGlyph->ascent;
			mVert[i].tl_y = mVert[i].bl_y + tDfGlyph->height;
			mVert[i].tr_y = mVert[i].tl_y;
			mVert[i].br_y = mVert[i].bl_y;

			mVert[i].bl_u = tDfGlyph->u;
			mVert[i].tl_u = mVert[i].bl_u;
			mVert[i].tr_u = tDfGlyph->u + tDfGlyph->width;
			mVert[i].br_u = mVert[i].tr_u;

			mVert[i].bl_v = tDfGlyph->v;
			mVert[i].tl_v = tDfGlyph->v + tDfGlyph->height;
			mVert[i].tr_v = mVert[i].tl_v;
			mVert[i].br_v = mVert[i].bl_v;

			mVert[i].bl_z = 0.0f;
			mVert[i].tl_z = 0.0f;
			mVert[i].tr_z = 0.0f;
			mVert[i].br_z = 0.0f;

			glyphCount++;
		}
	}

	// Adjusts mesh if Center justified
	if (mJustification == efjCenter) {
		float centerOffset = (tx + advancer) / 2;
		for (unsigned int i = 0; i < mTextSize; i++) {
			mVert[i].bl_x -= centerOffset;
			mVert[i].tl_x = mVert[i].bl_x;
			mVert[i].tr_x -= centerOffset;
			mVert[i].br_x = mVert[i].tr_x;
		}
	}

	mNumVerts = glyphCount * MESH_FONT_GLYPH_NUM_VERTEX;
	mNumIndices = glyphCount * MESH_FONT_GLYPH_NUM_TRIANGLE;
	mNumIndexElements = mNumIndices * MD_NUM_INDEX_ELEMENTS;

	setAttrOffsets();
	GLuint vertexBufferSize = vertexDataBytes * mNumVerts;
	attribOffsetIdx = 0;

	bindObjects();

	// Send data to GPU
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertexBufferSize, mVert);

#ifdef SLH_USE_VAO
	enableAttribs();
#endif

	unbindObjects();

}

// Overide to specify offset in data
void MeshFont::setAttrOffsets() {
	attribOffsetVert = 0;
	attribOffsetUv = 0;
	attribOffsetUv += sizeof(MdVertex);

	vertexDataBytes = sizeof(MdTypeVertex) * (MD_NUM_VERTEX_ELEMENTS + MD_NUM_UV_ELEMENTS);
}

// Override to specify mesh attributes
void MeshFont::enableAttribs() {
	glVertexAttribPointer(GL_IDX_VERTEX, MD_NUM_VERTEX_ELEMENTS, GL_FLOAT, GL_FALSE, vertexDataBytes, (GLvoid *)attribOffsetVert);
	glEnableVertexAttribArray(GL_IDX_VERTEX);

	glVertexAttribPointer(GL_IDX_UV, MD_NUM_UV_ELEMENTS, GL_FLOAT, GL_FALSE, vertexDataBytes, (GLvoid *)attribOffsetUv);
	glEnableVertexAttribArray(GL_IDX_UV);
}

}

/*
 * Texture.h
 *
 *  Created on: Jun 14, 2011
 *      Author: prophesci
 *
 * Interface to buffer texture data to the GPU.
 * Note support for iOS (PVR), Android Snapdragon (ATC) and Windows (DXT5) texture formats
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "RenderDevice.h"
#include "../util/File.h"
#include <Slash/Util/Data.h>

#if defined TEXTURE_PVR

	#ifndef MAX
		#define MAX(a, b) (((a) > (b)) ? (a) : (b))
	#endif

	#define PVR_TEXTURE_FLAG_TYPE_MASK	0xff

	enum {
		kPVRTextureFlagTypePVRTC_2 = 24,
		kPVRTextureFlagTypePVRTC_4
	};

	typedef struct _PVRTexHeader {
		uint32_t headerLength;
		uint32_t height;
		uint32_t width;
		uint32_t numMipmaps;
		uint32_t flags;
		uint32_t dataLength;
		uint32_t bpp;
		uint32_t bitmaskRed;
		uint32_t bitmaskGreen;
		uint32_t bitmaskBlue;
		uint32_t bitmaskAlpha;
		uint32_t pvrTag;
		uint32_t numSurfs;
	} PVRTexHeader;
#else
	#define CR_DDS_PIXEL_DATA_OFFSET 128
	#define CR_FOURCC_OFFSET 84
	#define CR_MIPMAP_COUNT_OFFSET 28
	#define CR_LINEAR_SIZE_OFFSET 20
	#define CR_DDS_WIDTH_OFFSET 16
	#define CR_DDS_HEIGHT_OFFSET 12

	//define something for Windows
	//FOURCC codes for DX compressed-texture pixel formats.
	#define FOURCC_DXT1 MAKEFOURCC('D', 'X', 'T', '1')
	#define FOURCC_ATC MAKEFOURCC('A', 'T', 'C', ' ')
	#define FOURCC_ATCA MAKEFOURCC('A', 'T', 'C', 'A')
	#define FOURCC_ATCI MAKEFOURCC('A', 'T', 'C', 'I')
	#define FOURCC_ETC0 MAKEFOURCC('E', 'T', 'C', '0')

	#ifdef SLH_WIN32
		#define FOURCC_DXT3 MAKEFOURCC('D', 'X', 'T', '3')
		#define FOURCC_DXT5 MAKEFOURCC('D', 'X', 'T', '5')
	#endif

#endif

namespace slh {

class Texture {
	GLuint muiTexture;

public:
	Texture();
	virtual ~Texture();
	GLuint create(const char* filename);
	void destroy();
	void use();
};

}

#endif /* TEXTURE_H_ */

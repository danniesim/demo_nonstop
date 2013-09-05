/*
 * Texture.cpp
 *
 *  Created on: Jun 14, 2011
 *      Author: prophesci
 */

#include "Texture.h"
#include "../Util/Log.h"

namespace slh {

Texture::Texture() {
}

Texture::~Texture() {
}


GLuint Texture::create(const char* filename)
{
    char* data;

    muiTexture = 0;
    File* tFile = new File();
    tFile->loadFile(filename);
	if (tFile->size() == 0) {
		LOGE("Cannot load Texture file.");
		exit(-1);
	}

    data = tFile->getBuffer();
    
#if defined TEXTURE_PVR

    int uiMipMapCount = 0;
    const int MAX_MIPMAPS = 128;
    uint8_t* mipMapData[MAX_MIPMAPS];
    int mipMapSize[MAX_MIPMAPS];

	GLuint _name = 0;
	uint32_t _width, _height;
	GLenum _internalFormat;
	int _hasAlpha;

	PVRTexHeader *header = NULL;
	uint32_t flags, pvrTag;
	uint32_t dataLength = 0, dataOffset = 0, dataSize = 0;
	uint32_t blockSize = 0, widthBlocks = 0, heightBlocks = 0;
	uint32_t width = 0, height = 0, bpp = 4;
	uint8_t *bytes = NULL;
	uint32_t formatFlags;

	header = (PVRTexHeader *)data;

	pvrTag = header->pvrTag;
	
	static char* gPVRTexIdentifier = (char*)"PVR!";

	if (gPVRTexIdentifier[0] != ((pvrTag >>  0) & 0xff) ||
		gPVRTexIdentifier[1] != ((pvrTag >>  8) & 0xff) ||
		gPVRTexIdentifier[2] != ((pvrTag >> 16) & 0xff) ||
		gPVRTexIdentifier[3] != ((pvrTag >> 24) & 0xff)) {
		return false;
	}

	flags = header->flags;
	formatFlags = flags & PVR_TEXTURE_FLAG_TYPE_MASK;

	if (formatFlags == kPVRTextureFlagTypePVRTC_4 || formatFlags == kPVRTextureFlagTypePVRTC_2)
	{
        
        if (header->bitmaskAlpha)
			_hasAlpha = true;
		else
			_hasAlpha = false;

		if (formatFlags == kPVRTextureFlagTypePVRTC_4) {
            if (_hasAlpha == true) {
                _internalFormat = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
            }
            else {
                _internalFormat = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
            }
        }
		else if (formatFlags == kPVRTextureFlagTypePVRTC_2) {
            if (_hasAlpha == true) {
                _internalFormat = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
            }
            else {
                _internalFormat = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
            }
        }

		_width = width = header->width;
		_height = height = header->height;

		dataLength = header->dataLength;

		bytes = ((uint8_t *)data) + sizeof(PVRTexHeader);

		// Calculate the data size for each texture level and respect the minimum number of blocks
		while (dataOffset < dataLength)
		{
			if (formatFlags == kPVRTextureFlagTypePVRTC_4)
			{
				blockSize = 4 * 4; // Pixel by pixel block size for 4bpp
				widthBlocks = width / 4;
				heightBlocks = height / 4;
				bpp = 4;
			}
			else
			{
				blockSize = 8 * 4; // Pixel by pixel block size for 2bpp
				widthBlocks = width / 8;
				heightBlocks = height / 4;
				bpp = 2;
			}

			// Clamp to minimum number of blocks
			if (widthBlocks < 2)
				widthBlocks = 2;
			if (heightBlocks < 2)
				heightBlocks = 2;

			dataSize = widthBlocks * heightBlocks * ((blockSize * bpp) / 8);

			mipMapData[uiMipMapCount] = bytes + dataOffset;
			mipMapSize[uiMipMapCount] = dataSize;

			uiMipMapCount++;

			dataOffset += dataSize;

			width = MAX(width >> 1, 1);
			height = MAX(height >> 1, 1);
		}

	}

	width = _width;
	height = _height;

	if (uiMipMapCount > 0)
	{
		if (_name != 0)
			glDeleteTextures(1, &_name);

		glGenTextures(1, &_name);
		glBindTexture(GL_TEXTURE_2D, _name);
	}

	if (uiMipMapCount > 1) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
	
    //Here we are setting the parameter to repeat the texture instead of clamping the texture
    //to the edge of our shape.
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	for (int i = 0; i < uiMipMapCount; i++)
	{
	    //Generate the texture
        glCompressedTexImage2D(GL_TEXTURE_2D, i, _internalFormat, width, height, 0, mipMapSize[i], mipMapData[i]);
	
//		std::stringstream s;
//		s << "Error uploading compressed texture level " << i;
		slh::RenderDevice::errorCheck("Error uploading compressed texture level ");

		width = MAX(width >> 1, 1);
		height = MAX(height >> 1, 1);
	}

    muiTexture = _name;

#elif defined TEXTURE_DDS
	#define BITS_PER_BYTE 8
	#define NUM_BLOCKS ((nMipWidth+nBlockWidth-1)/nBlockWidth) * ((nMipHeight+nBlockHeight-1)/nBlockHeight)
	#define MIP_SIZE (nPixelsPerBlock * NUM_BLOCKS * nBPP) / BITS_PER_BYTE

    unsigned long nWidth = 0;
    unsigned long nHeight = 0;
    unsigned long nNumLevels = 0;
    unsigned long nInternalPixelFormat = 0;
    unsigned long nPixelType = 0;
    unsigned long nBorder = 0;
    void* pInitialData = NULL;
    unsigned long nBaseSize = 0;
    GLuint* pTexId= NULL;

    unsigned int uiFourCC = 0;

	uiFourCC = *((unsigned int*)(data + CR_FOURCC_OFFSET));
	nBaseSize = *((unsigned int*)(data + CR_LINEAR_SIZE_OFFSET));

	nNumLevels = *((unsigned int*)(data + CR_MIPMAP_COUNT_OFFSET));
	// Hack to get non-mipped files to load with our mip code below
	if (nNumLevels == 0) {
		nNumLevels = 1;
	}

	nWidth = *((unsigned int*)(data + CR_DDS_WIDTH_OFFSET));
	nHeight = *((unsigned int*)(data + CR_DDS_HEIGHT_OFFSET));

	nPixelType = GL_UNSIGNED_BYTE; // Used for uncompressed only, stand in value.
	nBorder = 0; // Seems to be always 0 for our purpose...

	pTexId = &muiTexture;

	unsigned int nBlockWidth = 0;
	unsigned int nBlockHeight = 0;
	unsigned int nPixelsPerBlock = 0;
	bool bCompressed = false;
	unsigned long nBPP = 0;

	switch(uiFourCC)
	{
#ifdef SLH_ANDROID
		case FOURCC_ATC:
			bCompressed = true;
			nBlockWidth = 4;
			nBlockHeight = 4;
			nBPP = BITS_PER_BYTE * nBaseSize / ( nWidth * nHeight );
			nInternalPixelFormat = GL_ATC_RGB_AMD;
			LOGD("ATC");
			break;

		case FOURCC_ATCA:
			bCompressed = true;
			nBlockWidth = 4;
			nBlockHeight = 4;
			nBPP = BITS_PER_BYTE * nBaseSize / ( nWidth * nHeight );
			nInternalPixelFormat = GL_ATC_RGBA_EXPLICIT_ALPHA_AMD;
			LOGD("ATCA");
			break;

		case FOURCC_ATCI:
			bCompressed = true;
			nBlockWidth = 4;
			nBlockHeight = 4;
			nBPP = BITS_PER_BYTE * nBaseSize / ( nWidth * nHeight );
			nInternalPixelFormat = GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD;
			LOGD("ATCI");
			break;
            
		case FOURCC_ETC0:
			bCompressed = true;
			nBlockWidth = 4;
			nBlockHeight = 4;
			nBPP = 4;
			nInternalPixelFormat = GL_ETC1_RGB8_OES;
			LOGD("ETC1");
			break;
#endif

#ifdef SLH_WIN32
		case FOURCC_DXT1:
			bCompressed = true;
			nBlockWidth = 4;
			nBlockHeight = 4;
			nBPP = 4;
			nInternalPixelFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
			LOGD("DXT1");
			break;

		case FOURCC_DXT3:
			bCompressed = true;
			nBlockWidth = 4;
			nBlockHeight = 4;
			nBPP = 8;
			nInternalPixelFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			LOGD("DXT3");
			break;

		case FOURCC_DXT5:
			bCompressed = true;
			nBlockWidth = 4;
			nBlockHeight = 4;
			nBPP = 8;
			nInternalPixelFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			LOGD("DXT5");
			break;
#endif

		default:
			LOGE("Unsupported DDS format!");
			return (GLuint)NULL;
	}

	nPixelsPerBlock = nBlockWidth * nBlockHeight;

	LOGD("BaseSize = %d, Width = %d, Height = %d, Mips = %d", (int)nBaseSize, (int)nWidth, (int)nHeight, (int)nNumLevels);
	pInitialData = (data + CR_DDS_PIXEL_DATA_OFFSET);

    if( (!pInitialData) || (!pTexId) ) {
        return false;
    }

    // Create a texture
    glGenTextures( 1, (GLuint*)pTexId );
    glBindTexture( GL_TEXTURE_2D, *pTexId );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    if( nNumLevels > 1 ) {
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    }

    unsigned long nSubPixelFormat = nInternalPixelFormat;

    // Create the mip chain
    {
    	unsigned long nMipWidth  = nWidth;
    	unsigned long nMipHeight = nHeight;
    	unsigned long nMipSize   = MIP_SIZE;

        // Populate the mip-levels
        for( unsigned long nLevel = 0; nLevel < nNumLevels; nLevel++ )
        {
            // Load the mip data
            if( bCompressed ) {
                glCompressedTexImage2D( GL_TEXTURE_2D, nLevel, nInternalPixelFormat,
                                        nMipWidth, nMipHeight, nBorder, nMipSize, pInitialData );
//                LOGD("Processed Compressed Level %d", (int)nLevel);
            } else {
                glTexImage2D( GL_TEXTURE_2D, nLevel, nInternalPixelFormat, nMipWidth, nMipHeight,
                              nBorder, nSubPixelFormat, nPixelType, pInitialData );
//                LOGD("Processed Uncompressed Level %d", (int)nLevel);
            }

            // Advance to the next level
            pInitialData = (void*)( (unsigned char*)pInitialData + nMipSize );

            if( nMipWidth > 1 ) {
            	nMipWidth  /= 2;
            }

            if( nMipHeight > 1 ) {
            	nMipHeight /= 2;
            }

            nMipSize = MIP_SIZE;
        }
    }

    slh::RenderDevice::errorCheck("We got no DDS!");

#endif   
    
    glBindTexture(GL_TEXTURE_2D, 0);
    tFile->freeBuffer();

    return muiTexture; //return whether it was successful 
}



void Texture::destroy() {
	glDeleteTextures(1, &muiTexture);
}

void Texture::use() {
	glBindTexture(GL_TEXTURE_2D, muiTexture);
}
}

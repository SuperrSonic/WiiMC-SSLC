/****************************************************************************
 *   "Gif-Lib" - Yet another gif library.
 *
 * Written by:  Gershon Elber            IBM PC Ver 1.1,    Aug. 1990
 ****************************************************************************
 * The kernel of the GIF Decoding process can be found here.
 ****************************************************************************
 * History:
 * 16 Jun 89 - Version 1.0 by Gershon Elber.
 *  3 Sep 90 - Version 1.1 by Gershon Elber (Support for Gif89, Unique names)
 *
 * Copyright (C) 2009 by r-win
 * Modified by Tantric, 2010
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you
 * must not claim that you wrote the original software. If you use
 * this software in a product, an acknowledgment in the product
 * documentation would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 ***************************************************************************/

#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <gccore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "mem2_manager.h"

//only texture in mem2, internal memory managed by gcc
#define gif_malloc malloc
#define gif_free free
#define gif_memalign memalign

//#define gif_malloc gui_mem2_malloc
//#define gif_free gui_mem2_free
//#define gif_memalign gui_mem2_memalign

#define GIF_TRANSPARENT 	0x01
#define GIF_NOT_TRANSPARENT	-1

#define GIF_ERROR   0
#define GIF_OK      1

#define VoidPtr void *

#define GIF_STAMP "GIFVER"          /* First chars in file - GIF stamp.  */
#define GIF_STAMP_LEN sizeof(GIF_STAMP) - 1
#define GIF_VERSION_POS 3           /* Version first character in stamp. */
#define GIF87_STAMP "GIF87a"        /* First chars in file - GIF stamp.  */
#define GIF89_STAMP "GIF89a"        /* First chars in file - GIF stamp.  */

#define D_GIF_ERR_OPEN_FAILED    	101    /* And DGif possible errors. */
#define D_GIF_ERR_READ_FAILED    	102
#define D_GIF_ERR_NOT_GIF_FILE   	103
#define D_GIF_ERR_NO_SCRN_DSCR   	104
#define D_GIF_ERR_NO_IMAG_DSCR   	105
#define D_GIF_ERR_NO_COLOR_MAP   	106
#define D_GIF_ERR_WRONG_RECORD   	107
#define D_GIF_ERR_DATA_TOO_BIG   	108
#define D_GIF_ERR_NOT_ENOUGH_MEM 	109
#define D_GIF_ERR_CLOSE_FAILED   	110
#define D_GIF_ERR_NOT_READABLE   	111
#define D_GIF_ERR_IMAGE_DEFECT   	112
#define D_GIF_ERR_EOF_TOO_SOON   	113

#define LZ_MAX_CODE					4095    /* Biggest code possible in 12 bits. */
#define LZ_BITS						12

#define FLUSH_OUTPUT       			4096    /* Impossible code, to signal flush. */
#define FIRST_CODE          		4097    /* Impossible code, to signal first. */
#define NO_SUCH_CODE        		4098    /* Impossible code, to signal empty. */

#define FILE_STATE_WRITE    		0x01
#define FILE_STATE_SCREEN   		0x02
#define FILE_STATE_IMAGE    		0x04
#define FILE_STATE_READ     		0x08

#define IS_READABLE(Private)    	(Private->FileState & FILE_STATE_READ)
#define IS_WRITEABLE(Private)   	(Private->FileState & FILE_STATE_WRITE)

#define GRAPHICS_EXT_FUNC_CODE		0xf9    /* graphics control */
#define HT_SIZE						8192	   /* 12bits = 4096 or twice as big! */

typedef int GifBooleanType;
typedef unsigned char GifPixelType;
typedef unsigned char *GifRowType;
typedef unsigned char GifByteType;
typedef unsigned int GifPrefixType;
typedef int GifWord;

typedef struct GifColorType {
    GifByteType Red, Green, Blue;
} GifColorType;

typedef struct ColorMapObject {
    int ColorCount;
    int BitsPerPixel;
    GifColorType *Colors;    /* on malloc(3) heap */
} ColorMapObject;

typedef struct GifImageDesc {
    GifWord Left, Top, Width, Height,   /* Current image dimensions. */
      Interlace;                    /* Sequential/Interlaced lines. */
    ColorMapObject *ColorMap;       /* The local color map */
} GifImageDesc;

typedef struct GifFileType {
    GifWord SWidth, SHeight,        /* Screen dimensions. */
      SColorResolution,         /* How many colors can we generate? */
      SBackGroundColor;         /* I hope you understand this one... */
    ColorMapObject *SColorMap;  /* NULL if not exists. */
    int ImageCount;             /* Number of current image */
    GifImageDesc Image;         /* Block describing current image */
    struct SavedImage *SavedImages; /* Use this to accumulate file state */
    VoidPtr UserData;           /* hook to attach user data (TVT) */
    VoidPtr Private;            /* Don't mess with this! */
} GifFileType;

typedef enum {
    UNDEFINED_RECORD_TYPE,
    SCREEN_DESC_RECORD_TYPE,
    IMAGE_DESC_RECORD_TYPE, /* Begin with ',' */
    EXTENSION_RECORD_TYPE,  /* Begin with '!' */
    TERMINATE_RECORD_TYPE   /* Begin with ';' */
} GifRecordType;

/* func type to read gif data from arbitrary sources (TVT) */
typedef int (*InputFunc) (GifFileType *, GifByteType *, int);

/* func type to write gif data ro arbitrary targets.
 * Returns count of bytes written. (MRB)
 */
typedef int (*OutputFunc) (GifFileType *, const GifByteType *, int);

/* This is the in-core version of an extension record */
typedef struct {
    int ByteCount;
    char *Bytes;    /* on malloc(3) heap */
    int Function;   /* Holds the type of the Extension block. */
} ExtensionBlock;

/* This holds an image header, its unpacked raster bits, and extensions */
typedef struct SavedImage {
    GifImageDesc ImageDesc;
    unsigned char *RasterBits;  /* on malloc(3) heap */
    int Function;   /* DEPRECATED: Use ExtensionBlocks[x].Function instead */
    int ExtensionBlockCount;
    ExtensionBlock *ExtensionBlocks;    /* on malloc(3) heap */
} SavedImage;

typedef struct GifHashTableType {
    u32 HTable[HT_SIZE];
} GifHashTableType;

typedef struct GifFilePrivateType {
    GifWord FileState, FileHandle,  /* Where all this data goes to! */
      BitsPerPixel,     /* Bits per pixel (Codes uses at least this + 1). */
      ClearCode,   /* The CLEAR LZ code. */
      EOFCode,     /* The EOF LZ code. */
      RunningCode, /* The next code algorithm can generate. */
      RunningBits, /* The number of bits required to represent RunningCode. */
      MaxCode1,    /* 1 bigger than max. possible code, in RunningBits bits. */
      LastCode,    /* The code before the current code. */
      CrntCode,    /* Current algorithm code. */
      StackPtr,    /* For character stack (see below). */
      CrntShiftState;    /* Number of bits in CrntShiftDWord. */
    unsigned long CrntShiftDWord;   /* For bytes decomposition into codes. */
    unsigned long PixelCount;   /* Number of pixels in image. */
    FILE *File;    /* File as stream. */
    InputFunc Read;     /* function to read gif input (TVT) */
    OutputFunc Write;   /* function to write gif output (MRB) */
    GifByteType Buf[256];   /* Compressed input is buffered here. */
    GifByteType Stack[LZ_MAX_CODE]; /* Decoded pixels are stacked here. */
    GifByteType Suffix[LZ_MAX_CODE + 1];    /* So we can trace the codes. */
    GifPrefixType Prefix[LZ_MAX_CODE + 1];
    GifHashTableType *HashTable;
} GifFilePrivateType;

typedef struct GifMemoryType {
	void *imgData;
	int imgSize;
	int currentPosition;
} GifMemoryType;

#define READ(_gif,_buf,_len)                                     \
  (((GifFilePrivateType*)_gif->Private)->Read ?                   \
    ((GifFilePrivateType*)_gif->Private)->Read(_gif,_buf,_len) : \
    fread(_buf,1,_len,((GifFilePrivateType*)_gif->Private)->File))

static int _GifError = 0;

static int BitSize(int n)
{
	register int i;

	for (i = 1; i <= 8; i++)
		if ((1 << i) >= n)
			break;
	return (i);
}

static ColorMapObject * MakeMapObject(int ColorCount, const GifColorType * ColorMap)
{
	ColorMapObject *Object;

	if (ColorCount != (1 << BitSize(ColorCount)))
	{
		return ((ColorMapObject *) NULL);
	}

	Object = (ColorMapObject *) gif_malloc(sizeof(ColorMapObject));
	if (Object == (ColorMapObject *) NULL)
	{
		return ((ColorMapObject *) NULL);
	}

	Object->Colors = (GifColorType *) calloc(ColorCount, sizeof(GifColorType));
	if (Object->Colors == (GifColorType *) NULL)
	{
		return ((ColorMapObject *) NULL);
	}

	Object->ColorCount = ColorCount;
	Object->BitsPerPixel = BitSize(ColorCount);

	if (ColorMap)
	{
		memcpy((char *) Object->Colors, (char *) ColorMap, ColorCount * sizeof(GifColorType));
	}

	return (Object);
}

static void FreeMapObject(ColorMapObject * Object)
{
	if (Object != NULL)
	{
		gif_free(Object->Colors);
		gif_free(Object);
	}
}

static void FreeExtension(SavedImage * Image)
{
	ExtensionBlock *ep;

	if ((Image == NULL) || (Image->ExtensionBlocks == NULL))
	{
		return;
	}
	for (ep = Image->ExtensionBlocks; ep < (Image->ExtensionBlocks + Image->ExtensionBlockCount); ep++)
		(void) gif_free((char *) ep->Bytes);
	gif_free((char *) Image->ExtensionBlocks);
	Image->ExtensionBlocks = NULL;
}

static void FreeSavedImages(GifFileType * GifFile)
{
	SavedImage *sp;

	if ((GifFile == NULL) || (GifFile->SavedImages == NULL))
	{
		return;
	}
	for (sp = GifFile->SavedImages; sp < GifFile->SavedImages + GifFile->ImageCount; sp++)
	{
		if (sp->ImageDesc.ColorMap)
		{
			FreeMapObject(sp->ImageDesc.ColorMap);
			sp->ImageDesc.ColorMap = NULL;
		}

		if (sp->RasterBits)
			gif_free((char *) sp->RasterBits);

		if (sp->ExtensionBlocks)
			FreeExtension(sp);
	}
	gif_free((char *) GifFile->SavedImages);
	GifFile->SavedImages = NULL;
}

static int DGifGetWord(GifFileType * GifFile, GifWord *Word)
{
	unsigned char c[2];

	if (READ(GifFile, c, 2) != 2)
	{
		_GifError = D_GIF_ERR_READ_FAILED;
		return GIF_ERROR;
	}

	*Word = (((unsigned int) c[1]) << 8) + c[0];
	return GIF_OK;
}

static int DGifGetScreenDesc(GifFileType * GifFile)
{
	int i, BitsPerPixel;
	GifByteType Buf[3];
	GifFilePrivateType *Private = (GifFilePrivateType *) GifFile->Private;

	if (!IS_READABLE(Private))
	{
		_GifError = D_GIF_ERR_NOT_READABLE;
		return GIF_ERROR;
	}

	if (DGifGetWord(GifFile, &GifFile->SWidth) == GIF_ERROR || DGifGetWord(GifFile, &GifFile->SHeight) == GIF_ERROR)
		return GIF_ERROR;

	if (READ(GifFile, Buf, 3) != 3)
	{
		_GifError = D_GIF_ERR_READ_FAILED;
		return GIF_ERROR;
	}
	GifFile->SColorResolution = (((Buf[0] & 0x70) + 1) >> 4) + 1;
	BitsPerPixel = (Buf[0] & 0x07) + 1;
	GifFile->SBackGroundColor = Buf[1];
	if (Buf[0] & 0x80)
	{

		GifFile->SColorMap = MakeMapObject(1 << BitsPerPixel, NULL);
		if (GifFile->SColorMap == NULL)
		{
			_GifError = D_GIF_ERR_NOT_ENOUGH_MEM;
			return GIF_ERROR;
		}

		for (i = 0; i < GifFile->SColorMap->ColorCount; i++)
		{
			if (READ(GifFile, Buf, 3) != 3)
			{
				FreeMapObject(GifFile->SColorMap);
				GifFile->SColorMap = NULL;
				_GifError = D_GIF_ERR_READ_FAILED;
				return GIF_ERROR;
			}
			GifFile->SColorMap->Colors[i].Red = Buf[0];
			GifFile->SColorMap->Colors[i].Green = Buf[1];
			GifFile->SColorMap->Colors[i].Blue = Buf[2];
		}
	}
	else
	{
		GifFile->SColorMap = NULL;
	}

	return GIF_OK;
}

static GifFileType *
DGifOpen(void *userData, InputFunc readFunc)
{
	unsigned char Buf[GIF_STAMP_LEN + 1];
	GifFileType *GifFile;
	GifFilePrivateType *Private;

	GifFile = (GifFileType *) gif_malloc(sizeof(GifFileType));
	if (GifFile == NULL)
	{
		_GifError = D_GIF_ERR_NOT_ENOUGH_MEM;
		return NULL;
	}

	memset(GifFile, '\0', sizeof(GifFileType));

	Private = (GifFilePrivateType *) gif_malloc(sizeof(GifFilePrivateType));
	if (!Private)
	{
		_GifError = D_GIF_ERR_NOT_ENOUGH_MEM;
		gif_free((char *) GifFile);
		return NULL;
	}

	GifFile->Private = (VoidPtr) Private;
	Private->FileHandle = 0;
	Private->File = 0;
	Private->FileState = FILE_STATE_READ;

	Private->Read = readFunc; /* TVT */
	GifFile->UserData = userData; /* TVT */

	if (READ(GifFile, Buf, GIF_STAMP_LEN) != GIF_STAMP_LEN)
	{
		_GifError = D_GIF_ERR_READ_FAILED;
		gif_free((char *) Private);
		gif_free((char *) GifFile);
		return NULL;
	}

	Buf[GIF_STAMP_LEN] = 0;
	if (strncmp(GIF_STAMP, (void *) Buf, GIF_VERSION_POS) != 0)
	{
		_GifError = D_GIF_ERR_NOT_GIF_FILE;
		gif_free((char *) Private);
		gif_free((char *) GifFile);
		return NULL;
	}

	if (DGifGetScreenDesc(GifFile) == GIF_ERROR)
	{
		gif_free((char *) Private);
		gif_free((char *) GifFile);
		return NULL;
	}

	_GifError = 0;

	return GifFile;
}

static int DGifGetRecordType(GifFileType * GifFile, GifRecordType * Type)
{
	GifByteType Buf;
	GifFilePrivateType *Private = (GifFilePrivateType *) GifFile->Private;

	if (!IS_READABLE(Private))
	{
		_GifError = D_GIF_ERR_NOT_READABLE;
		return GIF_ERROR;
	}

	if (READ(GifFile, &Buf, 1) != 1)
	{
		_GifError = D_GIF_ERR_READ_FAILED;
		return GIF_ERROR;
	}

	switch (Buf)
	{
		case ',':
			*Type = IMAGE_DESC_RECORD_TYPE;
			break;
		case '!':
			*Type = EXTENSION_RECORD_TYPE;
			break;
		case ';':
			*Type = TERMINATE_RECORD_TYPE;
			break;
		default:
			*Type = UNDEFINED_RECORD_TYPE;
			_GifError = D_GIF_ERR_WRONG_RECORD;
			return GIF_ERROR;
	}

	return GIF_OK;
}

static int DGifGetCodeNext(GifFileType * GifFile, GifByteType ** CodeBlock)
{
	GifByteType Buf;
	GifFilePrivateType *Private = (GifFilePrivateType *) GifFile->Private;

	if (READ(GifFile, &Buf, 1) != 1)
	{
		_GifError = D_GIF_ERR_READ_FAILED;
		return GIF_ERROR;
	}

	if (Buf > 0)
	{
		*CodeBlock = Private->Buf;
		(*CodeBlock)[0] = Buf;
		if (READ(GifFile, &((*CodeBlock)[1]), Buf) != Buf)
		{
			_GifError = D_GIF_ERR_READ_FAILED;
			return GIF_ERROR;
		}
	}
	else
	{
		*CodeBlock = NULL;
		Private->Buf[0] = 0;
		Private->PixelCount = 0;
	}

	return GIF_OK;
}

static int DGifGetPrefixChar(GifPrefixType *Prefix, int Code, int ClearCode)
{
	int i = 0;

	while (Code > ClearCode && i++ <= LZ_MAX_CODE)
	{
		if (Code > LZ_MAX_CODE)
		{
			return NO_SUCH_CODE;
		}
		Code = Prefix[Code];
	}
	return Code;
}

static int DGifBufferedInput(GifFileType * GifFile, GifByteType * Buf, GifByteType * NextByte)
{
	if (Buf[0] == 0)
	{
		if (READ(GifFile, Buf, 1) != 1)
		{
			_GifError = D_GIF_ERR_READ_FAILED;
			return GIF_ERROR;
		}

		if (Buf[0] == 0)
		{
			_GifError = D_GIF_ERR_IMAGE_DEFECT;
			return GIF_ERROR;
		}
		if (READ(GifFile, &Buf[1], Buf[0]) != Buf[0])
		{
			_GifError = D_GIF_ERR_READ_FAILED;
			return GIF_ERROR;
		}
		*NextByte = Buf[1];
		Buf[1] = 2;
		Buf[0]--;
	}
	else
	{
		*NextByte = Buf[Buf[1]++];
		Buf[0]--;
	}

	return GIF_OK;
}

static int DGifDecompressInput(GifFileType * GifFile, int *Code)
{
	GifFilePrivateType *Private = (GifFilePrivateType *) GifFile->Private;

	GifByteType NextByte;
	static unsigned short CodeMasks[] =
	{ 0x0000, 0x0001, 0x0003, 0x0007, 0x000f, 0x001f, 0x003f, 0x007f, 0x00ff, 0x01ff, 0x03ff, 0x07ff, 0x0fff };
	if (Private->RunningBits > LZ_BITS)
	{
		_GifError = D_GIF_ERR_IMAGE_DEFECT;
		return GIF_ERROR;
	}

	while (Private->CrntShiftState < Private->RunningBits)
	{
		if (DGifBufferedInput(GifFile, Private->Buf, &NextByte) == GIF_ERROR)
		{
			return GIF_ERROR;
		}
		Private->CrntShiftDWord |= ((unsigned long) NextByte) << Private->CrntShiftState;
		Private->CrntShiftState += 8;
	}
	*Code = Private->CrntShiftDWord & CodeMasks[Private->RunningBits];

	Private->CrntShiftDWord >>= Private->RunningBits;
	Private->CrntShiftState -= Private->RunningBits;

	if (Private->RunningCode < LZ_MAX_CODE + 2 && ++Private->RunningCode > Private->MaxCode1 && Private->RunningBits < LZ_BITS)
	{
		Private->MaxCode1 <<= 1;
		Private->RunningBits++;
	}
	return GIF_OK;
}

static int DGifSetupDecompress(GifFileType * GifFile)
{
	int i, BitsPerPixel;
	GifByteType CodeSize;
	GifPrefixType *Prefix;
	GifFilePrivateType *Private = (GifFilePrivateType *) GifFile->Private;

	READ(GifFile, &CodeSize, 1);
	BitsPerPixel = CodeSize;

	Private->Buf[0] = 0;
	Private->BitsPerPixel = BitsPerPixel;
	Private->ClearCode = (1 << BitsPerPixel);
	Private->EOFCode = Private->ClearCode + 1;
	Private->RunningCode = Private->EOFCode + 1;
	Private->RunningBits = BitsPerPixel + 1;
	Private->MaxCode1 = 1 << Private->RunningBits;
	Private->StackPtr = 0;
	Private->LastCode = NO_SUCH_CODE;
	Private->CrntShiftState = 0;
	Private->CrntShiftDWord = 0;

	Prefix = Private->Prefix;
	for (i = 0; i <= LZ_MAX_CODE; i++)
		Prefix[i] = NO_SUCH_CODE;

	return GIF_OK;
}

static int DGifDecompressLine(GifFileType * GifFile, GifPixelType * Line, int LineLen)
{
	int i = 0;
	int j, CrntCode, EOFCode, ClearCode, CrntPrefix, LastCode, StackPtr;
	GifByteType *Stack, *Suffix;
	GifPrefixType *Prefix;
	GifFilePrivateType *Private = (GifFilePrivateType *) GifFile->Private;

	StackPtr = Private->StackPtr;
	Prefix = Private->Prefix;
	Suffix = Private->Suffix;
	Stack = Private->Stack;
	EOFCode = Private->EOFCode;
	ClearCode = Private->ClearCode;
	LastCode = Private->LastCode;

	if (StackPtr > LZ_MAX_CODE)
	{
		return GIF_ERROR;
	}

	if (StackPtr != 0)
	{
		while (StackPtr != 0 && i < LineLen)
			Line[i++] = Stack[--StackPtr];
	}

	while (i < LineLen)
	{
		if (DGifDecompressInput(GifFile, &CrntCode) == GIF_ERROR)
			return GIF_ERROR;

		if (CrntCode == EOFCode)
		{
			if (i != LineLen - 1 || Private->PixelCount != 0)
			{
				_GifError = D_GIF_ERR_EOF_TOO_SOON;
				return GIF_ERROR;
			}
			i++;
		}
		else if (CrntCode == ClearCode)
		{
			for (j = 0; j <= LZ_MAX_CODE; j++)
				Prefix[j] = NO_SUCH_CODE;
			Private->RunningCode = Private->EOFCode + 1;
			Private->RunningBits = Private->BitsPerPixel + 1;
			Private->MaxCode1 = 1 << Private->RunningBits;
			LastCode = Private->LastCode = NO_SUCH_CODE;
		}
		else
		{
			if (CrntCode < ClearCode)
			{
				Line[i++] = CrntCode;
			}
			else
			{
				if (Prefix[CrntCode] == NO_SUCH_CODE)
				{
					if (CrntCode == Private->RunningCode - 2)
					{
						CrntPrefix = LastCode;
						Suffix[Private->RunningCode - 2] = Stack[StackPtr++] = DGifGetPrefixChar(Prefix, LastCode, ClearCode);
					}
					else
					{
						_GifError = D_GIF_ERR_IMAGE_DEFECT;
						return GIF_ERROR;
					}
				}
				else
					CrntPrefix = CrntCode;

				j = 0;
				while (j++ <= LZ_MAX_CODE && CrntPrefix > ClearCode && CrntPrefix <= LZ_MAX_CODE)
				{
					Stack[StackPtr++] = Suffix[CrntPrefix];
					CrntPrefix = Prefix[CrntPrefix];
				}
				if (j >= LZ_MAX_CODE || CrntPrefix > LZ_MAX_CODE)
				{
					_GifError = D_GIF_ERR_IMAGE_DEFECT;
					return GIF_ERROR;
				}
				Stack[StackPtr++] = CrntPrefix;

				while (StackPtr != 0 && i < LineLen)
					Line[i++] = Stack[--StackPtr];
			}
			if (LastCode != NO_SUCH_CODE)
			{
				Prefix[Private->RunningCode - 2] = LastCode;

				if (CrntCode == Private->RunningCode - 2)
				{
					Suffix[Private->RunningCode - 2] = DGifGetPrefixChar(Prefix, LastCode, ClearCode);
				}
				else
				{
					Suffix[Private->RunningCode - 2] = DGifGetPrefixChar(Prefix, CrntCode, ClearCode);
				}
			}
			LastCode = CrntCode;
		}
	}
	Private->LastCode = LastCode;
	Private->StackPtr = StackPtr;
	return GIF_OK;
}

static int DGifGetImageDesc(GifFileType * GifFile)
{
	int i, BitsPerPixel;
	GifByteType Buf[3];
	GifFilePrivateType *Private = (GifFilePrivateType *) GifFile->Private;
	SavedImage *sp;

	if (!IS_READABLE(Private))
	{
		_GifError = D_GIF_ERR_NOT_READABLE;
		return GIF_ERROR;
	}

	if (DGifGetWord(GifFile, &GifFile->Image.Left) == GIF_ERROR || DGifGetWord(GifFile, &GifFile->Image.Top) == GIF_ERROR || DGifGetWord(GifFile, &GifFile->Image.Width) == GIF_ERROR
			|| DGifGetWord(GifFile, &GifFile->Image.Height) == GIF_ERROR)
		return GIF_ERROR;
	if (READ(GifFile, Buf, 1) != 1)
	{
		_GifError = D_GIF_ERR_READ_FAILED;
		return GIF_ERROR;
	}
	BitsPerPixel = (Buf[0] & 0x07) + 1;
	GifFile->Image.Interlace = (Buf[0] & 0x40);
	if (Buf[0] & 0x80)
	{
		if (GifFile->Image.ColorMap && GifFile->SavedImages == NULL)
			FreeMapObject(GifFile->Image.ColorMap);

		GifFile->Image.ColorMap = MakeMapObject(1 << BitsPerPixel, NULL);
		if (GifFile->Image.ColorMap == NULL)
		{
			_GifError = D_GIF_ERR_NOT_ENOUGH_MEM;
			return GIF_ERROR;
		}

		for (i = 0; i < GifFile->Image.ColorMap->ColorCount; i++)
		{
			if (READ(GifFile, Buf, 3) != 3)
			{
				FreeMapObject(GifFile->Image.ColorMap);
				_GifError = D_GIF_ERR_READ_FAILED;
				GifFile->Image.ColorMap = NULL;
				return GIF_ERROR;
			}
			GifFile->Image.ColorMap->Colors[i].Red = Buf[0];
			GifFile->Image.ColorMap->Colors[i].Green = Buf[1];
			GifFile->Image.ColorMap->Colors[i].Blue = Buf[2];
		}
	}
	else if (GifFile->Image.ColorMap)
	{
		FreeMapObject(GifFile->Image.ColorMap);
		GifFile->Image.ColorMap = NULL;
	}

	if (GifFile->SavedImages)
	{
		if ((GifFile->SavedImages = (SavedImage *) realloc(GifFile->SavedImages, sizeof(SavedImage) * (GifFile->ImageCount + 1))) == NULL)
		{
			_GifError = D_GIF_ERR_NOT_ENOUGH_MEM;
			return GIF_ERROR;
		}
	}
	else
	{
		if ((GifFile->SavedImages = (SavedImage *) gif_malloc(sizeof(SavedImage))) == NULL)
		{
			_GifError = D_GIF_ERR_NOT_ENOUGH_MEM;
			return GIF_ERROR;
		}
	}

	sp = &GifFile->SavedImages[GifFile->ImageCount];
	memcpy(&sp->ImageDesc, &GifFile->Image, sizeof(GifImageDesc));
	if (GifFile->Image.ColorMap != NULL)
	{
		sp->ImageDesc.ColorMap = MakeMapObject(GifFile->Image.ColorMap->ColorCount, GifFile->Image.ColorMap->Colors);
		if (sp->ImageDesc.ColorMap == NULL)
		{
			_GifError = D_GIF_ERR_NOT_ENOUGH_MEM;
			return GIF_ERROR;
		}
	}
	sp->RasterBits = (unsigned char *) NULL;
	sp->ExtensionBlockCount = 0;
	sp->ExtensionBlocks = (ExtensionBlock *) NULL;

	GifFile->ImageCount++;

	Private->PixelCount = (long) GifFile->Image.Width * (long) GifFile->Image.Height;

	DGifSetupDecompress(GifFile);

	return GIF_OK;
}

static int DGifGetLine(GifFileType * GifFile, GifPixelType * Line, int LineLen)
{
	GifByteType *Dummy;
	GifFilePrivateType *Private = (GifFilePrivateType *) GifFile->Private;

	if (!IS_READABLE(Private))
	{
		_GifError = D_GIF_ERR_NOT_READABLE;
		return GIF_ERROR;
	}

	if (!LineLen)
		LineLen = GifFile->Image.Width;

	if ((Private->PixelCount -= LineLen) > 0xffff0000UL)
	{
		_GifError = D_GIF_ERR_DATA_TOO_BIG;
		return GIF_ERROR;
	}

	if (DGifDecompressLine(GifFile, Line, LineLen) == GIF_OK)
	{
		if (Private->PixelCount == 0)
		{
			do
				if (DGifGetCodeNext(GifFile, &Dummy) == GIF_ERROR)
					return GIF_ERROR; while (Dummy != NULL);
		}
		return GIF_OK;
	}
	else
		return GIF_ERROR;
}

static int DGifGetExtensionNext(GifFileType * GifFile, GifByteType ** Extension)
{
	GifByteType Buf;
	GifFilePrivateType *Private = (GifFilePrivateType *) GifFile->Private;

	if (READ(GifFile, &Buf, 1) != 1)
	{
		_GifError = D_GIF_ERR_READ_FAILED;
		return GIF_ERROR;
	}
	if (Buf > 0)
	{
		*Extension = Private->Buf;
		(*Extension)[0] = Buf;
		if (READ(GifFile, &((*Extension)[1]), Buf) != Buf)
		{
			_GifError = D_GIF_ERR_READ_FAILED;
			return GIF_ERROR;
		}
	}
	else
		*Extension = NULL;

	return GIF_OK;
}

static int DGifGetExtension(GifFileType * GifFile, int *ExtCode, GifByteType ** Extension)
{
	GifByteType Buf;
	GifFilePrivateType *Private = (GifFilePrivateType *) GifFile->Private;

	if (!IS_READABLE(Private))
	{
		_GifError = D_GIF_ERR_NOT_READABLE;
		return GIF_ERROR;
	}

	if (READ(GifFile, &Buf, 1) != 1)
	{
		_GifError = D_GIF_ERR_READ_FAILED;
		return GIF_ERROR;
	}
	*ExtCode = Buf;

	return DGifGetExtensionNext(GifFile, Extension);
}

static int DGifCloseFile(GifFileType * GifFile)
{
	GifFilePrivateType *Private;
	FILE *File;

	if (GifFile == NULL)
		return GIF_ERROR;

	Private = (GifFilePrivateType *) GifFile->Private;

	if (!IS_READABLE(Private))
	{
		_GifError = D_GIF_ERR_NOT_READABLE;
		return GIF_ERROR;
	}

	File = Private->File;

	if (GifFile->Image.ColorMap)
	{
		FreeMapObject(GifFile->Image.ColorMap);
		GifFile->Image.ColorMap = NULL;
	}

	if (GifFile->SColorMap)
	{
		FreeMapObject(GifFile->SColorMap);
		GifFile->SColorMap = NULL;
	}

	if (Private)
	{
		gif_free((char *) Private);
		Private = NULL;
	}

	if (GifFile->SavedImages)
	{
		FreeSavedImages(GifFile);
		GifFile->SavedImages = NULL;
	}

	gif_free(GifFile);

	if (File && (fclose(File) != 0))
	{
		_GifError = D_GIF_ERR_CLOSE_FAILED;
		return GIF_ERROR;
	}
	return GIF_OK;
}

static int DGifOpenFromMemoryCallback(GifFileType *gifFile, GifByteType *gifByteType, int length)
{
	GifMemoryType *gifMemoryType = (GifMemoryType *) gifFile->UserData;

	if (length + gifMemoryType->currentPosition > gifMemoryType->imgSize)
	{
		return D_GIF_ERR_READ_FAILED;
	}

	void *start = (gifMemoryType->imgData + gifMemoryType->currentPosition);
	memcpy(gifByteType, start, length);
	gifMemoryType->currentPosition += length;

	return length;
}

static int DGifCloseMem(GifFileType *gifFile)
{
	if (gifFile->UserData != NULL)
	{
		GifMemoryType *gifMemoryType = (GifMemoryType *) gifFile->UserData;
		gif_free(gifMemoryType);
	}
	return DGifCloseFile(gifFile);
}

static GifFileType *DGifOpenMem(const unsigned char *img, int imgSize)
{
	GifMemoryType *gifMemoryType = (GifMemoryType *) gif_malloc(sizeof(GifMemoryType));
	memset((void *) gifMemoryType, 0, sizeof(GifMemoryType));
	gifMemoryType->imgData = (void *) img;
	gifMemoryType->imgSize = imgSize;

	return DGifOpen(gifMemoryType, DGifOpenFromMemoryCallback);
}

static u8* DGifDecodeTo4x4RGB8(GifFileType *gifFile, GifRowType *rowType, short transparentColor, u8 *dstPtr)
{
	unsigned int vert, hor, col, row;

	ColorMapObject *colorMap = (gifFile->Image.ColorMap ? gifFile->Image.ColorMap : gifFile->SColorMap);

	if (colorMap == NULL)
		return NULL;

	int newWidth = gifFile->Image.Width;
	if (newWidth % 4)
		newWidth += (4 - newWidth % 4);
	int newHeight = gifFile->Image.Height;
	if (newHeight % 4)
		newHeight += (4 - newHeight % 4);

	int len = (newWidth * newHeight) << 2;
	if (len % 32)
		len += (32 - len % 32);

	u8 * dst;

	if(dstPtr)
		dst = dstPtr; // use existing allocation
	else
		dst = mem2_memalign (32, len, MEM2_GUI);

	if (!dst)
		return NULL;

	unsigned char *p = (unsigned char*) dst;

	for (vert = 0; vert < newHeight; vert += 4)
	{
		for (hor = 0; hor < newWidth; hor += 4)
		{
			for (row = 0; row < 4; row++)
			{
				GifRowType gifRow = rowType[vert + row];
				unsigned char *gb = (unsigned char *) (p + 32);
				for (col = 0; col < 4; col++)
				{
					if (vert + row >= gifFile->Image.Height || // Transparent cell
							hor + col >= gifFile->Image.Width)
					{
						*p++ = 0;
						*p++ = 255;
						*gb++ = 255;
						*gb++ = 255;
					}
					else
					{
						int color = *(char*) (gifRow + hor + col);
						GifColorType *colorMapEntry = &colorMap->Colors[color];
						*p++ = color == transparentColor ? 0 : 255;
						*p++ = colorMapEntry->Red;
						*gb++ = colorMapEntry->Green;
						*gb++ = colorMapEntry->Blue;
					}
				}
			}
			p += 32;
		}
	}
	DCFlushRange(dst, len);
	return dst;
}

static GifRowType *DGifDecompress(GifFileType *gifFile, short *transparentColor)
{
	static int InterlacedOffset[] =
	{ 0, 4, 2, 1 }, InterlacedJumps[] =
	{ 8, 8, 4, 2 };

	int i, j, row, col, width, height = 0;
	GifRowType *ScreenBuffer = NULL;

	int rowSize = gifFile->SWidth * sizeof(GifPixelType);

	ScreenBuffer = (GifRowType *) gif_malloc(gifFile->SHeight * sizeof(GifRowType *));
	if (ScreenBuffer == NULL)
	{
		return NULL;
	}

	int Size = gifFile->SWidth * sizeof(GifPixelType);
	ScreenBuffer[0] = (GifRowType) gif_malloc(Size);
	if (ScreenBuffer[0] == NULL)
	{
		goto cleanup;
	}

	for (i = 0; i < gifFile->SWidth; i++)
	{
		ScreenBuffer[0][i] = gifFile->SBackGroundColor;
	}
	for (i = 1; i < gifFile->SHeight; i++)
	{
		ScreenBuffer[i] = (GifRowType) gif_malloc(Size);
		if (ScreenBuffer[i] == NULL)
		{
			goto cleanup;
		}
		memcpy(ScreenBuffer[i], ScreenBuffer[0], Size);
	}

	*transparentColor = GIF_NOT_TRANSPARENT;

	GifRecordType recordType;
	do
	{
		if (DGifGetRecordType(gifFile, &recordType) == GIF_ERROR)
		{
			goto cleanup;
		}
		switch (recordType)
		{
			case IMAGE_DESC_RECORD_TYPE:
			{
				if (DGifGetImageDesc(gifFile) == GIF_ERROR)
				{
					goto cleanup;
				}

				row = gifFile->Image.Top;
				col = gifFile->Image.Left;
				width = gifFile->Image.Width;
				height = gifFile->Image.Height;

				GifPixelType *gifPixelType;
				int location = 0;
				if (gifFile->Image.Interlace)
				{
					for (i = 0; i < 4; i++)
					{
						for (j = row + InterlacedOffset[i]; j < row + height; j += InterlacedJumps[i])
						{
							gifPixelType = &ScreenBuffer[j][col];
							if (DGifGetLine(gifFile, gifPixelType, width) == GIF_ERROR)
							{
								goto cleanup;
							}
							location += rowSize;
						}
					}
				}
				else
				{
					for (i = 0; i < height; i++)
					{
						if (DGifGetLine(gifFile, &ScreenBuffer[row++][col], width) == GIF_ERROR)
						{
							goto cleanup;
						}
					}
				}
				break;
			}
			case EXTENSION_RECORD_TYPE:
			{
				GifByteType *extension;
				int extCode;
				if (DGifGetExtension(gifFile, &extCode, &extension) == GIF_ERROR)
				{
					goto cleanup;
				}
				switch (extCode)
				{
					case GRAPHICS_EXT_FUNC_CODE:
					{
						int flag = extension[1];
						*transparentColor = (flag & GIF_TRANSPARENT) ? extension[4] : GIF_NOT_TRANSPARENT;
						break;
					}
				}
				while (extension != NULL)
				{
					if (DGifGetExtensionNext(gifFile, &extension) == GIF_ERROR)
					{
						goto cleanup;
					}
				}
				break;
			}
			case TERMINATE_RECORD_TYPE:
			case UNDEFINED_RECORD_TYPE:
			default:
				break;
		}
	} while (recordType != TERMINATE_RECORD_TYPE);

	goto finish;

	cleanup: if (ScreenBuffer != NULL)
	{
		gif_free(ScreenBuffer);
		ScreenBuffer = NULL;
	}
	finish: return ScreenBuffer;
}

u8 * DecodeGIF(const u8 *src, u32 srclen, int *width, int *height, u8 *dstPtr)
{
	GifFileType *gifFile = DGifOpenMem(src, srclen);

	if (gifFile == NULL)
		return NULL;

	short transparentColor;

	GifRowType *rows = (GifRowType *) DGifDecompress(gifFile, &transparentColor);

	if (rows == NULL)
		return NULL;

	u8 *dst = DGifDecodeTo4x4RGB8(gifFile, rows, transparentColor, dstPtr);

	if (dst)
	{
		*width = gifFile->Image.Width;
		*height = gifFile->Image.Height;
	}

	DGifCloseMem(gifFile);
	gif_free(rows);
	return dst;
}

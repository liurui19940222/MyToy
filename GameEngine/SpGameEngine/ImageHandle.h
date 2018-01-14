#ifndef _IMAGE_HANDLE_H_
#define _IMAGE_HANDLE_H_

#include"SpCommon\ApiDefine.h"

extern "C" bool ENGINE_API ChangeImageSize(const char* srcPath, const char* dstPath, int width, int height);
extern "C" bool ENGINE_API ChangeImageSizeWithPercentage(const char* srcPath, const char* dstPath, float percentage);

#endif

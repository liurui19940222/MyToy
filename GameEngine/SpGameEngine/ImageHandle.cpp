#include"ImageHandle.h"
#include"..\SpAssetLoader\ImageLoader.h"

bool ChangeImageSize(const char* srcPath, const char* dstPath, int width, int height)
{
	bool result = false;
	ImageLoader* image = new ImageLoader(srcPath);
	if (image)
	{
		result = image->ChangeSize(width, height) && image->Save(dstPath, FREE_IMAGE_FORMAT::FIF_PNG);
		image->ReleaseSource();
		delete(image);
	}
	return result;
}

bool ChangeImageSizeWithPercentage(const char* srcPath, const char* dstPath, float percentage)
{
	bool result = false;
	ImageLoader* image = new ImageLoader(srcPath);
	if (image)
	{
		result = image->ChangeSize(image->GetWidth() * percentage, image->GetHeight() * percentage) && image->Save(dstPath, FREE_IMAGE_FORMAT::FIF_PNG);
		image->ReleaseSource();
		delete(image);
	}
	return result;
}
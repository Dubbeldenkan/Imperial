#include "Image.h"

namespace GraphicsNS
{
	std::map<std::string, Image> Image::_imageMap;

	Image* Image::CreateImage(std::string stringKey)
	{
		Image* image = new Image(stringKey);
		return image;
	}

	Image::Image()
	{}

	Image::Image(std::string filePath)
	{
		_imageMap[filePath] = *this;
	}

	int Image::GetXSize() const
	{
		if (_xSize == 0)
		{
			D3DSURFACE_DESC surfaceDesc;
			_texture->GetLevelDesc(0, &surfaceDesc);
			return surfaceDesc.Width;
		}
		return _xSize;
	}

	int Image::GetYSize() const
	{
		if (_ySize == 0)
		{
			D3DSURFACE_DESC surfaceDesc;
			_texture->GetLevelDesc(0, &surfaceDesc);
			return surfaceDesc.Height;
		}
		return _ySize;
	}

	void Image::SetXSize(int size)
	{
		_xSize = size;
	}

	void Image::SetYSize(int size)
	{
		_ySize = size;
	}

	LPDIRECT3DTEXTURE9* Image::GetTexture()
	{
		return &_texture;
	}

	bool Image::ImageExist(const std::string key)
	{
		return !(_imageMap.find(key) == _imageMap.end());
	}

	Image* Image::GetImage(const std::string key)
	{
		return &_imageMap[key];
	}
}
#ifndef IMAGE_H
#define IMAGE_H

#include <d3dx9.h>
#include <map>

namespace GraphicsNS
{

	class Image
	{
		int _xSize = 0;
		int _ySize = 0;
		LPDIRECT3DTEXTURE9 _texture;

	public:
		Image();
		Image(std::string);

		static Image* CreateImage(std::string);

		int GetXSize();
		int GetYSize();

		void SetXSize(int);
		void SetYSize(int);

		LPDIRECT3DTEXTURE9* GetTexture();

		static bool ImageExist(std::string);
		static Image* GetImage(std::string);

	private:
		static std::map<std::string, Image> _imageMap;
	};
}

#endif // !IMAGE_H

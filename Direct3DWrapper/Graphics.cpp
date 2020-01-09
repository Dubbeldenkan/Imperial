#include "Graphics.h"

namespace GraphicsNS
{
	const static int FONT_SIZE_SMALL = 8;
	const static int FONT_SIZE_BIG = 15;

	const static D3DCOLOR D3DWHITE = D3DCOLOR_XRGB(255, 255, 255);
	const static D3DCOLOR D3DBLACK = D3DCOLOR_XRGB(0, 0, 0);
	const static D3DCOLOR D3DRED = D3DCOLOR_XRGB(255, 0, 0);
	const static D3DCOLOR D3DBLUE = D3DCOLOR_XRGB(0, 0, 255);
	const static D3DCOLOR D3DGREEN = D3DCOLOR_XRGB(0, 255, 0);
	const static D3DCOLOR D3DYELLOW = D3DCOLOR_XRGB(255, 255, 0);
	const static D3DCOLOR D3DPURPLE = D3DCOLOR_XRGB(255, 0, 255);

	DrawStruct::DrawStruct()
	{
		D3DXMatrixScaling(&transformMatrix, scale, scale, 0.0f);
	}

	Graphics::Graphics(HWND hWnd)
	{
		char tempPath[MAX_PATH];
		GetModuleFileName(NULL, tempPath, MAX_PATH);
		std::string str = std::string(tempPath);
		str.erase(str.rfind('\\'));
		str.erase(str.rfind('\\'));
		imagePath = str + "\\Bilder\\";

		_d3d = Direct3DCreate9(D3D_SDK_VERSION);
		if (_d3d == 0)
		{
			OutputDebugString("Unable to setup Direct3DCreate9\n\n");
		}
		D3DPRESENT_PARAMETERS presentParameters = { 0 };
		presentParameters.Windowed = true;
		presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
		presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;

		_d3dDevice = 0;
		HRESULT hr = _d3d->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&presentParameters,
			&_d3dDevice);

		if (FAILED(hr))
		{
			OutputDebugString("Unable to setup d3dDevice\n\n");
		}

		hr = D3DXCreateSprite(_d3dDevice, &_sprite);
		if (FAILED(hr))
		{
			OutputDebugString("Unable to setup sprite\n\n");
		}

		//Makes it possible to print text
		_hdc = GetDC(0);
		int logicalHeight = -MulDiv(FONT_SIZE_SMALL, GetDeviceCaps(_hdc, LOGPIXELSY), 72);
		hr = D3DXCreateFont(
			_d3dDevice,
			logicalHeight,
			0,
			0,
			1,
			0,
			ANSI_CHARSET,
			OUT_TT_ONLY_PRECIS,
			0,
			0,
			"Helvetica",
			&_font8);

		if (FAILED(hr))
		{
			OutputDebugString("Unable to setup font8\n\n");
		}

		logicalHeight = -MulDiv(FONT_SIZE_BIG, GetDeviceCaps(_hdc, LOGPIXELSY), 72);
		hr = D3DXCreateFont(
			_d3dDevice,
			logicalHeight,
			0,
			0,
			1,
			0,
			ANSI_CHARSET,
			OUT_TT_ONLY_PRECIS,
			0,
			0,
			"Helvetica",
			&_font15);

		if (FAILED(hr))
		{
			OutputDebugString("Unable to setup font8\n\n");
		}

		//load pixel Image
		whitePixel = LoadImageFromFile("Pixel/White.png");
	}

	Graphics::~Graphics()
	{
		if (_d3dDevice)
		{
			_d3dDevice->Release();
			_d3dDevice = 0;
		}
		if (_d3d)
		{
			_d3d->Release();
			_d3d = 0;
		}
		if (_sprite)
		{
			_sprite->Release();
			_sprite = 0;
		}
		for (int i = 0; i < (int)imageVector.size(); i++)
		{
			delete imageVector[i];
		}
	}

	void Graphics::Flip(bool waitRetrace)
	{
		_d3dDevice->Present(0, 0, 0, 0);
	}

	void Graphics::Clear(DWORD color)
	{
		int colorB = color % 256; color /= 256;
		int colorG = color % 256; color /= 256;
		int colorR = color % 256; color /= 256;
		HRESULT hr = _d3dDevice->Clear(
			0, 0,
			D3DCLEAR_TARGET,
			D3DCOLOR_XRGB(colorB, colorG, colorR),
			0, 0);
		if (FAILED(hr))
		{
			OutputDebugString("Unable to run clear for d3dDevice in function Flip\n\n");
		}
	}

	Image* Graphics::LoadImageFromFile(std::string fileName)
	{
		return LoadImageFromFile(fileName, 0, 0, 0, 0);
	}

	Image* Graphics::LoadImageFromFile(std::string fileName, int imageXSize, int imageYSize)
	{
		Image* image = LoadImageFromFile(fileName, imageXSize, imageYSize, 0, 0);
		image->SetXSize(imageXSize);
		image->SetYSize(imageYSize);
		return image;
	}

	Image* Graphics::LoadImageFromFile(std::string fileName, int imageXSize, int imageYSize,
		int partImageSizeX, int partImageSizeY)
	{
		//TODO gör så att man kan använda samma image för att rita ut flera sprites
		/*if (Image::ImageExist(fileName))
		{
			char result[100];
			strcpy_s(result, "Image ");
			strcat_s(result, fileName.c_str());
			strcat_s(result, " was already loaded\n\n");
			OutputDebugString(result);

			Image* image = Image::GetImage(fileName);
			imageVector.push_back(image);
			return image;
		}
		else
		{*/
			Image* image = Image::CreateImage(fileName);
			std::string path = imagePath + fileName;
			HRESULT hr = D3DXCreateTextureFromFileEx(
				_d3dDevice,
				path.c_str(),
				imageXSize, imageYSize,
				1,
				0,
				D3DFMT_UNKNOWN,
				D3DPOOL_MANAGED,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				D3DCOLOR_XRGB(0, 0, 0),
				0, 0,
				image->GetTexture());
			if (FAILED(hr))
			{
				std::string outputMsg = "\n\nCannot load image: " + path;
				outputMsg = outputMsg + "\n\n";
				OutputDebugString(outputMsg.c_str());
			}
			else
			{
				imageVector.push_back(image);
				char result[100];
				strcpy_s(result, "Image ");
				strcat_s(result, fileName.c_str());
				strcat_s(result, " was loaded\n\n");
				OutputDebugString(result);
			}
			image->SetXSize(partImageSizeX);
			image->SetYSize(partImageSizeY);
			return image;
		/*}
		return NULL;*/
	}

	void Graphics::StartDrawing()
	{
		_d3dDevice->BeginScene();
	}

	void Graphics::StopDrawing()
	{
		_d3dDevice->EndScene();
	}

	void Graphics::Draw(DrawStruct dS, Image* image) const
	{
		//TODO lägg in try catch 
		_sprite->Begin(D3DXSPRITE_ALPHABLEND);
		_sprite->SetTransform(&(dS.transformMatrix));
		_sprite->Draw(
			*(image->GetTexture()),
			dS.rect,
			dS.tilePosition,
			dS.position,
			dS.color);
		_sprite->End();
	}

	void Graphics::Draw(Image* image, int x, int y, float scale) const
	{
		DrawStruct dS = DrawStruct();
		dS.scale = scale;
		dS.position = &D3DXVECTOR3(x/scale, y/scale, 0);
		D3DXMatrixScaling(&dS.transformMatrix, scale, scale, 0.0f);
		Draw(dS, image);
	}

	void Graphics::DrawWithColor(Image* image, int x, int y, Color color) const
	{
		DrawStruct dS = DrawStruct();
		dS.color = ConvertToD3DCOLOR(color);
		dS.position = &D3DXVECTOR3(x / dS.scale, y / dS.scale, 0);
		Draw(dS, image);
	}

	void Graphics::DrawWithColor(Image* image, int x, int y, Color color, float scale) const
	{
		DrawStruct dS = DrawStruct();
		dS.color = ConvertToD3DCOLOR(color);
		dS.scale = scale;
		dS.position = &D3DXVECTOR3(x/scale, y/scale, 0);
		D3DXMatrixScaling(&dS.transformMatrix, scale, scale, 0.0f);
		Draw(dS, image);
	}

	void Graphics::DrawTile(Image* image, int x, int y, int tileIndex, int orientation) const
	{
		DrawStruct dS = DrawStruct();
		RECT* rect = new RECT();
		rect->top = 0;
		rect->bottom = image->GetYSize();
		rect->left = image->GetXSize()*tileIndex;
		rect->right = image->GetXSize()*(tileIndex + 1);
		dS.rect = rect;

		D3DXVECTOR2 center((float)x + image->GetXSize() / 2, (float)y + image->GetYSize() / 2);
		D3DXMATRIX transformMatrix;
		D3DXMatrixTransformation2D(&transformMatrix, NULL, NULL, NULL, &center, ((float)orientation) / 100.0f, NULL);
		dS.transformMatrix = transformMatrix;

		dS.position = &D3DXVECTOR3(static_cast<float>(x), static_cast<float>(y), 0.0);

		Draw(dS, image);
	}

	//if mirros is true the image should be mirrored over the y-axis
	void Graphics::DrawAnimation(Image* image, int xPos, int yPos, int imageIndex, bool mirror) const
	{
		DrawStruct dS = DrawStruct();
		D3DXVECTOR3 position;
		int direction;
		if (mirror)
		{
			position = D3DXVECTOR3((float)-xPos - image->GetXSize(), (float)yPos, 0.0f);
			direction = -1;
		}
		else
		{
			position = D3DXVECTOR3((float)xPos, (float)yPos, 0);
			direction = 1;
		}
		dS.position = &position;

		RECT* rect = new RECT();
		rect->top = 0;
		rect->bottom = image->GetYSize();
		rect->left = image->GetXSize()*imageIndex;
		rect->right = image->GetXSize()*(imageIndex + 1);
		dS.rect = rect;

		D3DXMATRIX transformMatrix;
		D3DXMatrixScaling(&transformMatrix, (float)direction, 1.0f, 1.0f);
		dS.transformMatrix = transformMatrix;

		Draw(dS, image);
	}

	void Graphics::DrawRotateImage(Image* image, int xPos, int yPos, double angle, int direction) const
	{
		DrawStruct dS = DrawStruct();

		D3DXMATRIX transformMatrix;
		D3DXMatrixScaling(&transformMatrix, (float)direction, 1.0f, 1.0f);
		D3DXVECTOR2 center((float)xPos, (float)yPos);
		D3DXMatrixTransformation2D(&transformMatrix, NULL, NULL, NULL, &center, (float)angle, NULL);
		dS.transformMatrix = transformMatrix;

		dS.position = &D3DXVECTOR3(static_cast<float>(xPos), static_cast<float>(yPos), 0);

		Draw(dS, image);
	}

	void Graphics::PrintText(int number, int xPos, int yPos, Color color, FontSize fontSize) const
	{
		PrintText(std::to_string(number), xPos, yPos, color, fontSize);
	}

	void Graphics::PrintText(std::string text, int xPos, int yPos, Color color, FontSize fontSize) const
	{
		D3DCOLOR D3DColor = ConvertToD3DCOLOR(color);
		RECT rectangle = { xPos, yPos, 0, 0 };
		LPD3DXFONT font = _font15;

		font->DrawText(
			0,
			text.c_str(),
			-1,
			&rectangle,
			DT_NOCLIP,
			D3DColor);
	}

	void Graphics::DrawRectangle(int xPos, int yPos, int xSize, int ySize, Color color) const
	{
		D3DCOLOR D3DColor = ConvertToD3DCOLOR(color);
		Image* image = whitePixel;
		D3DXVECTOR3 position((float)xPos / (float)xSize, (float)yPos / (float)ySize, 0.0f);
		_sprite->Begin(D3DXSPRITE_ALPHABLEND);
		D3DXMATRIX transformMatrix;
		D3DXMatrixScaling(&transformMatrix, (float)xSize, (float)ySize, 0.0f);
		_sprite->SetTransform(&transformMatrix);
		_sprite->Draw(
			*(image->GetTexture()),
			0,
			0,
			&position,
			D3DColor);
		_sprite->End();
	}

	void Graphics::DrawUnfilledRectangle(int xPos, int yPos, int xSize, int ySize, int thickness, Color color) const
	{
		//top
		DrawRectangle(xPos, yPos, xSize, thickness, color);
		//bottom
		DrawRectangle(xPos, yPos + ySize - thickness, xSize, thickness, color);
		//left
		DrawRectangle(xPos, yPos, thickness, ySize, color);
		//right
		DrawRectangle(xPos + xSize - thickness, yPos, thickness, ySize, color);
	}

	D3DCOLOR Graphics::ConvertToD3DCOLOR(Color color) const
	{
		D3DCOLOR resultColor;
		switch (color)
		{
		case GraphicsNS::Graphics::Color::WHITE:
			resultColor = D3DWHITE;
			break;
		case GraphicsNS::Graphics::Color::BLACK:
			resultColor = D3DBLACK;
			break;
		case GraphicsNS::Graphics::Color::RED:
			resultColor = D3DRED;
			break;
		case GraphicsNS::Graphics::Color::BLUE:
			resultColor = D3DBLUE;
			break;
		case GraphicsNS::Graphics::Color::GREEN:
			resultColor = D3DGREEN;
			break;
		case GraphicsNS::Graphics::Color::YELLOW:
			resultColor = D3DYELLOW;
			break;
		case GraphicsNS::Graphics::Color::PURPLE:
			resultColor = D3DPURPLE;
			break;
		default:
			break;
		}
		return resultColor;
	}

	LPD3DXFONT Graphics::ConvertToLPD3DXFONT(FontSize fontSize) const
	{
		LPD3DXFONT resultFont;
		switch (fontSize)
		{
		case GraphicsNS::Graphics::FontSize::font8:
			resultFont = _font8;
			break;
		case GraphicsNS::Graphics::FontSize::font15:
			resultFont = _font15;
			break;
		default:
			resultFont = _font15;
			break;
		}
		return resultFont;
	}
}

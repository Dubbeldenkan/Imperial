#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <d3d9.h>
#include <Windows.h>
#include "Image.h"
#include <stdio.h>
#include <D3dx9math.h>
#include <vector>
#include <string>
#include <math.h>

namespace GraphicsNS
{
	struct DrawStruct
	{
		float scale = 1.0f;
		RECT* rect = 0;
		D3DXVECTOR3* position = 0;
		D3DXVECTOR3* tilePosition = 0;
		D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255);
		D3DXMATRIX transformMatrix;

		DrawStruct();
	};

	class Graphics
	{
	public:
		enum Color { WHITE, BLACK, RED, BLUE, GREEN, YELLOW, PURPLE };

		const static int FONT_SIZE_SMALL = 8;
		const static int FONT_SIZE_BIG = 15;

	private:
		const static D3DCOLOR D3DWHITE = D3DCOLOR_XRGB(255, 255, 255);
		const static D3DCOLOR D3DBLACK = D3DCOLOR_XRGB(0, 0, 0);
		const static D3DCOLOR D3DRED = D3DCOLOR_XRGB(255, 0, 0);
		const static D3DCOLOR D3DBLUE = D3DCOLOR_XRGB(0, 0, 255);
		const static D3DCOLOR D3DGREEN = D3DCOLOR_XRGB(0, 255, 0);
		const static D3DCOLOR D3DYELLOW = D3DCOLOR_XRGB(255, 255, 0);
		const static D3DCOLOR D3DPURPLE = D3DCOLOR_XRGB(255, 0, 255);

		LPDIRECT3D9 _d3d;
		LPDIRECT3DDEVICE9 _d3dDevice;
		LPD3DXFONT _font8;
		LPD3DXFONT _font15;
		HDC _hdc;
		LPD3DXSPRITE _sprite = 0;
		std::vector<Image*> imageVector;
		std::string imagePath;

		Image* whitePixel;

	public:
		Graphics(HWND hWnd);
		~Graphics();

		void Flip(bool waitRetrace = true);
		void Clear(DWORD color = 0);
		void StartDrawing();
		void StopDrawing();

		//Används då man bara har en bild, dvs inte flera tiles
		Image* LoadImageFromFile(std::string);
		//Används då man bara har en bild, dvs inte flera tiles och definierad storlek
		Image* LoadImageFromFile(std::string, int, int);
		//Används då man har flera tiles
		Image* LoadImageFromFile(std::string, int, int, int, int);

		void Draw(Image*, int, int, float);
		void DrawWithColor(Image*, int, int, Color);
		void DrawWithColor(Image*, int, int, Color, float);
		void DrawTile(Image*, int, int, int, int);
		void DrawAnimation(Image*, int, int, int, int);
		void DrawRotateImage(Image*, int, int, double, int);

		void PrintText8(std::string, int, int, Color);
		void PrintText15(std::string, int, int, Color);
		void PrintText8(int, int, int, Color);
		void PrintText15(int, int, int, Color);

		void DrawRectangle(int, int, int, int, Color);

	private:
		void Draw(DrawStruct, Image*);
		D3DCOLOR ConvertToD3DCOLOR(Color);
	};
}

#endif
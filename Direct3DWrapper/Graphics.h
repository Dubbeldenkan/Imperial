#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Image.h"

#include <d3d9.h>
#include <D3dx9math.h>
#include <math.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <Windows.h>

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
		enum class Color { WHITE, BLACK, RED, BLUE, GREEN, YELLOW, PURPLE };
		enum class FontSize { font8, font15 };

		constexpr static int FONT_SIZE_SMALL = 8;
		constexpr static int FONT_SIZE_BIG = 15;

	private:
		constexpr static D3DCOLOR D3DWHITE = D3DCOLOR_XRGB(255, 255, 255);
		constexpr static D3DCOLOR D3DBLACK = D3DCOLOR_XRGB(0, 0, 0);
		constexpr static D3DCOLOR D3DRED = D3DCOLOR_XRGB(255, 0, 0);
		constexpr static D3DCOLOR D3DBLUE = D3DCOLOR_XRGB(0, 0, 255);
		constexpr static D3DCOLOR D3DGREEN = D3DCOLOR_XRGB(0, 255, 0);
		constexpr static D3DCOLOR D3DYELLOW = D3DCOLOR_XRGB(255, 255, 0);
		constexpr static D3DCOLOR D3DPURPLE = D3DCOLOR_XRGB(255, 0, 255);

		//TODO g�r dessa till const och flytta in dem i initieringslistan
		LPD3DXFONT _font8;
		LPD3DXFONT _font15;

		LPDIRECT3D9 _d3d;
		LPDIRECT3DDEVICE9 _d3dDevice;
		HDC _hdc;
		LPD3DXSPRITE _sprite = 0;
		std::vector<Image*> imageVector;
		std::string imagePath;

		Image* whitePixel;

	public:
		Graphics(HWND hWnd);
		~Graphics();

		/// S�tter det som har m�lats till sk�rmen, anv�nds efter stopDrawing
		void Flip(bool waitRetrace = true);
		/// Anv�nds f�r att ta bort det som nu finns p� bilden
		void Clear(DWORD color = 0);
		/// Skapar en ny canvas som kan m�las p�
		void StartDrawing();
		/// Anv�nds f�r att avsluta d� man har m�lat. Anv�nts f�r flip
		void StopDrawing();

		///Anv�nds d� man bara har en bild, dvs inte flera tiles
		Image* LoadImageFromFile(std::string fileName);
		///Anv�nds d� man bara har en bild, dvs inte flera tiles och definierad storlek
		Image* LoadImageFromFile(std::string fileName, int imageXSize, int imageYSize);
		///Anv�nds d� man har flera tiles
		Image* LoadImageFromFile(std::string fileName, int imageXSize, int imageYSize, int partImageSizeX, int partImageSizeY);

		/// F�r att rita ut en image
		void Draw(Image* image, int x, int y, float scale) const;
		/// Anv�nts f�r att m�la ut en image och samtidigt �ndra f�rgen p� imagen
		void DrawWithColor(Image* image, int x, int y, Color color) const;
		/// Anv�nds f�r att m�la ut en image och samtidigt �ndra f�rg och skala bilden
		void DrawWithColor(Image* image, int x, int y, Color color, float scale) const;
		/// Anv�nds f�r att m�la ut en tile fr�n en bild
		void DrawTile(Image* image, int x, int y, int tileIndex, int orientation) const;
		///Anv�nds f�r att m�la ut en tile fr�n en bild och eventuellt spegla den
		void DrawAnimation(Image* image, int x, int y, int imageIndex, bool mirror) const;
		/// Anv�nds f�r att rita ut en bild med en specifik vinkel
		void DrawRotateImage(Image* image, int xPos, int yPos, double angle, int direction) const;

		/// Skriver ut en text
		void PrintText(std::string text, int xPos, int yPos, Color color, FontSize fontSize) const;
		/// Skriver ut ett nummer
		void PrintText(int number, int xPos, int yPos, Color color, FontSize fontSize) const;

		///Draw a filled rectangle with the specified color
		void DrawRectangle(int xPos, int yPos, int xSize, int ySize, Color color) const;

		///Ritar ut en ofylld rektangel i specifiserad f�rg
		void DrawUnfilledRectangle(int xPos, int yPos, int xSize, int ySize, int thickness, Color color) const;

	private:
		///hj�lpfunktion som anv�nds f�r att rita ut en bild
		void Draw(DrawStruct, Image*) const;
		///Anv�nds f�r att ta ut f�rg baserat p� enum color
		D3DCOLOR ConvertToD3DCOLOR(Color color) const;
		///Anv�nds f�r att ta ut fontstorlek baserat p� enum color
		LPD3DXFONT ConvertToLPD3DXFONT(FontSize fontSize) const;
	};
}

#endif
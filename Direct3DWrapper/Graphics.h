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

		//TODO gör dessa till const och flytta in dem i initieringslistan
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

		/// Sätter det som har målats till skärmen, används efter stopDrawing
		void Flip(bool waitRetrace = true);
		/// Används för att ta bort det som nu finns på bilden
		void Clear(DWORD color = 0);
		/// Skapar en ny canvas som kan målas på
		void StartDrawing();
		/// Används för att avsluta då man har målat. Använts för flip
		void StopDrawing();

		///Används då man bara har en bild, dvs inte flera tiles
		Image* LoadImageFromFile(std::string fileName);
		///Används då man bara har en bild, dvs inte flera tiles och definierad storlek
		Image* LoadImageFromFile(std::string fileName, int imageXSize, int imageYSize);
		///Används då man har flera tiles
		Image* LoadImageFromFile(std::string fileName, int imageXSize, int imageYSize, int partImageSizeX, int partImageSizeY);

		/// För att rita ut en image
		void Draw(Image* image, int x, int y, float scale) const;
		/// Använts för att måla ut en image och samtidigt ändra färgen på imagen
		void DrawWithColor(Image* image, int x, int y, Color color) const;
		/// Används för att måla ut en image och samtidigt ändra färg och skala bilden
		void DrawWithColor(Image* image, int x, int y, Color color, float scale) const;
		/// Används för att måla ut en tile från en bild
		void DrawTile(Image* image, int x, int y, int tileIndex, int orientation) const;
		///Används för att måla ut en tile från en bild och eventuellt spegla den
		void DrawAnimation(Image* image, int x, int y, int imageIndex, bool mirror) const;
		/// Används för att rita ut en bild med en specifik vinkel
		void DrawRotateImage(Image* image, int xPos, int yPos, double angle, int direction) const;

		/// Skriver ut en text
		void PrintText(std::string text, int xPos, int yPos, Color color, FontSize fontSize) const;
		/// Skriver ut ett nummer
		void PrintText(int number, int xPos, int yPos, Color color, FontSize fontSize) const;

		///Draw a filled rectangle with the specified color
		void DrawRectangle(int xPos, int yPos, int xSize, int ySize, Color color) const;

		///Ritar ut en ofylld rektangel i specifiserad färg
		void DrawUnfilledRectangle(int xPos, int yPos, int xSize, int ySize, int thickness, Color color) const;

	private:
		///hjälpfunktion som används för att rita ut en bild
		void Draw(DrawStruct, Image*) const;
		///Används för att ta ut färg baserat på enum color
		D3DCOLOR ConvertToD3DCOLOR(Color color) const;
		///Används för att ta ut fontstorlek baserat på enum color
		LPD3DXFONT ConvertToLPD3DXFONT(FontSize fontSize) const;
	};
}

#endif
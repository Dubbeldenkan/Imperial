#ifndef IO_H
#define IO_H

#include "Game.h"
#include "UtilityClasses.h"

#include <Windows.h>

class IO
{
#define SCREENXSIZE 1500
#define SCREENYSIZE 800

public:
	static const int ID_NEW_GAME = 0;
	static const int ID_SAVE = 1;
	static const int ID_LOAD = 2;
	static const int ID_EXIT = 3;

	static const int ID_HELP = 10;
	static const int ID_CONTROLS = 11;

public:
	IO();
	~IO() {};

	void EndGame();

	static LRESULT CALLBACK MsgProc(HWND, UINT, WPARAM, LPARAM);
	static void InitWinMain(HINSTANCE*, std::string, IO*);
	static HWND InitWindow(HINSTANCE*, std::string, IO*);
	static HMENU CreateMainMenu();

private:
	static void MouseClick(int, int);
	static void MouseMove(int, int);
	static void MenuOption(int);
};

#endif // !CONTROL_H
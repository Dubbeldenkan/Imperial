#include "Graphics.h"
#include <Windows.h>
#include "IO.h"
#include <sys\timeb.h>

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	//init rand
	srand((unsigned int)time(NULL));

	const int sleepTime = 20;
	IO* io = new IO();
	std::string windowName = "Imperial"; //TODO fixa en snyggare lösning

	HWND hWnd = IO::InitWindow(&hInstance, windowName, io);

	GameBoardObject::SetGraphics(&hWnd); // TODO lös på ett snyggare sätt
	Game *game = new Game();

	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);

	// Main message loop:
	while (true)
	{
		game->Run();
		if (game->GetGameOver())
		{
			io->EndGame();
		}
		MSG msg = { 0 };
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
		}
		Sleep(sleepTime);
	}
	return 0;
}
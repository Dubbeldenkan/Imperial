#include "IO.h"

IO::IO()
{}

void IO::InitWinMain(HINSTANCE* hInst, std::string winName, IO* io)
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = io->MsgProc;
	wc.hInstance = *hInst;
	wc.lpszClassName = winName.c_str();
	wc.hCursor = (HCURSOR)LoadCursor(0, IDC_ARROW);
	RegisterClass(&wc);
}

HWND IO::InitWindow(HINSTANCE* hInst, std::string winName, IO* io)
{
	InitWinMain(hInst, winName, io);

	DWORD windowStyle = WS_OVERLAPPEDWINDOW;
	RECT windowRect = { 1, 1, SCREENXSIZE, SCREENYSIZE};
	AdjustWindowRect(&windowRect, windowStyle, false);
	HWND hWnd = CreateWindow(
		winName.c_str(),
		winName.c_str(),
		windowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		CreateMainMenu(),
		*hInst,
		NULL);

	return hWnd;
}

LRESULT CALLBACK IO::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
	{
		//TODO lägg till keys pressed här
		break;
	}
	case WM_KEYUP:
	{
		//TODO lägg till keys released här
		break;
	}
	case WM_LBUTTONUP:
	{
		break;
	}
	case WM_MOUSEMOVE:
	{
		int xPos = LOWORD(lParam);
		int yPos = HIWORD(lParam);
		MouseMove(xPos, yPos);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		int xPos = LOWORD(lParam);
		int yPos = HIWORD(lParam);
		MouseClick(xPos, yPos);
		break;
	}
	case WM_COMMAND:
	{
		int menuId = LOWORD(wParam);
		MenuOption(menuId);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		ValidateRect(hWnd, 0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

HMENU IO::CreateMainMenu()
{
	HMENU main = CreateMenu();

	HMENU file = CreateMenu();
	AppendMenu(file, MF_STRING, IO::ID_NEW_GAME, "Nytt Spel");
	AppendMenu(file, MF_STRING, IO::ID_SAVE, "Spara Spel");
	AppendMenu(file, MF_STRING, IO::ID_LOAD, "Ladda Spel");
	AppendMenu(file, MF_SEPARATOR, 0, 0);
	AppendMenu(file, MF_STRING, IO::ID_EXIT, "Avsluta");
	AppendMenu(main, MF_POPUP, (UINT_PTR)file, "Meny");

	HMENU help = CreateMenu();
	AppendMenu(help, MF_STRING, IO::ID_CONTROLS, "Kontroller");
	AppendMenu(help, MF_STRING, IO::ID_HELP, "Hjälp");
	AppendMenu(main, MF_POPUP, (UINT_PTR)help, "Hjälp");

	return main;
}

void IO::EndGame()
{
	std::string text = "Spelet är slut." 
		"\nVill du spela en ny omgång?";
	int result = MessageBox(NULL, text.c_str(),
		"Spelet Slut", MB_YESNO);
	if (result == IDYES)
	{
		//TODO
	}
	else if (result == IDNO)
	{
		PostQuitMessage(0);
	}
}

void IO::MouseClick(int xPos, int yPos)
{
	TupleInt clickPos = TupleInt(xPos, yPos);
	Game::MouseClicked(clickPos);
}

void IO::MouseMove(int xPos, int yPos)
{
	TupleInt movePos = TupleInt(xPos, yPos);
	Game::MouseMoved(movePos);
}

void IO::MenuOption(int menuId)
{
	switch (menuId)
	{
	case ID_NEW_GAME:
	{
		int result = MessageBox(NULL, "Säker att du vill starta ett nytt spel och avsluta nuvarande?",
			"Nytt Spel", MB_YESNO);
		if (result == IDYES)
		{
			Game::SetInitNewGame();
		}
		break;
	}
	case ID_SAVE:
	{
		Game::SetSaveGame();
		break;
	}
	case ID_LOAD:
	{
		Game::SetLoadGame();
		break;
	}
	case ID_HELP:
	{
		MessageBox(NULL,
			"TODO",	"Hjälp", MB_OK);
		break;
	}
	case ID_CONTROLS:
	{
		MessageBox(NULL,
			"TODO",	"Kontroller", MB_OK);
		break;
	}
	case ID_EXIT:
	{
		PostQuitMessage(0);
		break;
	}
	default:
		break;
	}
}
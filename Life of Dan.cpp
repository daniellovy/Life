// test project.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Windowsx.h"

#include <windows.h>
#include <shobjidl.h> 

#include "Life of Dan.h"
#include "grid.h"
#include "DanFile.h"
#include "ControlIT.h"

#define MAX_LOADSTRING 100

#define ID_TESTBUTTON 1
#define ID_RUNBUTTON 3
#define ID_TESTEDIT 2
#define ID_EDITCHILD 4
//////////////////////////file stuff


////////////////////////
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
WCHAR sszText[50];
TCHAR *fnText[50];
char inbuff[MAX_FILESTRING];
HWND hwndEdit;
HWND hwndEdit2;
HWND hwndLable;
HWND hwndLable2;

HWND hwndCheckbox;
bool dialogstate;  //hack to turn of screen updates if a file dialog is showing
ControlIT Controler;

grid *testgrid;
DanFile FileHandler;

HFONT fontLabeltiny = CreateFont(-12, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Ariel");


void __cdecl _tmain(int argc, TCHAR *argv[]);
int testwrite();
void Scribe();
void LifeLoader();
HRESULT BasicFileOpen();


void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Release(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Settings(HWND, UINT, WPARAM, LPARAM);

void textout(HWND hWnd, LPSTR zText)
{
	TCHAR cret[] = L"\n";
	int ndx;
	ndx = GetWindowTextLength(hWnd);
	SetFocus(hWnd);
#ifdef WIN32
	SendMessage(hWnd, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
#else
	SendMessage(hWnd, EM_SETSEL, 0, MAKELONG(ndx, ndx));
#endif
	SendMessage(hWnd, EM_REPLACESEL, 0, (LPARAM)((LPSTR)zText));

	// add the CR
	ndx = GetWindowTextLength(hWnd);
	SetFocus(hWnd);
#ifdef WIN32
	SendMessage(hWnd, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
#else
	SendMessage(hWnd, EM_SETSEL, 0, MAKELONG(ndx, ndx));
#endif
	SendMessage(hWnd, EM_REPLACESEL, 0, (LPARAM)((LPSTR)cret));
}

void paintcontrols(HWND hWnd)
{

	hwndLable = CreateWindow(
		L"static",  // Predefined class; Unicode assumed 
		L"Cell Size",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD,  // Styles 
		LEFT_EDGE,         // x position 
		35,         // y position 
		70,        // Button width
		15,        // Button height
		hWnd,     // Parent window
		(HMENU)0,       // No menu.
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);      // Pointer not needed.

	_itow_s(testgrid->GR_CELLSIZE, sszText, 10);

	hwndEdit = CreateWindow(
		L"EDIT",  // Predefined class; Unicode assumed 
		sszText,      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | ES_NUMBER,  // Styles 
		LEFT_EDGE+72,         // x position 
		33,         // y position 
		50,        // Button width
		21,        // Button height
		hWnd,     // Parent window
		(HMENU)ID_TESTEDIT,       // No menu.
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);      // Pointer not needed.


	HWND hwndButton = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"New Grid",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		LEFT_EDGE,         // x position 
		62,         // y position 
		120,        // Button width
		B_HEIGHT,        // Button height
		hWnd,     // Parent window
		(HMENU)ID_TESTBUTTON,       // No menu.
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);      // Pointer not needed.


	HWND hwndCheckbox = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		L"Grid Lines On",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,  // Styles 
		LEFT_EDGE,         // x position 
		97,         // y position 
		120,        // Button width
		B_HEIGHT,        // Button height
		hWnd,     // Parent window
		(HMENU)BTN_LIFE_GRIDTOGGLE,       // No menu.
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);      // Pointer not needed.

	hwndLable2 = CreateWindow(
		L"static",  // Predefined class; Unicode assumed 
		L"Faster with grid lines off.",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD,  // Styles 
		LEFT_EDGE,         // x position 
		125,         // y position 
		150,        // Button width
		15,        // Button height
		hWnd,     // Parent window
		(HMENU)0,       // No menu.
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);      // Pointer not needed.

	SendMessage(hwndLable2, WM_SETFONT, (WPARAM)fontLabeltiny, MAKELPARAM(true, 0));


	SendMessage(hwndCheckbox, BM_SETCHECK, BST_CHECKED, 0);
	testgrid->gridtoggle = IsDlgButtonChecked(hWnd, BTN_LIFE_GRIDTOGGLE);

	//CheckDlgButton(hwndCheckbox, BTN_LIFE_GRIDTOGGLE, BST_CHECKED);

	Controler.PaintIT(hWnd);

}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TESTPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTPROJECT));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTPROJECT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TESTPROJECT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
   int horizontal = 0;
   int vertical = 0;
   GetDesktopResolution(horizontal, vertical);
   //HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_MAXIMIZE,
   //   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ,
	   0, 0, horizontal, vertical, nullptr, nullptr, hInstance, nullptr);

   //SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0L);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);

   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int newsize;
    switch (message)
    {
	case BN_CLICKED:
		int i;
		i = 1;
		break;
	case WM_CREATE:
		RECT lpRect;
		 GetWindowRect(
			hWnd,
			&lpRect
		);
		 //ShowWindow(hWnd, SW_MAXIMIZE);
		 testgrid = new grid; 
		 //remeber to call destroy if you create another
		 //testgrid->grid_create(100, 100, 10);
		 newsize = 10;

		 testgrid->grid_create((lpRect.right - lpRect.left-X_OFFSET-BORDER_OFFSET) / (newsize + GR_CELLSPACE), (lpRect.bottom - lpRect.top-Y_OFFSET-BORDER_OFFSET-75 ) / (newsize + GR_CELLSPACE), newsize);
		 //testgrid->grid();
		
		paintcontrols(hWnd);
		dialogstate = FALSE;
		
			//strcpy_s((char *)fnText,40,"c:\\dantest.txt");
		//_tcscpy_s(fnText, "c:\\dantest.txt");

		break;

	case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);

            // Parse the menu selections:
            switch (wmId)
            {

			case ID_HELP_RELEASENOTES:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_RELEASE), hWnd, Release);
				break;

            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
			case ID_DAN_SETTINGS:
				//DialogBox(hInst, MAKEINTRESOURCE(IDD_SETTINGS), hWnd, Settings);
				
				//_tmain(2,fnText);
				// basic file operations
				//testwrite();
				//Scribe();
				//FileHandler.OpenDialog();
					
				
				break;


			case ID_DAN_MYOPEN:
				dialogstate = TRUE;


				LifeLoader();
				dialogstate = FALSE;
				InvalidateRect(hWnd, NULL, FALSE);
				break;

			case ID_DAN_SAVE:
				dialogstate = TRUE;

				FileHandler.SaveDialog();
				FileHandler.WriteStream("Hello\n");
				FileHandler.WriteStream("Line 1\n");
				FileHandler.WriteStream("Line 2\n");
				FileHandler.CloseStream();
				dialogstate = FALSE;

				break;

            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case ID_DAN_BYYOURCOMMAND:
				
				//testgrid->draw_grid(hWnd);
				testgrid->toggle(hWnd);
				//InvalidateRect(hWnd, NULL, TRUE);

				break;

			case BTN_LIFE_GRIDTOGGLE:
				//if (testgrid->gridtoggle)
				//{
				//	SendMessage(hwndCheckbox, BM_SETCHECK, BST_UNCHECKED, 0);
				//	testgrid->gridtoggle = FALSE;
				//}
				//else
				//{
				//	SendMessage(hwndCheckbox, BM_SETCHECK, BST_CHECKED, 0);

				//	testgrid->gridtoggle = TRUE;
				//}
				testgrid->gridtoggle = IsDlgButtonChecked(hWnd, BTN_LIFE_GRIDTOGGLE);
				InvalidateRect(hWnd, NULL, FALSE);

				
				break;

			case ID_DAN_RUN:
			case ID_RUNBUTTON:

				//testgrid->draw_grid(hWnd);
				testgrid->run(hWnd);
				SetFocus(hWnd);
				//InvalidateRect(hWnd, NULL, TRUE);

				break;

			case BTN_LIFE_GENERATION:
			case ID_DAN_GENERATION:
				testgrid->generation();
				InvalidateRect(hWnd, NULL, FALSE);
				break;

			case BTN_LIFE_CLEAR:
			case ID_DAN_CLEAR:
				testgrid->clear();
				InvalidateRect(hWnd, NULL, FALSE);
				break;

			case ID_TESTBUTTON:

				int editval;
				RECT lpRect;
				GetWindowRect(
					hWnd,
					&lpRect
				);
				GetWindowText(hwndEdit, sszText, 50);
				//retval = GetDlgItemText(hWnd, ID_TESTEDIT, szText, bufSize);
				//string myString = szText;
				editval = _ttoi(sszText);
				testgrid->GR_CELLSIZE = editval;
				delete testgrid;
				testgrid = new grid;
				testgrid->grid_create((lpRect.right - lpRect.left - X_OFFSET - BORDER_OFFSET) / (editval + GR_CELLSPACE), (lpRect.bottom - lpRect.top - Y_OFFSET - BORDER_OFFSET - 75) / (editval + GR_CELLSPACE), editval);
				//SendMessage(hwndCheckbox, BM_SETCHECK, BST_CHECKED, 0);
				testgrid->gridtoggle = IsDlgButtonChecked(hWnd, BTN_LIFE_GRIDTOGGLE);

				SetFocus(hWnd);
				InvalidateRect(hWnd, NULL, TRUE);
				break;

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
			if (!dialogstate)
			{
				HDC hdc = BeginPaint(hWnd, &ps);
				//TCHAR greeting[] = _T("Hello, World!");
				//_itow_s(testgrid->gencount, sszText, 10);
				//TextOut(hdc,
				//	10, 3,
				//	sszText, _tcslen(sszText));
				testgrid->draw_grid(hdc);
				//FillRect(hdc, &ps.rcPaint, (HBRUSH)(55));
				// TODO: Add any drawing code that uses hdc here...
				EndPaint(hWnd, &ps);
				//SendMessage(hwndEdit2, WM_SETTEXT, 0, (LPARAM)sszText);

				//textout(hwndEdit2, (LPSTR)sszText);
				SetFocus(hWnd);
			}

        }
        break;
	case WM_LBUTTONDOWN:
		int xPos, yPos;
		xPos = GET_X_LPARAM(lParam);
		yPos = GET_Y_LPARAM(lParam);
		testgrid->set_point(xPos, yPos);
		InvalidateRect(hWnd, NULL, FALSE);

		break;
    case WM_DESTROY:
		//delete[] testgrid;
        PostQuitMessage(0);
        break;

	case WM_CTLCOLORSTATIC:
		static HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
		{
			DWORD CtrlID = GetDlgCtrlID((HWND)lParam);
			//if (CtrlID == ID_TESTBUTTON) //If desired control
			{
				HDC hdcStatic = (HDC)wParam;
				SetTextColor(hdcStatic, RGB(0, 0, 0));
				SetBkColor(hdcStatic, RGB(255, 255, 255));
				//SetBkMode(hdcStatic, TRANSPARENT);
				return (INT_PTR)hBrush;
			}
		}
		break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// Message handler for Release Notes box.
INT_PTR CALLBACK Release(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	TCHAR releasetext[2048] = L"";

	wcscat_s(releasetext, L"Release Notes 1.0\r\n ");
	wcscat_s(releasetext, L"11/22/2017\r\n ");
	wcscat_s(releasetext, L"\r\n ");
	wcscat_s(releasetext, L"Welcome to Dan's Life!\r\n ");	
	wcscat_s(releasetext, L"\r\n ");	
	wcscat_s(releasetext, L"First coding excersize in decades so please be gentle.\r\n ");
	wcscat_s(releasetext, L"Mostly works.\r\n ");	
	wcscat_s(releasetext, L"\r\n ");
	wcscat_s(releasetext, L"Known open issues\r\n ");
	wcscat_s(releasetext, L"\r\n ");
	wcscat_s(releasetext, L"- File LOAD works with .RLE files.  These are run length encoded files for \r\n ");
	wcscat_s(releasetext, L"  Conway's life.  For formate description, see: \r\n ");
	wcscat_s(releasetext, L"  http://www.conwaylife.com/w/index.php?title=Run_Length_Encoded\r\n ");
	wcscat_s(releasetext, L"  I have only implemented the rule B3/S23.  All others behave strange.\r\n ");
	wcscat_s(releasetext, L"\r\n ");
	wcscat_s(releasetext, L"- File SAVE does not work.  It will create a test file.\r\n ");
	wcscat_s(releasetext, L"\r\n ");
	wcscat_s(releasetext, L"- You can paint one pixel at a time.  When I get a chance\r\n ");
	wcscat_s(releasetext, L"  I will add a continous mouse down paint.\r\n ");
	wcscat_s(releasetext, L"\r\n ");
	wcscat_s(releasetext, L"- The RUN infinite loop is truly infinite.  It's a hack.\r\n ");
	wcscat_s(releasetext, L"  The only way to break out of it is with a key press or mouse button.\r\n ");
	wcscat_s(releasetext, L"  Open to suggestions for a more elegant implementation.\r\n ");
	wcscat_s(releasetext, L"\r\n ");
	wcscat_s(releasetext, L"- There is one HUGE optimization to make.  Currently I redraw each cell\r\n ");
	wcscat_s(releasetext, L"  each refresh whether or not there has been a change.  Thats a lot of \r\n ");
	wcscat_s(releasetext, L"  reduntant little tiny rectangle calls.  It will eventually bother me enough\r\n ");
	wcscat_s(releasetext, L"  to fix.  Seems plenty fast right now though.\r\n ");
	wcscat_s(releasetext, L"\r\n ");

	wcscat_s(releasetext, L"ENJOY!\r\n ");
	wcscat_s(releasetext, L"    --- Dan\r\n ");
	






	switch (message)
	{
	case WM_INITDIALOG:
		SetDlgItemText(hDlg, IDC_EDIT1, releasetext);

		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Settings(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	//HWND editcontrol;
	int bufSize = 1024;
	switch (message)
	{
	case WM_INITDIALOG:
	
		_itow_s(testgrid->GR_CELLSIZE,sszText, 10);
		SetDlgItemText(hDlg, IDC_EDIT1, sszText);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK) 
		{

			UINT retval;
			int editval;
			LPTSTR szText = new TCHAR[bufSize];
			retval = GetDlgItemText(hDlg, IDC_EDIT1, szText,bufSize);
			//string myString = szText;
			editval = _ttoi(szText);
			delete[] szText;
			testgrid->GR_CELLSIZE = editval;
			//editcontrol = GetDlgItem(hDlg, IDC_EDIT1);
			//RedrawWindow(NULL, NULL, NULL, RDW_INTERNALPAINT);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
bool IsDigit(char c)
{
	return ('0' <= c && c <= '9');
}
void LifeLoader()
{
	int i,r,c,clength;
	int runlength;
	if (FileHandler.OpenDialog())
	{
		testgrid->clear();
		//HACK, just skip to line after X=)
		while (!(FileHandler.IsEOF()))
		{
			FileHandler.ReadStreamLine(inbuff);
			cUTF utf(inbuff);
			//textout(hwndEdit2, (LPSTR)utf.get16());
			if ((inbuff[0] == 'X') || (inbuff[0]=='x')) break;
		}
		while (!(FileHandler.IsEOF()))
		{

			FileHandler.ReadStreamLine(inbuff);
			cUTF utf(inbuff);
			//textout(hwndEdit2, (LPSTR)utf.get16());
		}
		int len = strlen(inbuff);
		r = 0;
		c = 0;
		for (i = 0; i < len; i++)
		{
			if (inbuff[i] == '\n') break;
			else
			{
				runlength = 1;
				if (IsDigit(inbuff[i]))
				{
					_tcscpy_s(sszText,50, L"         ");
					sszText[0] = inbuff[i];
					i++;
					if (IsDigit(inbuff[i])) { sszText[i] = inbuff[i]; i++; }
					if (IsDigit(inbuff[i])) { sszText[1] = inbuff[i]; i++; }
					if (IsDigit(inbuff[i])) { sszText[2] = inbuff[i]; i++; }

					runlength = _ttoi(sszText);
				}
				_itow_s(c, sszText, 10);
				//textout(hwndEdit2, (LPSTR)sszText);
				//textout(hwndEdit2, (LPSTR)" ");
				switch (inbuff[i])
				{
				case 'b':
					c += runlength;
					break;

				case 'o':
					for (clength = c; (clength-c) < runlength; clength++)
					{
						testgrid->set_grid(clength,r);
					}
					c += runlength;

					break;

				case '$':
					r++;
					c = 0;
					break;
				}

			}
		}
	}
	FileHandler.CloseStream();
}


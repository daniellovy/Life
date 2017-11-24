#include "stdafx.h"
#include "Windowsx.h"
#include "stdio.h"
#include "controlit.h"


#if !defined MSFTEDIT_CLASS
#define MSFTEDIT_CLASS L"RICHEDIT50W"
#endif



//****************************** BASE CLASS
// Constructor
ControlIT::ControlIT()
{
	 font = CreateFont(-14, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Courier New");
	 fontLabel = CreateFont(-14, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Ariel");
	 fontButton = CreateFont(-14, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Ariel");

	 controlCount = 0;
	 paramCount = 0;
	 indexCount = 0;

}

int ControlIT::MakeControl(HWND hWnd,int type, TCHAR * labeltext,int *initValue, int controlID,double * initDvalue,int flag)
{


	int posX, posY;
	long eflag;

	// WATCHOUT - this got duplicated in the label section, needed to make delete work 
	ctrlValues[indexCount] = initValue;
	ctrlDoubles[indexCount] = initDvalue;
	ctrlIDs[indexCount] = controlID;
	ctrlFlags[indexCount] = flag;


	ctrlType[indexCount]= type;

	switch (type)
	{
	case CT_BUTTON:
		if (flag == FL_UPDATEBTN)
		{
			posX = LEFT_EDGE + P_M_WIDTH + P_HORZ_SPACE;;
			posY = paramCount *(P_M_HEIGHT + P_VERT_SPACE) + P_TOP_EDGE;
		}
		else
		{

			posX = controlCount % NUM_B_COLUMNS;
			posY = controlCount / NUM_B_COLUMNS;
			posX = posX *(B_WIDTH + B_HORZ_SPACE) + LEFT_EDGE;
			posY = posY*(B_HEIGHT + B_VERT_SPACE) + B_TOP_EDGE;
		}
	

		ctrlHWND[indexCount]= CreateWindow( 
			L"BUTTON",  // Predefined class; Unicode assumed 
			labeltext,      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			posX,         // x position 
			posY,         // y position 
			B_WIDTH,        // Button width
			B_HEIGHT,        // Button height
			hWnd,     // Parent window
			(HMENU)controlID,       // No menu.
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.

		SendMessage(ctrlLabel_HWND[indexCount], WM_SETFONT, (WPARAM)fontButton, MAKELPARAM(true, 0));
		controlCount++;
		indexCount++;
		break;

	case CT_EDITBOX:
		ctrlType[indexCount] = CT_STATIC;  //funky I know
		posX = LEFT_EDGE;
		posY = paramCount *(P_M_HEIGHT + P_VERT_SPACE)+P_TOP_EDGE;

		if (flag == FL_DOUBLE)
		{
			//convert to double string
			swprintf_s(sszText, MAX_LOADSTRING, L"%1.3f", *initDvalue);
		}
		else
		{
			_itow_s(*initValue, sszText, 10);
		}

		eflag = ES_NUMBER;
		ctrlIDs[indexCount] = 0;
		ctrlLabel_HWND[indexCount] = CreateWindow(
			L"static",  // Predefined class; Unicode assumed 
			labeltext,      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD ,  // Styles 
			posX,         // x position 
			posY,         // y position 
			P_M_WIDTH,        // Button width
			P_M_HEIGHT,        // Button height
			hWnd,     // Parent window
			(HMENU)controlID,       // No menu.
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.
		//SendMessage(ctrlLabel_HWND[indexCount], WM_SETFONT, (WPARAM)fontLabel, MAKELPARAM(true, 0));
		indexCount++;



		ctrlValues[indexCount] = initValue;
		ctrlDoubles[indexCount] = initDvalue;
		ctrlIDs[indexCount] = controlID;
		ctrlFlags[indexCount] = flag;
		ctrlType[indexCount] = type;


		posX = LEFT_EDGE+P_M_WIDTH+P_HORZ_SPACE;
		posY = paramCount *(P_M_HEIGHT + P_VERT_SPACE) + P_TOP_EDGE;

		ctrlHWND[indexCount] = CreateWindow(
			L"EDIT",  // Predefined class; Unicode assumed 
			sszText,      // Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER | eflag,  // Styles 
			posX,         // x position 
			posY,         // y position 
			P_E_WIDTH,        // Button width
			P_E_HEIGHT,        // Button height
			hWnd,     // Parent window
			(HMENU)controlID,       // No menu.
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			NULL);      // Pointer not needed.

		paramCount++;
		indexCount++;
		break;

	case CT_LABEL:
		    
			controlCount += 3;
		    controlCount = controlCount + (NUM_B_COLUMNS - (controlCount % NUM_B_COLUMNS));
			posX = controlCount % NUM_B_COLUMNS;
			posY = controlCount / NUM_B_COLUMNS;
			posX = posX *(P_M_WIDTH + B_HORZ_SPACE) + LEFT_EDGE;
			posY = posY*(B_HEIGHT + B_VERT_SPACE) + B_TOP_EDGE;
		


			ctrlLabel_HWND[indexCount] = CreateWindow(
				L"static",  // Predefined class; Unicode assumed 
				labeltext,      // Button text 
				WS_TABSTOP | WS_VISIBLE | WS_CHILD,  // Styles 
				posX,         // x position 
				posY,         // y position 
				P_M_WIDTH + P_M_WIDTH,        // Button width
				P_M_HEIGHT,        // Button height
				hWnd,     // Parent window
				(HMENU)controlID,       // No menu.
				(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
				NULL);      // Pointer not needed.

		SendMessage(ctrlLabel_HWND[indexCount], WM_SETFONT, (WPARAM)fontLabel, MAKELPARAM(true, 0));
		controlCount +=3;
		indexCount++;
		break;
	}
	return indexCount;
}

void ControlIT::KillControls()
{
	for (int i = 0; i < indexCount; i++)
	{
		DestroyWindow(ctrlHWND[i]);
	}
	indexCount = 0;
	controlCount = 0; 
	paramCount = 0;
}

void ControlIT::PaintIT(HWND hWnd)
{
#define ID_RUNBUTTON 3 // hack will fix later

	MakeControl(hWnd, CT_BUTTON, L"Run", NULL, ID_RUNBUTTON,NULL,0);
	MakeControl(hWnd, CT_BUTTON, L"One Generation", NULL,BTN_LIFE_GENERATION, NULL, 0);
	MakeControl(hWnd, CT_BUTTON, L"Clear", NULL, BTN_LIFE_CLEAR, NULL, 0);
	//MakeControl(hWnd, CT_BUTTON, L"Spawn", NULL, 3, NULL, 0);
	//MakeControl(hWnd, CT_BUTTON, L"test ABD", NULL, 4, NULL, 0);
	//MakeControl(hWnd, CT_BUTTON, L"Load", NULL, 5, NULL, 0);
	//MakeControl(hWnd, CT_BUTTON, L"Generation", NULL, 6, NULL, 0);

	

	//SendMessage(populationPTR->hwndEdit2, WM_SETFONT, (WPARAM)font, MAKELPARAM(true, 0));

	SetFocus(hWnd);

}

void ControlIT::UpdateParams()
{
	
	for (int i = 0; i < indexCount; i++)
	{
		if (ctrlType[i] == CT_EDITBOX)
		{
			if (ctrlFlags[i] == FL_DOUBLE)
			{
				GetWindowText(ctrlHWND[i], sszText, MAX_LEADBYTES);
				if (ctrlDoubles[i] != NULL)
				{
					//*ctrlDoubles[i] = _wtodbl(sszText);
					//cUTF utf(sszText);
					//TCHAR *t = _T("1000.99");
					LPTSTR endPtr;

					*ctrlDoubles[i] = _tcstod(sszText, &endPtr);;
					//_atodbl((_CRT_DOUBLE *)ctrlDoubles[i], (char *)utf.get8());
				}
	
			}
			else
			{
				GetWindowText(ctrlHWND[i], sszText, MAX_LOADSTRING);
				if (ctrlValues[i]!=NULL)
					*ctrlValues[i] = _ttoi(sszText);
			}
		}
	}
}

ControlIT::~ControlIT()
{

}



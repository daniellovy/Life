#pragma once

#define MAX_LOADSTRING 100

#define MAX_CONTROLS 30
#define CT_BUTTON 1
#define CT_EDITBOX 2
#define CT_STATIC 3
#define CT_LABEL 4

#define FL_DOUBLE 1
#define FL_UPDATEBTN 2

#define B_HEIGHT 30
#define B_WIDTH 114
#define B_HORZ_SPACE 8
#define B_VERT_SPACE 8
#define LEFT_EDGE 35
#define B_TOP_EDGE 175
#define NUM_B_COLUMNS 1

#define P_HORZ_SPACE 5
#define P_VERT_SPACE 15
#define P_TOP_EDGE 40

#define P_M_WIDTH 180
#define P_M_HEIGHT 30

#define P_E_WIDTH 70
#define P_E_HEIGHT 20

// Defining contro IDs
// conventions is
// CONTEXT_NAME_TYPE_NAME
// Current contexts are
//    BASE = owned by the base population class
//    TSP = Traveling Sales Man test
//    NEU = neuron evolver

#define BTN_LIFE_GENERATION 1001
#define BTN_LIFE_CLEAR 1002
#define BTN_LIFE_GRIDTOGGLE 1003

// Base buttons 2000
#define BASE_BTN_UPDATE 2000
#define BASE_BTN_EVOLVE 2001
#define BASE_BTN_NEWPOP 2002

// Base Fields 2500
#define BASE_FLD_POPULATION 2500
#define BASE_FLD_CHROMOSOME  2501
#define BASE_FLD_MUTATE 2502

// Traveling buttons 3000
#define TSP_BTN_HEURISTIC 3000
#define TSP_BTN_NEWPATHS 3001
#define TSP_BTN_TOGGLE 3002



class ControlIT
{
private:

public:

	int test1, test2, test3;  //get rid of after testing.
	double testdouble;

	WCHAR sszText[MAX_LOADSTRING];  //temp will fix later
	char charzText[MAX_LOADSTRING];
	HFONT font;
	HFONT fontLabel;
	HFONT fontButton;

	int deviceWidth, deviceHeight;

	int controlCount, paramCount,indexCount;

	HWND ctrlHWND[MAX_CONTROLS];
	HWND ctrlLabel_HWND[MAX_CONTROLS];
	int  *ctrlValues[MAX_CONTROLS];
	int  ctrlIDs[MAX_CONTROLS];
	double  *ctrlDoubles[MAX_CONTROLS];

	int ctrlType[MAX_CONTROLS];
	int ctrlFlags[MAX_CONTROLS];


	///TEMP FOR TEST AND XFER REMOVE



	HWND hwndButton2;
	HWND hwndButton;

	ControlIT();

	

	virtual void PaintIT(HWND);
	virtual int MakeControl(HWND, int , TCHAR * , int *, int ,double*,int);
	virtual void UpdateParams();
	virtual void KillControls();
	~ControlIT();
};



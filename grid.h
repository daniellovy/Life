#pragma once
//#define GR_ROWS 100
//#define GR_COLUMNS 100
//#define GR_CELLSIZE 10
#define GR_CELLSPACE 1
#define X_OFFSET 200
#define Y_OFFSET 25
#define BORDER_OFFSET 25
#define TESTSHIFT 5
#define LOAD_ROW_OFFS 10
#define LOAD_COL_OFFS 10

class grid
{
private:
	//int map[GR_ROWS][GR_COLUMNS];
	//int switchmap[GR_ROWS][GR_COLUMNS];

	int **map;
	int **switchmap;

	int GR_ROWS;
	int GR_COLUMNS;




	bool show = FALSE;
public:
	int GR_CELLSIZE;
	int gencount;
	bool gridtoggle;
	grid();
	void set_point(int row, int column);
	void draw_grid(HDC hdc);
	void toggle(HWND hWnd);
	void generation();
	void run(HWND hWnd);
	void set_grid(int, int);
	void clear();
	void grid_create(int, int, int);
	void grid_destroy();
	~grid();
};


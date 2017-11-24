#include "stdafx.h"
#include "grid.h"
#include "time.h"



grid::grid()
{
	unsigned int time_ui = unsigned int(time(NULL));

	srand(time_ui);
}
void grid::grid_create(int columns, int rows, int size)
{
	int r, c, rn;
	gencount = 0;

	gridtoggle = TRUE;

	GR_ROWS = rows;
	GR_COLUMNS = columns;
	GR_CELLSIZE = size;

	map = new int*[GR_ROWS];
	switchmap = new int*[GR_ROWS];

	for (int i = 0; i < GR_ROWS; i++)
	{
		map[i] = new int[GR_COLUMNS];
		switchmap[i] = new int[GR_COLUMNS];
	}

	for (r = 0; r < GR_ROWS; r++)
	{
		for (c = 0; c < GR_COLUMNS; c++)
		{
			rn = rand() % 3;
			if (rn == 1)
				map[r][c] = 1;
			else
				map[r][c] = 0;
		}
	}

}

void grid::set_point(int column, int row)
{
	if ((column-X_OFFSET >= 0) && (row-Y_OFFSET >0) && 
		(column <(X_OFFSET + (GR_COLUMNS *(GR_CELLSIZE + GR_CELLSPACE))) ) &&
		(row <( Y_OFFSET + (GR_ROWS *(GR_CELLSIZE + GR_CELLSPACE)))) )
		
		if(map[int((row - Y_OFFSET) / (GR_CELLSIZE + GR_CELLSPACE))][int((column - X_OFFSET) / (GR_CELLSIZE + GR_CELLSPACE))]==1)
			map[int((row - Y_OFFSET )/(GR_CELLSIZE+GR_CELLSPACE ))][int((column - X_OFFSET )/ (GR_CELLSIZE + GR_CELLSPACE))] = 0;
		else
			map[int((row - Y_OFFSET) / (GR_CELLSIZE + GR_CELLSPACE))][int((column - X_OFFSET) / (GR_CELLSIZE + GR_CELLSPACE))] = 1;

}

void grid::set_grid(int column, int row)
{
	  
		//map[row + LOAD_ROW_OFFS][column + LOAD_COL_OFFS] = 1;
		map[row + GR_ROWS/2][column + GR_COLUMNS/2] = 1;


}

void grid::clear()
{
	int r, c;
	for (r = 0; r < GR_ROWS; r++)
	{
		for (c = 0; c<GR_COLUMNS; c++)
		{
			map[r][c] = 0;
		}
	}
}

void grid::draw_grid(HDC hdc)
{
	int r;
	int c;
	//if (show)
	HBRUSH hWhiteBrush = CreateSolidBrush(RGB(255, 255, 255));
	HBRUSH hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP memBitmap = CreateCompatibleBitmap(hdc, (GR_COLUMNS )*(GR_CELLSPACE + GR_CELLSIZE), (GR_ROWS )*(GR_CELLSPACE + GR_CELLSIZE));
	SelectObject(memDC, memBitmap);
	//BitBlt(memDC, 0, 0, (GR_ROWS )*(GR_CELLSPACE + GR_CELLSIZE), (GR_COLUMNS )*(GR_CELLSPACE + GR_CELLSIZE),
	//	hdc, X_OFFSET, Y_OFFSET, SRCCOPY);
	SelectObject(memDC, hWhiteBrush);
	Rectangle(memDC, 0, 0, (GR_COLUMNS)*(GR_CELLSPACE + GR_CELLSIZE), (GR_ROWS)*(GR_CELLSPACE + GR_CELLSIZE));
	

		//Ellipse(hdc, 20, 20, 500, 500);
		for (r = 0; r < GR_ROWS; r++)
		{
			for (c=0;c<GR_COLUMNS;c++)
			{
				if (map[r][c] == 1) SelectObject(memDC, hBlackBrush);
				else SelectObject(memDC, hWhiteBrush);

				if ((map[r][c] == 1) || (gridtoggle==TRUE))
				Rectangle(memDC, c * (GR_CELLSIZE + GR_CELLSPACE), r * (GR_CELLSIZE + GR_CELLSPACE),
					c *(GR_CELLSIZE + GR_CELLSPACE)  + GR_CELLSIZE, r *(GR_CELLSIZE + GR_CELLSPACE)   + GR_CELLSIZE);

			}
		}
		BitBlt(hdc, X_OFFSET, Y_OFFSET, (GR_COLUMNS )*(GR_CELLSPACE + GR_CELLSIZE), (GR_ROWS )*(GR_CELLSPACE + GR_CELLSIZE),
			memDC, 0, 0, SRCCOPY);
		DeleteObject(hBlackBrush);
		DeleteObject(hWhiteBrush);
		DeleteDC(memDC);
		DeleteObject(memBitmap);
	
}

void grid::generation()
{
	int r, c, n;
	int r1, c1;
	gencount++;

	for (r = 0; r < GR_ROWS; r++)
	{
		for (c = 0; c<GR_COLUMNS; c++)
		{
			//switchmap[r][c] = map[r + TESTSHIFT][c + TESTSHIFT];
			// Determine number of neighbors
			n = 0;


			for (r1 = -1; r1 < 2; r1++) 
			{
				for (c1 = -1; c1 < 2; c1++)
				{
					if (!((r1==0) && (c1==0)))
					{
						if ((r+r1>=0)&&(c+c1>=0)&&(r+r1 < GR_ROWS)&&(c+c1<GR_COLUMNS))
							if (map[r + r1][c + c1] == 1) n++;
					}
						
				}
			}

			switchmap[r][c] = 0;
			if ((n < 2) && (map[r][c] == 1)) switchmap[r][c] = 0;
			if (((n == 2) || (n == 3)) && (map[r][c] == 1)) switchmap[r][c] = 1;
			if ((n>3) && (map[r][c]==1)) switchmap[r][c] = 0;
			if ((map[r][c]==0) && (n==3)) switchmap[r][c] = 1;
		}
	}
	// copy back to main grid map
	for (r = 0; r < GR_ROWS; r++)
	{
		for (c = 0; c<GR_COLUMNS; c++)
		{
			map[r][c] = switchmap[r][c];
		}
	}
}

void grid::toggle(HWND hWnd)
{
	if (show) 
		show = FALSE;
	else
		show = TRUE;

	InvalidateRect(hWnd, NULL, TRUE);

}

void grid::run(HWND hWnd)
{

	bool tester = FALSE;
	MSG msg;
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	while (TRUE)
	{
		hdc = BeginPaint(hWnd, &ps);
		//Ellipse(hdc, i, i, i+50, i+50);
		draw_grid(hdc);
		EndPaint(hWnd, &ps);
		//Sleep(1);

		generation();
		tester = PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE);
		switch (msg.message)
		{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_KEYDOWN:
			// 
			// Perform any required cleanup. 
			// 
			InvalidateRect(hWnd, NULL, FALSE);

			return;
		}

		InvalidateRect(hWnd, NULL, FALSE);

	}
}
void grid::grid_destroy()
{
	for (int i = 0; i < GR_ROWS; i++)
	{
		delete[] map[i];
		delete[] switchmap[i];
	}

	delete[] map;
	delete[] switchmap;

}
grid::~grid()
{
	grid_destroy();
}

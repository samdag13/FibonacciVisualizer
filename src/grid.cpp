#include "grid.h"

//First cell layout is always DownLeft, init to UpLeft value so NextCellLayout() returns DownLeft at first iteration when initializing cells
Grid::Grid() : CurrentCellLayout(UpLeft), Display_h(0), Display_w(0), Zoom(1){}
//--------------------------------------------------------------------------------------------------------------------------------

Layout Grid::NextCellLayout() 
{
	switch (CurrentCellLayout)
	{
	case UpRight:
		CurrentCellLayout = UpLeft;
		break;
	case UpLeft:
		CurrentCellLayout = DownLeft;
		break;
	case DownLeft:
		CurrentCellLayout = DownRight;
		break;
	case DownRight:
		CurrentCellLayout = UpRight;
		break;
	}
	return CurrentCellLayout;
}
//--------------------------------------------------------------------------------------------------------------------------------

void Grid::InitCells(std::vector<unsigned int>& sequence)
{
	int size = sequence.size();

	//Nothing to do
	if (size == 0)
		return;

	Cells.reserve(size);
	for (int i = 0; i < size; i++) 
	{
		Cell c;
		c.SeqValue = sequence[i];
		c.Radius = c.SeqValue * Zoom;
		c.CellLayout = NextCellLayout();
		Cells.emplace_back(c);
	}

	//Base Case n = 1
	Cells[0].CenterX = Display_w / 2.0f;
	Cells[0].CenterY = Display_h / 2.0f;
	Cells[0].OppositeX = Cells[0].CenterX - Cells[0].Radius;
	Cells[0].OppositeY = Cells[0].CenterY + Cells[0].Radius;
	Cells[0].AngleMin = 0.5f;
	Cells[0].AngleMax = 1.0f;

	if (Cells[0].SeqValue == 1 && Cells[1].SeqValue == 1)
	{
		//Possible second occurence of n = 1
		Cells[1].CenterX = Cells[0].CenterX;
		Cells[1].CenterY = Cells[0].CenterY;
		Cells[1].OppositeX = Cells[1].CenterX + Cells[1].Radius;
		Cells[1].OppositeY = Cells[1].CenterY + Cells[1].Radius;
		Cells[1].AngleMin = 0.0f;
		Cells[1].AngleMax = 0.5f;

		//General Case starts at i = 2
		for (int i = 2; i < size; i++) 
			SetPositionAndAngles(i);
	}
	else
		//General Case starts at i = 1
		for (int i = 1; i < size; i++)
			SetPositionAndAngles(i);


}
//--------------------------------------------------------------------------------------------------------------------------------

void Grid::ResetGrid() 
{
	Cells.clear();
	CurrentCellLayout = UpLeft;
}
//--------------------------------------------------------------------------------------------------------------------------------

void Grid::SetUpGrid(std::vector<unsigned int> &sequence, int display_w, int display_h, int zoom)
{
	Display_w = display_w;
	Display_h = display_h;
	Zoom = zoom;
	InitCells(sequence);
}
//--------------------------------------------------------------------------------------------------------------------------------

std::vector<Cell>& Grid::GetCells()
{
	return Cells;
}

void Grid::SetPositionAndAngles(unsigned int i) 
{
	//AngleMin/Max are coefficients of PI for angles in rad. (Multiply by IM_PI in path plotting)
	switch (Cells[i].CellLayout)
	{
	case UpRight:
		Cells[i].CenterX = Cells[i - 1].CenterX - (Cells[i].Radius - Cells[i - 1].Radius);
		Cells[i].CenterY = Cells[i - 1].CenterY;
		Cells[i].OppositeX = Cells[i].CenterX + Cells[i].Radius;
		Cells[i].OppositeY = Cells[i].CenterY - Cells[i].Radius;
		Cells[i].AngleMin = 1.5f;
		Cells[i].AngleMax = 2.0f;
		break;
	case UpLeft:
		Cells[i].CenterX = Cells[i - 1].CenterX;
		Cells[i].CenterY = Cells[i - 1].CenterY + (Cells[i].Radius - Cells[i - 1].Radius);
		Cells[i].OppositeX = Cells[i].CenterX - Cells[i].Radius;
		Cells[i].OppositeY = Cells[i].CenterY - Cells[i].Radius;
		Cells[i].AngleMin = 1.0f;
		Cells[i].AngleMax = 1.5f;
		break;
	case DownLeft:
		Cells[i].CenterX = Cells[i - 1].CenterX + (Cells[i].Radius - Cells[i - 1].Radius);
		Cells[i].CenterY = Cells[i - 1].CenterY;
		Cells[i].OppositeX = Cells[i].CenterX - Cells[i].Radius;
		Cells[i].OppositeY = Cells[i].CenterY + Cells[i].Radius;
		Cells[i].AngleMin = 0.5f;
		Cells[i].AngleMax = 1.0f;
		break;
	case DownRight:
		Cells[i].CenterX = Cells[i - 1].CenterX;
		Cells[i].CenterY = Cells[i - 1].CenterY + (Cells[i - 1].Radius - Cells[i].Radius);
		Cells[i].OppositeX = Cells[i].CenterX + Cells[i].Radius;
		Cells[i].OppositeY = Cells[i].CenterY + Cells[i].Radius;
		Cells[i].AngleMin = 0.0f;
		Cells[i].AngleMax = 0.5f;
		break;
	}
}
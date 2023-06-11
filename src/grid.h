#include <vector>

enum Layout
{
	UpRight,
	UpLeft,
	DownRight,
	DownLeft
};

struct Cell
{
	Layout CellLayout;

	unsigned int SeqValue;

	float AngleMax;
	float AngleMin;
	float CenterX;
	float CenterY;
	float OppositeX;
	float OppositeY;
	float Radius;
};

class Grid
{
private:
	std::vector<Cell> Cells;

	Layout CurrentCellLayout;

	int Display_w;
	int Display_h;
	int Zoom;

	void InitCells(std::vector<unsigned int>& sequence);
	void SetPositionAndAngles(unsigned int i);

	Layout NextCellLayout();

public:

	Grid();

	void ResetGrid();
	void SetUpGrid(std::vector<unsigned int>& sequence, int display_w, int display_h, int zoom);

	std::vector<Cell>& GetCells();
};


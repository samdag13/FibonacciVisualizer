#include "fibonacci.h"
#include "grid.h"


class Controller
{
public:
	Controller();

	void ExportSequence(const std::string& filename);
	void SetSequence(unsigned int first, unsigned int last, int display_w, int display_h, int zoom);

	unsigned int GetFirst() const { return Calculator.GetFirst(); }
	unsigned int GetLast() const { return Calculator.GetLast(); }

	bool ImportSequence(const std::string& filename);

	std::vector<Cell>& Controller::GetPopulatedGrid() ;

private:
	Grid FibGrid;
	Fibonacci Calculator;

};
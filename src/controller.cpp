#include "controller.h"
#include "fstream"
#include "iostream"

Controller::Controller(){}
//--------------------------------------------------------------------------------------------------------------------------------

void Controller::SetSequence(unsigned int first, unsigned int last, int display_w, int display_h, int zoom) 
{
	Calculator.SetSequence(first, last);
	std::vector<unsigned int> sequence;
	Calculator.CalculateSequence(sequence);
	FibGrid.ResetGrid();
	FibGrid.SetUpGrid(sequence, display_w, display_h, zoom);
}
//--------------------------------------------------------------------------------------------------------------------------------

std::vector<Cell>& Controller::GetPopulatedGrid()
{
	return FibGrid.GetCells();
}
//--------------------------------------------------------------------------------------------------------------------------------

bool Controller::ImportSequence(const std::string& filename) 
{
	std::ifstream file;
	std::string initializer;

	file.open(filename);

	if (!file.is_open())
		return false;

	//Gets all char until first blank space
	file >> initializer;
	file.close();

	int exp = 0;
	int num = 0;
	int indexe = 0;
	unsigned int first, last;

	//Decode first
	while(initializer[indexe] != '/')
	{
		int currentInt = (initializer[indexe] - '0');
		if (currentInt == 0)
			num *= 10;
		else
			num += currentInt * pow(10, exp);

		indexe++;
		exp++;
	}

	indexe++;
	first = num;
	num = 0;
	exp = 0;

	//Decode last
	while (initializer[indexe] != ':')
	{
		int currentInt = (initializer[indexe] - '0');
		if (currentInt == 0)
			num *= 10;
		else
			num += currentInt * pow(10, exp);

		indexe++;
		exp++;
	}
	last = num;

	Calculator.SetSequence(first, last);

	return true;

}
//--------------------------------------------------------------------------------------------------------------------------------

void Controller::ExportSequence(const std::string &filename) 
{
	std::string sequence = Calculator.GetSerializedSequence();
	std::ofstream file;

	file.open(filename + ".txt");
	file << sequence;
	file.close();
}
//--------------------------------------------------------------------------------------------------------------------------------
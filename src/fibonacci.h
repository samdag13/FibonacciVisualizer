#include <string>
#include <vector>

class Fibonacci
{
public:
	Fibonacci();

	void CalculateSequence(std::vector<unsigned int>& v);
	void SetSequence(unsigned int first, unsigned int last);

	unsigned int GetFirst() const { return First; }
	unsigned int GetLast() const { return Last; }

	const std::string GetSerializedSequence() const;

private:
	unsigned int First;
	unsigned int Last;

	std::vector<unsigned int> Sequence;
};
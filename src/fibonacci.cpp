#include "Fibonacci.h"

Fibonacci::Fibonacci(): First(0), Last(0){}
//--------------------------------------------------------------------------------------------------------------------------------

void Fibonacci::SetSequence(unsigned int first, unsigned int last)
{
	First = first;
	Last = last;
}
//--------------------------------------------------------------------------------------------------------------------------------

//Aucun acces aux valeurs du vecteur, on peut considérer que l'utilisateur utilise des indexes qui débutent à 1 au lieu de 0 et continuer avec ce format pour cette fonction
void Fibonacci::CalculateSequence(std::vector<unsigned int>& v) 
{
	v.clear();
	v.reserve(Last - First);

	size_t size = v.size();
	unsigned int n0 = 1;
	unsigned int n1 = 1;
	if (n0 >= First && n1 >= First) 
	{
		v.push_back(n0);
		v.push_back(n1);
	}
	unsigned int n = n0 + n1;

	while (n <= Last) 
	{
		if(n >= First)
			v.push_back(n);
		n0 = n1;
		n1 = n;
		n = n0 + n1;
	}

	Sequence = v;
	//Discard extra space, operation in O(1) since no elements are erased from this operation
	v.resize(v.size());
}
//--------------------------------------------------------------------------------------------------------------------------------

const std::string Fibonacci::GetSerializedSequence() const
{
	std::string str;
	str += std::to_string(First);
	str += "/";
	str += std::to_string(Last);
	str += ": ";

	for each (unsigned int num in Sequence)
	{
		str += std::to_string(num);
		str += " ";
	}
	return str;
}
//--------------------------------------------------------------------------------------------------------------------------------

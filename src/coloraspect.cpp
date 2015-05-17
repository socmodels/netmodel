#include "framework.h"
#include "coloraspect.h"

void ColorAspect::Evolve()
{

}
int ColorAspect::GetInfoSize()
{
	return sizeof(ColorInfo);
}
void ColorAspect::InitializeAspect()
{
	// Set random colors for everybody...
	for (Population::Iterator agentId = this->begin;
		agentId != this->end; ++agentId)
	{
		(*this)[*agentId]->Color= rand() % 5;
	}
}

ColorInfo * ColorAspect::operator[](key agentId)
{
	return (ColorInfo *) this->GetInfo(agentId);
}

void ColorAspect::ShowValues(int agentId, std::vector <char *> & fields,
								 std::vector <double> & values)
{
	for (std::vector <char *>::size_type n = 0; n < fields.size();n++)
	{
		if (strcmp(fields[n], "Color")==0)
			values.push_back((*this)[agentId]->Color);
		else
			values.push_back(0);
	}
}

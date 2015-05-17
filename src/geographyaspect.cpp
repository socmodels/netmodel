#include "framework.h"
#include "geographyaspect.h"
#include "distributions.h"

void GeographyAspect::Evolve()
{

}
int GeographyAspect::GetInfoSize()
{
	return sizeof(GeographyInfo);
}
void GeographyAspect::InitializeAspect()
{
	int xWidth = GetConfigInt("Width");
	int yHeight = GetConfigInt("Height");
	int xFocusWidth = GetConfigInt("FocusWidth");
	int yFocusHeight = GetConfigInt("FocusHeight");

	_seed = GetConfigLong("Seed"); Seed = &_seed;
	// Set random socialcircles for everybody...
	GeographyInfo *element;
	for (Population::Iterator agentId = this->begin;
		agentId != this->end; ++agentId)
	{
		element = (*this)[*agentId];
		element->X = (int)floor(xWidth/2 * gasdev(Seed) / xFocusWidth);
		element->Y = (int)floor(yHeight/2 * gasdev(Seed) / yFocusHeight);
	}
}

GeographyInfo * GeographyAspect::operator[](key agentId)
{
	return (GeographyInfo *) this->GetInfo(agentId);
}

void GeographyAspect::ReleaseAspect()
{

}

void GeographyAspect::ShowValues(int agentId, std::vector <char *> & fields,
								 std::vector <double> & values)
{
	for (std::vector <char *>::size_type n = 0; n < fields.size();n++)
	{
		if (strcmp(fields[n], "X")==0)
			values.push_back((*this)[agentId]->X);
		else if (strcmp(fields[n], "Y")==0)
			values.push_back((*this)[agentId]->Y);
		else
			values.push_back(0);
	}
}

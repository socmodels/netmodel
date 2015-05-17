#include <string>
#include <vector>

#include "aspect.h"
#include "outputaggregation.h"

using std::vector;
#ifndef _OUTPUT
	#define _OUTPUT

struct FieldGroup
{
	AspectBase * Aspect;
	OutputAggregation *OutputAggregationManager;
	vector <char *> FieldNames;
	vector <double> FieldCriteria;
	vector <FieldOperationEnum> FieldOperations;
};

class Output
{
public:
	FILE*	File;
	int		FileCount;
	char *	Filename;
	bool	Headers;
	void *	SubPopulation;
	int		SubPopulationId;
	bool	IsAggregate;
	vector <FieldGroup *> FieldGroups;
	int		Loops;

	Output();
	~Output();
private:

};
#endif

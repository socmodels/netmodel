#include "outputaggregation.h"

OutputAggregation::OutputAggregation()
{
	_results =NULL;
}
void OutputAggregation::Initialize (vector <char *> *fieldNames,
									vector <FieldOperationEnum> *fieldOperations,
								vector <double> *fieldCriteria)
{
	_fieldOperations = fieldOperations;
	_fieldCriteria = fieldCriteria;
	_fieldNames = fieldNames;
	_resolved = false;
	_results = new double[_fieldNames->size()];
	_count = new int[_fieldNames->size()];
	startResults();
}
OutputAggregation::~OutputAggregation()
{
	if (_results != NULL)
	{
		delete _results;
		delete _count;
	}
}
					
void OutputAggregation::copyvector(double *results, vector <double> &values)
{
	for(vector <double>::size_type n = 0; n < values.size(); n++)
		results[n] = values[n];
}
void OutputAggregation::ProcessValues(vector <double> &values)
{
	if (_fieldOperations->size() == 0)
	{	// No tiene que hacer ning�n aggregate... copia valores
		copyvector(_results, values);
		return;
	}
	// Procesa para el aggregate...
	for(vector <double>::size_type n = 0; n < _fieldOperations->size(); n++)
	{
		switch((*_fieldOperations)[n])
		{
			case Sum:
			case Average:
				_results[n] += values[n];
				break;
			case Min:
				if (values[n] < _results[n] || _count[n] == 0)
					_results[n] += values[n];
				break;
			case Max:
				if (values[n] > _results[n] || _count[n] == 0)
					_results[n] += values[n];
				break;
			case Count:
				_results[n] += 1;
				break;
			case Percentage:
				if (values[n] == (*_fieldCriteria)[n])
					_results[n] += 1;
				/*
				Faltan implementar:
				Median,
				StdDev
				*/
			default:
				throw("Unsupported operation.");
		}
		_count[n]++;
	}
}
void OutputAggregation::BeginHeaders(FILE *file, bool isAggregate)
{
	fprintf(file,"Loop\t");
	if (isAggregate == false)
		fprintf(file,"AgentId\t");
}

void OutputAggregation::ShowHeaders(FILE *file)
{
	char buffer[40];
	for(vector <char *>::size_type n = 0; n < _fieldNames->size(); n++)
	{
		fprintf(file,"%s", (*_fieldNames)[n]);
		if (_fieldOperations->size() > 0) // isAggregate
		{
			fprintf(file,"_%s", operationToText((*_fieldOperations)[n],buffer));
		}
		fprintf(file,"\t");
	}
}
char *OutputAggregation::operationToText(FieldOperationEnum operation, char *buffer)
{
	switch(operation)
	{
	case Average:
		strcpy(buffer, "mean");
		break;
	case Sum:
		strcpy(buffer, "sum");
		break;
	case Min:
		strcpy(buffer, "min");
		break;
	case Max:
		strcpy(buffer, "max");
		break;
	case Median:
		strcpy(buffer, "med");
		break;
	case StdDev:
		strcpy(buffer, "stdv");
		break;
	case Count:
		strcpy(buffer, "count");
		break;
	case Percentage:
		strcpy(buffer, "pin");
		break;
	default:
		throw("Operation missing in outputAggregation::OperationToText");
	}
	return buffer;
}
void OutputAggregation::Resolve(void)
{
	if (_fieldOperations->size() > 0)
	{
		// Procesa para el aggregate...
		for(vector <double>::size_type n = 0; n < _fieldOperations->size(); n++)
		{
			switch((*_fieldOperations)[n])
			{
				case Sum:
				case Min:
				case Max:
				case Count:
					break;
				case Average:
				case Percentage:
					_results[n] /= _count[n];
					break;
					/*
					Faltan implementar:
					Median,
					StdDev,
					 */
				default:
					throw("Unsupported operation.");
			}
		}
	}
}
void OutputAggregation::Clear(void)
{
	startResults();
}
void OutputAggregation::startResults()
{
	// Initialize results...
	for(vector <double>::size_type n = 0; n < _fieldNames->size(); n++)
	{
		_results[n]=0;	
		_count[n]=0;	
	}
}

void OutputAggregation::BeginLine(FILE *file, int loop)
{
	BeginLine(file, loop, 0);
}
void OutputAggregation::BeginLine(FILE *file, int loop, unsigned int agentId)
{
	fprintf(file,"%ld\t", loop);
	if (agentId != 0)
		fprintf(file,"%#.8X\t", agentId);
}

void OutputAggregation::EndLine(FILE *file)
{
	fprintf(file,"\n");
}

void OutputAggregation::ShowValues(FILE *file)
{
	if (_resolved == false)
		Resolve();	
	for(vector <char *>::size_type n = 0; n < _fieldNames->size(); n++)
	{
		if (_results[n] != (int) _results[n])
            fprintf(file,"%f\t", _results[n]);
		else
			fprintf(file,"%ld\t", (int) _results[n]);
	}
	this->Clear();
}


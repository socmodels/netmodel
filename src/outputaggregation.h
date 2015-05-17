#include <string>
#include <vector>
#include <stdio.h>

using std::vector;
#ifndef _OUTPUT_AG
	#define _OUTPUT_AG

enum FieldOperationEnum
{
	Average,
	Sum,
	Min,
	Max,
	Median,
	StdDev,
	Count,
	Percentage
};

class OutputAggregation
{
    public:
        OutputAggregation();
        ~OutputAggregation();
        void Initialize (vector <char *> *fieldNames,
                                vector <FieldOperationEnum> *fieldOperations,
                                    vector <double> *fieldCriteria);
        void ProcessValues(vector <double> &values);
        void Resolve(void);
        void Clear(void);
        void static BeginLine(FILE *file, int loop);
        void static BeginLine(FILE *file, int loop, unsigned int agentId);
        void static BeginHeaders(FILE *file, bool isAggregate);
        void static EndLine(FILE *file);
        void ShowValues(FILE *file);
        void ShowHeaders(FILE *file);


    private:
        vector <FieldOperationEnum> *_fieldOperations;
        vector <char *>				*_fieldNames;
        vector <double>				*_fieldCriteria;
        double						*_results;
        int							*_count;
        FILE						*_file;
        bool						_resolved;
        void						startResults();
        void						copyvector(double *_results, vector <double> &values);
        char						*operationToText(FieldOperationEnum operation, char *buffer);
};

#endif

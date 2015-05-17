// framework.cpp : Defines the entry point for the console application.
//
#include "framework.h"

int _tmain(int argc, char* argv[])
{
	try
	{
		char *configFile = argv[1];
		Population p = Population(configFile);
		p.Run();
	}
	catch(char * str )
    {
        printf("\n** Error occured: %s **\n", str);
    }
	getc(stdin);
	return 0;
}


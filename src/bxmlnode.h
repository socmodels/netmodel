#include <stdio.h>
#include <string>
#ifndef _BXMLNODE
	#define _BXMLNODE

void deletebasicxmlnode( struct basicxmlnode * node );
void printbasicxmlnode( struct basicxmlnode * node );
struct basicxmlnode *readbasicxmlnode( FILE * fpi );
basicxmlnode *getchildxmlnode( struct basicxmlnode * node, char *child );
basicxmlnode *getchildxmlnode( struct basicxmlnode * node, int n);
int getchildxmlnodeintvalue( struct basicxmlnode * node, char *child );
int getchildxmlnodelongvalue( struct basicxmlnode * node, char *child );
char * getchildxmlnodestringvalue( struct basicxmlnode * node, char *child );
bool getchildxmlnodeboolvalue( struct basicxmlnode * node, char *child );
double getchildxmlnodedoublevalue( struct basicxmlnode * node, char *child );
char *getxmlnodetag( struct basicxmlnode * node);
char *getxmlnodetext( struct basicxmlnode * node);
char* getchildxmlnodeattributestringvalue( struct basicxmlnode * node, char *attribute );

#endif

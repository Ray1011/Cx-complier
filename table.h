#include "stdio.h"
#include "string.h"

#define TXMAX 100
#define AL 10
#define LEVMAX 3
#define AMAX 2047
#define STACKSIZE 500

int lev=0;
int tx=0;
int dx=0;
int num;
char id[AL];

enum object{variable};
enum object kind;

void error(int);

struct table1{
	char name[AL];
	enum object kind;
	int val,level,adr,size,type;
	};

struct table1 table[TXMAX+1];

void enter(enum object k){
tx=tx+1;
strcpy(table[tx].name,id);
table[tx].kind=k;
switch(k)
	{		
	case variable:
		table[tx].level=lev;		
		table[tx].adr=dx;
		table[tx].type=1;
		table[tx].val=0;
		dx++;
		break;
	}
}


int position(char id[10])
{
	int i;
	strcpy(table[0].name,id);
	i=tx;
	while(strcmp(table[i].name,id)!=0)
		i--;
	return i;
}

int positionX(char id[10],int lev)
{
	int i;
	strcpy(table[0].name,id);
	i=tx;
	while(strcmp(table[i].name,id)!=0 || lev !=table[i].level )
		{
			i--;
			if (i==0)
				break;
		}
	return i;
}

#include "table.h"
#define CXMAX 200

int cx;
int err;
int temp;

enum fct{lit,opr,lod,sto,cal,ini,jmp,jpc,jpcelse};

char *mnemonic[9]={"lit","opr","lod","sto","cal","ini","jmp","jpc","jpcelse"};

FILE *fin;
char fname[AL];

enum listswitcher{false,true};
enum listswitcher listswitch;

int WhilePointer;
int WStatementPointer;
int WhileStack[100];
int WStatementStack[100];

struct instruction{
	enum fct f;
	int l;
	int a;
};

struct instruction code[CXMAX+1];

extern int line;

void error(int n){
	printf("*** error%d occur ***\n",n);
	return;
}

void gen(enum fct x,int y,int z)
{
	if(cx>CXMAX)
		printf("program too long!");
	code[cx].f=x;
	code[cx].l=y;
	code[cx].a=z;
	cx++;
	return;
}

void listcode()
{
	int i;
	if(listswitch==true){
		for(i=0;i<=cx-1;i++){
			printf("%2d	%5s	%3d	%5d\n",i,mnemonic[(int)code[i].f],code[i].l,code[i].a);
			}
		}
	return;
}

void interpret()
{
	int p=0;
	int b=1;
	int t=0;
	struct instruction i;
	int s[STACKSIZE];
	printf("********Start CX*********\n");
	// for(int i = 0;i <10;i++)
 // 	{
 // 		printf("name:%s\n",table[i].name);
 // 		printf("lev:%d\n",table[i].level);
 // 		printf("adr:%d\n",table[i].adr);
 // 		printf("kind:%u\n",table[i].kind);
 // 		printf("val:%d\n",table[i].val);
 // 	}	
 	s[0]=0;
	s[1]=0;
	s[2]=0;
	s[3]=0;
	do{
		i=code[p];
		p=p+1;
		switch(i.f){
			case lit:
				t=t+1;
				s[t]=i.a;
				break;
			case opr:
				switch(i.a){
					case 0:
						t=b-1;
						p=s[t+3];
						b=s[t+2];
						break;
					case 1:
						s[t]=-s[t];
						break;
					case 2:
						t=t-1;
						s[t]=s[t]+s[t+1];
						break;
					case 3:
						t=t-1;
						s[t]=s[t]-s[t+1];
						break;
					case 4:
						t=t-1;
						s[t]=s[t]*s[t+1];
						break;
					case 5:
						t=t-1;
						s[t]=s[t]/s[t+1];
						break;
					case 6:
						if(s[t]%2==0)
							s[t]=0;
						else
							s[t]=1;
						break;
					case 8:
						t=t-1;
						if(s[t]==s[t+1])
							s[t]=1;
						else
							s[t]=0;
						break;
					case 9:
						t=t-1;
						if(s[t]==s[t+1])
							s[t]=0;
						else
							s[t]=1;
						break;
					case 10:
						t--;
						if(s[t]<s[t+1])
							s[t]=1;
						else
							s[t]=0;
						break;
					case 11:
						t--;
						if(s[t]>=s[t+1])
							s[t]=1;
						else
							s[t]=0;
						break;
					case 12:
						t=t-1;
						if(s[t]>s[t+1])
							s[t]=1;
						else
							s[t]=0;
						break;
					case 13:
						t=t-1;
						if(s[t]<=s[t+1])
							s[t]=1;
						else
							s[t]=0;
						break;
					case 14://SYM_not
						if(s[t]==1)
							s[t]=0;
						else
							s[t]=1;
						break;
					case 15://SYM_and
						t=t-1;
						if(s[t]==1 && s[t+1]==1)
							s[t]=1;
						else
							s[t]=0;
						break;
					case 16://SYM_or
						t=t-1;
						if(s[t]==1||s[t+1]==1)
							s[t]=1;
						else
							s[t]=0;
						break;
					case 17://SYM_write
						printf("%d\n",s[t]);
						t--;
						break;
					case 18://SYM_read
						printf("Please input number:");
						int num1;
						scanf("%d",&num1);
						t=t+1;
						s[t]=num1;
						break;
					case 19://SYM_true
						t=t+1;
						s[t]=1;
						break;
					case 20://SYM_false 
						t=t+1;
						s[t]=0;
						break;
					case 21://SYM_reminder
						t=t-1;
						s[t]=s[t+1]%s[t];
						break;
				}
				break;	
			case lod:
				t=t+1;
				s[t]=table[i.a].val;
				break;
			case sto:
				table[i.a].val=s[t];
				t=t-1;
				break;
			case cal:
				break;
			case ini:
				t=t+i.a;
				break;
			case jmp:
				p=i.a;
				break;
			case jpc:
				temp=s[t];
				if(s[t]==0)
					p=i.a;
				t=t-1;
				break;
			case jpcelse:
				if(temp==1)
					p=i.a;
					t=t-1;
				break;
			}
		}while(p!=0);
		printf("\n******End CX********\n");

		return;
}
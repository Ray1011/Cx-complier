%{
#include "code.h"
#include "malloc.h"
#include "stdio.h"
#include "stdlib.h"
int yylex();
void redirectInput(FILE *input);
void yyerror(const char *s);
%}

%union{
char *ID;
int NUM;
}

%token SYM_if SYM_else SYM_while SYM_int SYM_bool SYM_true SYM_false //reserved words
%token SYM_write SYM_read//io opration
%token SYM_plus SYM_minus SYM_times SYM_slash SYM_reminder//Arithmetic opr
%token SYM_lparen SYM_rparen SYM_lbraces SYM_rbraces SYM_lcomment SYM_rcomment//reserved 
%token SYM_gt SYM_lss SYM_gte SYM_lte SYM_eql SYM_neq SYM_become SYM_or SYM_and SYM_not SYM_semicolon //logical opr

%token <ID> SYM_ident
%token <NUM> SYM_number

%type <ID> aid


%%

//	rules section

program:
	{lev--;}
	block
	;

block:
	{
		lev ++;
		dx=3;
	if(lev>LEVMAX)
		error(32);
	}
	SYM_lbraces
	decls{
		gen(ini,0,dx);
	}
	stmts
	SYM_rbraces{lev--;}
	;

decls:
	|decl decls
	;

decl:SYM_int aid SYM_semicolon
	|SYM_bool aid SYM_semicolon
	;

aid:SYM_ident
	;


stmts:
	|stmt stmts
	;

stmt:aid SYM_become aexpr SYM_semicolon
	|SYM_if SYM_lparen aexpr SYM_rparen stmts elseStmt SYM_semicolon
	|SYM_while SYM_lparen aexpr SYM_rparen stmts SYM_semicolon
	|SYM_write aexpr SYM_semicolon
	|SYM_read aid SYM_semicolon
	|block
	;

elseStmt:
	|SYM_else stmts;

aexpr:aterm SYM_plus aterm{gen(opr,0,2);}
	|aterm SYM_minus aterm{gen(opr,0,3);}
	|aexpr SYM_or aterm{gen(opr,0,16);}
	|aterm
	;

aterm:afactor SYM_times afactor{gen(opr,0,4);}
	|afactor SYM_slash afactor{gen(opr,0,5);}
	|afactor SYM_reminder afactor{gen(opr,0,21);}
	|aterm SYM_and afactor{gen(opr,0,15);}
	|afactor
	;

afactor:aid
	|SYM_number
	|SYM_lparen aexpr SYM_rparen
	|SYM_true{gen(opr,0,19);}
	|SYM_false{gen(opr,0,20);}
	|SYM_not afactor{gen(opr,0,14);}
	|rel
	;


rel:aid SYM_lss aexpr{gen(opr,0,10);}
	|SYM_number SYM_lss aexpr
	|aid SYM_lte aexpr{gen(opr,0,13);}
	|SYM_number SYM_lte aexpr
	|aid SYM_gt aexpr{gen(opr,0,12);}
	|SYM_number SYM_gt aexpr
	|aid SYM_gte aexpr{gen(opr,0,11);}
	|SYM_number SYM_gte aexpr
	|aid SYM_eql aexpr{gen(opr,0,8);}
	|SYM_number SYM_eql aexpr
	|aid SYM_neq aexpr{gen(opr,0,9);}
	|SYM_number SYM_neq aexpr
	;

%%

/////////////////////////////////////////////////////////////////////////////
// programs section

void yyerror(const char *s)
{
  	err++;
	printf("%s in line %d\n",s,line);
}

int main()
{	
	printf("Input file?\n");
	scanf("%s",fname);

	if((fin=fopen(fname,"r"))==NULL){
		printf("Cann't open file according to given filename\n");
		exit(0);
	}
	redirectInput(fin);
	printf("List object code?[y/n]");
	scanf("%s",fname);

	if(fname[0]=='y')
		listswitch=true;
	else
		listswitch=false;
	err=0;
	cx=0;
	
	yyparse();

	if(err==0)
		{
		listcode();
		interpret();
		}
	else
		printf("%d errors in this program\n",err);

	fclose(fin);
	getchar();
    return 0;}
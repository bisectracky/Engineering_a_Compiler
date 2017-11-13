%{
#include<stdio.h>
#include<stdlib.h>

int yylval;
int line=1;

enum yytokentype {
  NUMBER = 258,
  INT,
  STR,
  VOID,
  ASSIGN,
  ID,
  IF,
  ELSE,
  WHILE,
  RETURN,
  PRINT,
  SCAN,
  CMP,
  EOL,
  LT,
  LE,
  EQ,
  NE,
  GT,
  GE,
  FUNC,
  NEGA
};


%}

DIGIT [0-9]
ID [A-Za-z_][A-Za-z0-9_]*
SPACE [ \t]+

SEMI ";"
COMMA ","
ASSIGNOP "="
PRE-TREATMENT ^#.* 
COMMENT "/""/".*

STR ["][^"]*["]
LP \(
RP \)
LB \[
RB \]
LC \{
RC \}

%%

{DIGIT}+  {yylval=atoi(yytext); return NUMBER;}

{STR} {return STR;}
 
"<" {yylval=LT; return CMP;}
"<=" {yylval=LE; return CMP;}
"==" {yylval=EQ; return CMP;}
">" {yylval=GT; return CMP;}
">=" {yylval=GE; return CMP;}
"<>"|"!="  {yylval=NE; return CMP;}

{ASSIGNOP} |
{COMMA}	   |
{SEMI}     |
{LP}       |
{RP}       |
{LB}       |
{RB}       |
{LC}       |
{RC}      {
    return yytext[0];
}

if {return IF;}
else {return ELSE;}
while {return WHILE;}
return { return RETURN;}
print { return PRINT;}
scan { return SCAN;}
int {return INT;}
str {return STR;}
void {return VOID;}

"+"|"-"|"*"|"/"|"%" {return yytext[0];} 

{COMMENT} {}
{PRE-TREATMENT} {}
{SPACE}  {}
{ID} {return ID;}
[\n\r] {line++;}

%%

int yywrap(){
	return 1;
}


int yyerror(char *s){
	fprintf(stderr, "error: %s\n", s);
	return 0;
}


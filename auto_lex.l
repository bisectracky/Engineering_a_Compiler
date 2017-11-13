%{

#include<stdio.h>
#include<stdlib.h>

%}

DIGIT [+-]？[0-9]
ID [A-Za-z_][A-Za-z0-9_]*
RELOP [>]|[<]|[>][=]|[<][=]|[=][=]|[!][=]
SPACE [ \t\n]+
INDENT ^[ \t]
AND [&]
OR	[|]
Backlash \\
LP \(
RP \)
LB \[
RB \]
LC \{
RC \}
DOT "."
SEMI ";"
COMMA ","
ASSIGNOP "="
COLON ":"
KEYWORD and|as|assert|break|class|continue|def|del|elif|else|except|exec|finally|for|from|global|if|import|in|is|lambda|not|or|pass|print|raise|return|try|while|with|yield
STR ([\"][^"]*[\"]|[\'][^']*[\'])

%%
{SEMI} {
    printf("<Semmi : %s >\n", yytext);
}
{COLON} {
	printf("<COLON : %s >\n",yytext);
}
{COMMA} {
    printf("<Comma : %s >\n", yytext);
}
{STR} {
	printf("<Str : %s >\n", yytext);
}
{Backlash} {
	printf("<Backlash : %s >\n", yytext);
}
{ASSIGNOP} {
    printf("<Assignop : %s >\n", yytext);
}
{DIGIT}+   {
	printf("<Integer : %s >\n",yytext);   
}
{DIGIT}+"."{DIGIT}*  { printf("<Float : %s >\n",yytext);}

{SPACE} 

{KEYWORD}   { printf("<Keyword : %s >\n",yytext); }
"+"|"-"|"*"|"/"|"%"|"^" { printf("<Operator : %s >\n",yytext); }

"#".*

\'\'\'(.|\n)*\'\'\'	 { }
\"\"\"(.|\n)*\"\"\"  { }

{RELOP} 	{ printf("<Relop : %s >\n",yytext);}

{AND}|{OR} { printf("<Logic Operator : %s >\n",yytext); }

{LP} |
{RP} |
{LB} |
{RB} |
{LC} |
{RC} {
    printf("<Brackets : %s >\n", yytext);
}
{DOT} { printf("<Dot : %s >\n",yytext);}
{ID}  	{ 
	printf("<ID : %s >\n",yytext); 
}

%%

int yywrap()

{
    return 1;
}


int main(int argc, char** argv) {
   if (argc > 1) {
       if (!(yyin = fopen(argv[1], "r"))) {   
           perror(argv[1]);
           return 1;
       }
   }
   while (yylex());
   return 0;
  }
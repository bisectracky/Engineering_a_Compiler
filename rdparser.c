#include<stdio.h>
#include<stdlib.h>
#include<string.h>

extern int yylval;
extern char* yytext;
extern int yylex();
FILE* yyin;
int tok;

enum{
	INT=258,STR,VOID,NUMBER,ID,
	OP,WHILE,IF,ELSE,RETURN,STRING,
	PUNC,PRINT,ASSIGN,CMP,SCAN,EQU
};

typedef struct node{
	int ivalue;
	char* svalue;
	char* type;
	struct node* left;
	struct node* mid;
	struct node* right;
}ast,*past;

char* stringcp(char* src);
void next();
void Error();
int sym(char check,int mode);
past newnode();
past newIf();
past newWhile();
past newReturn();
past newPrint();
past newScan();
past newID();
past newNum();
past newStr();
past newVoid();

past program();
past external_declaration();
past decl_or_stmt();
past declarator_list();
past intstr_list();
past initializer();
past declarator();
past parameter_list();
past parameter();
past type();
past statement();
past statement_list();
past expression_statement();
past expr();
past cmp_expr();
past add_expr();
past mul_expr();
past primary_expr();
past expr_list();
past id_list();

char* stringcp(char *src){
	int n=strlen(src),i=0;
	char *s=(char*)malloc(sizeof(char)*n);
	strcpy(s,src);
	return s;
}

void strcp(char *dest,char *src){
	dest=stringcp(src);
	return;
}

past newIf(past condition,past s1,past s2){
	past l=newnode();
	l->type="if_else";
	l->left=condition;
	l->mid=s1;
	l->right=s2;
	return l;
}

past newWhile(past condition,past s){
	past l=newnode();
	l->type="while";
	l->left=condition;
	l->right=s;
	return l;
}

past newReturn(past expr){
	past l=newnode();
	l->type="return";
	l->left=expr;
	return l;
}

past newPrint(past exprlist){
	past l=newnode();
	l->type="print";
	l->left=exprlist;
	return l;
}

past newScan(past exprlist){
	past l=newnode();
	l->type="scan";
	l->left=exprlist;
	return l;
}

past newInt(){
	past l=newnode();
	l->type="int";
	return l;
}

past newStr(){
	past l=newnode();
	l->type="string";
	return l;
}

past newVoid(){
	past l=newnode();
	l->type="void";
	return l;
}

past newNUMBER(int value){
	past l=newnode();
	l->type="NUMBER";
	l->ivalue=value;
	return l;
}

past newSTRING(){
	past l=newnode();
	l->type="STRING";
	l->svalue=stringcp(yytext);
	return l;
}

past newID(){
	past l=newnode();
	l->type="ID";
	l->svalue=stringcp(yytext);
	return l;
}

past newnode(){
	past p=(past)malloc(sizeof(struct node));
	p->ivalue=-1;
	p->svalue="#";
	p->type="#";
	p->left=NULL;
	p->mid=NULL;
	p->right=NULL;
	return p;
}

void next(){
	tok=yylex();
	return;
}

void Error(){
	printf("Error in text:\"%s\"\n",yytext);
	exit(0);
}

int sym(char check,int mode){
	if(check==yytext[0]){
		next();
		return 0;
	}
	if(mode){
		Error();
	}
	return 1;
}

past type(){
	past p=NULL;
	if(tok==INT){
		next();
		p=newInt();
		return p;
	}
	if(tok==STR){
		next();
		p=newStr();
		return p;
	}
	if(tok==VOID){
		next();
		p=newVoid();
		return p;
	}
	Error();
}

past id_list(){
	past p=NULL,q=NULL,l;
	if(tok!=ID){
		Error();
	}
	p=newID();

	sym(',',1);
	if(tok!=ID){
		Error();
	}
	q=newID();

	l=newnode();
	l->type="id_list";
	l->left=p;
	l->right=q;
	return l;
}

past expr_list(){
	past p=expr(),q=NULL;
	past l=newnode();
	l->type="expr_list";
	l->left=p;
	if(!sym(',',0)){
		q=expr_list();
		l->mid=q;
		return l;
	}
	return l;
}

past array(){
	past p=NULL,q=NULL;
	if(tok!=ID){
		Error();
	}
	p=newID();

	sym('[',1);
	q=expr();
	sym(']',1);

	past l=newnode();
	l->type="array";
	l->left=p;
	l->mid=q;
	return l;
}

past primary_expr(){
	past p,l=newnode();
	l->type="primary_expr";
	switch(tok){
		case NUMBER:case STRING:{
				if(tok==NUMBER){
					p=newNUMBER(atoi(yytext));
				}
				else{
					p=newSTRING();
				}
				l->left=p;
				next();
				return l;
			}
			break;
		case ID:{
				p=newID();
				l->left=p;
				next();
				if(!sym('(',0)){
					if(!sym(')',0)){
						return l;
					}
					past q=expr_list();
					sym(')',1);
					l->mid=q;
					return l;
				}
				else{
					if(tok==ASSIGN){
						next();
						past q=expr();
						l->mid=q;
						l->type="ASSIGN";
						return l;
					}
					else{
						return l;
					}
				}
			}
			break;
		default:{
				if(!sym('(',0)){
					p=expr();
					sym(')',1);
					l->left=p;
					return l;
				}
				else{
					p=array();
					l->left=p;
					
                    if(tok!=ASSIGN){
						return l;
					}
					
                    past q=newnode();
					q->type="ASSIGN";
					l->mid=q;
					next();
					past r=expr();
					l->right=r;
					return l;
				}
			}
	}
}

past mul_expr(){
	past p=NULL,q=NULL,l=newnode();
	l->type="mulExpr";
	if(!sym('-',0));
	p=primary_expr();
	l->left=p;
	if(yytext[0]=='*'||yytext[0]=='/')
	{
		char tmp=yytext[0];
		next();
		q=mul_expr();
		l->mid=q;
		if(tmp=='*'){
			l->svalue="*";
		}
		if(tmp=='/'){
			l->svalue="/";
		}
		return l;
	}
	return l;
}

past add_expr(){
	past p=mul_expr(),q=NULL,l=newnode();
	l->left=p;
	l->type="addExpr";
	if(yytext[0]=='+'||yytext[0]=='-'){
	    char tmp=yytext[0];
	    next();
		q=add_expr();
		if(tmp=='+'){
			q->svalue="+";
		}
		if(tmp=='-'){
			q->svalue="-";
		}
		l->mid=q;
		return l;
	}
	return l;
}

past cmp_expr(){
	past p=add_expr(),q=NULL,l=newnode();
	l->left=p;
	l->type="cmpExpr";
	char *cmp;
	if(tok==CMP||tok==EQU){
		if(yytext[0]=='>'){
			cmp=">";
		}
		if(yytext[0]=='<'){
			cmp="<";
		}
		next();
		q=cmp_expr();
		l->mid=q;
		l->svalue=stringcp(cmp);
		return l;
	}
	return l;
}

past expr(){
	past p=cmp_expr(),l=newnode();
	l->type="type";
	l->left=p;
	return l;
}

past expr_statement(){
	past p=NULL,l=newnode();
	l->type="expr_statement";
	if(sym(';',0)){
		p=expr();
		l->left=p;
		return l;
	}
	return NULL;
}

past statement_list(){
	past p=statement(),q=NULL,l=newnode();
	l->type="statement_list";
	l->left=p;
	if(yytext[0]!='}'){
		q=statement_list();
		l->mid=q;
		return l;
	}
	return l;
}

past statement(){
	past p=NULL,q=NULL,r=NULL;
	past l=newnode();
	switch(tok){
		case IF:{
				next();
				sym('(',1);
				p=expr();
				sym(')',1);
				q=statement();;
				if(tok==ELSE){
					next();
					r=statement();;
					return newIf(p,q,r);
				}
				return newIf(p,q,r);
			}
			break;
		case WHILE:{
				next();
				sym('(',1);
				p=expr();
				sym(')',1);
				q=statement();
				return newWhile(p,q);
			}
			break;
		case RETURN:{
				next();
				if(sym(';',0)){
					p=expr();
					sym(';',1);
					return newReturn(p);
				}
				return newReturn(NULL);
			}
			break;
		case PRINT:{
				next();
				if(sym(';',0)){
					p=expr_list();
					sym(';',1);
					return newPrint(p);
				}
				return newReturn(NULL);
			}
			break;
		case SCAN:{
				next();
				p=id_list();
				sym(';',1);
				return newScan(p);
			}
			break;
		default:{
				if(!sym('{',0)){
					p=statement_list();
					l->left=p;
					sym('}',1);
					return l;
				}
				else{
					if(tok==INT||tok==STR||tok==VOID){
						next();
						p=declarator_list();
						sym(';',1);
						return p;
					}
					else{
						p=expr_statement();
						return p;
					}
				}
			}
	}
}

past parameter(){
	past p=type(),q=NULL,l=newnode();
	l->type="parameter";
	l->left=p;
	if(tok==ID){
		q=newID();
		l->mid=q;
		next();
		return l;
	}
	else{
		Error();
	}
}

past parameter_list(){
	past p=parameter(),q=NULL,l=newnode();
	l->type="parameter_list";
	l->left=p;
	if(!sym(',',0)){
		q=parameter_list();
		l->mid=q;
	}
	return l;
}

past declarator(){
	past p=NULL,q=NULL,r=NULL,l=newnode();
	if(tok!=ID){
		Error();
	}
	p=newID();
	l->left=p;
	next();
	switch(yytext[0]){
		case '=':{
				q=newnode();
				q->type="assign";
				q->svalue="=";
				l->mid=q;
				next();
				r=expr();
				l->right=r;
				l->type="declarator";
				return l;
			}
			break;
		case '(':{
				next();
				if(!sym(')',0)){
					l->type="functionID";
					return l;
				}
				else{
					q=parameter_list();
					l->mid=q;
					l->type="functionID";
					sym(')',1);
					return l;
				}
			}
			break;
		case '[':{
				l->type="array";
				if(!sym(']',0)){
					if(!sym('=',0)){
						q=newnode();
						q->type="assign";
						q->svalue="=";
						l->mid=q;
						sym('{',1);
						r=intstr_list();
						l->right=r;
						sym('}',1);
						return l;
					}
					next();
					return l;
				}
				next();
				q=expr();
				l->mid=q;
				sym(']',1);
				return l;
			}
			break;
		default:{
				l->type="declarator";
				return l;
			}
	}
}

past intstr_list(){
	past p=initializer(),q=NULL,l=newnode();
	l->type="iststrlist";
	l->left=p;
	if(!sym(',',0)){
		q=intstr_list();
		l->mid=q;
	}
	return l;
}

past initializer(){
	past p=NULL;
	if(tok==NUMBER){
		next();
		p=newInt();
		next();
		return p;
	}
	else if(tok==STR){
		next();
		p=newStr(stringcp(yytext));
		next();
		return p;
	}
	Error();
}

past decl_or_stmt(){
    past p=NULL;
	switch(yytext[0]){
		case '{':{
				next();
				if(sym('}',0)){
					p=statement_list();
					sym('}',1);
				}
				else{
					sym('}',1);
				}
        }
			break;
		case ',':{
				next();
				past p=declarator_list();
				sym(';',1);
        }
			break;
		case ';':
			next();
			break;
		default:
			Error();
	}
	return p;
}

past declarator_list(){
	past p=declarator(),q=NULL,l=newnode();
	l->type="declarator_list";
	l->left=p;
	if(!sym(',',0)){
		q=declarator_list();
		l->mid=q;
	}
	return l;
}

past exter_declaration(){
	past p=type(),q=declarator(),r=decl_or_stmt(),l=newnode();
	l->type="exter_declaration";
	l->left=p;
	l->mid=q;
	l->right=r;
	return l;
}

past expr_dec_list(){
	past p=exter_declaration(),q=NULL,l=newnode();
	l->type="experDeclarationList";
	l->left=p;
	if(tok){
		q=expr_dec_list();
		l->mid=q;
	}
	return l;
}

past program(){
	past p=exter_declaration(),q=expr_dec_list(),l=newnode();
	l->type="program";
	l->left=p;
	l->mid=q;
	return l;
}

void showAst(FILE* fp,past p,int nest){
	if(p==NULL){
		return;
	}
	char *t=" ";
	int n=nest,i=0;
	for(i=0;i<n;i++){
		fprintf(fp,"%s",t);
	}
	if(p->ivalue==-1){
		fprintf(fp,"%s %s\n",p->type,p->svalue);
	}
	else{
		fprintf(fp,"%s %d\n",p->type,p->ivalue);
	}
	showAst(fp,p->left,nest+1);
	showAst(fp,p->mid,nest+1);
	showAst(fp,p->right,nest+1);
	return;
}

int main(int argc,char* argv[]){
	yyin=fopen(argv[1],"r");
	FILE* fp=fopen(argv[2],"w+");
	if(!yyin||!fp){
		printf("Error to open file!\n");
		exit(0);
	}
	next();
	past rr=program();
	showAst(fp,rr,0);
	printf("program doesn't have any mistake!\n");
	fclose(yyin);
	fclose(fp);
	return 0;
}

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define MAX_LEN 100
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 999

int charClass;
char lexeme[MAX_LEN];
char nextChar;
char next2Char;
char next3Char;
int lexLen;
int token;
int nextToken;

FILE *inFile;

enum{
	AND=258,AS,ASSERT,BREAK,CLASS,CONTINUE,DEF,DEL,ELIF,ELSE,EXCEPT,EXEC,FINALLY,
	FOR,FROM,GLOBAL,IF,IMPORT,IN,IS,LAMBDA,NOT,OR,PASS,PRINT,RAISE,RETURN,TRY,
	WHILE,WITH,YIELD,EQU,GEQU,LEQU,DEQU,ERROR,RET,ID,INT
};

char* keywords[]={
                  "and","as","assert","break","class","continue","def","del","elif","else","except"
                  "exec","finally","for","from","global","if","import","in","is","lambda","not","or"
                  "pass","print","raise","return","try","while","with","yield",0
                  };

void addChar(){
	if(lexLen<=MAX_LEN-2){
			lexeme[lexLen++]=nextChar;
			lexeme[lexLen]=0;
	}
	else
		printf("ERROR:lexeme is too long. \n");
}

void getChar(){
	static int firstRun=1;
	if(firstRun){
		nextChar=getc(inFile);
		next2Char=getc(inFile);
		next3Char=getc(inFile);
		firstRun=0;
	}
	else{
		nextChar=next2Char;
		next2Char=next3Char;
		next3Char=getc(inFile);
	}

	if(nextChar=='#'){
		while(nextChar!='\n'){
		nextChar=next2Char;
		next2Char=next3Char;
		next3Char=getc(inFile);
		}
			
	}
	if(nextChar=='\''&&next2Char=='\''&&next3Char=='\''){
		nextChar=next2Char;
		next2Char=next3Char;
		next3Char=getc(inFile);
		while(!(nextChar=='\''&&next2Char=='\''&&next3Char=='\'')){
			nextChar=next2Char;
			next2Char=next3Char;
			next3Char=getc(inFile);
		}
	}
	if(nextChar==EOF){
		charClass=EOF;
	}
	else{
		if(isalpha(nextChar))
			charClass=LETTER;
		else if(isdigit(nextChar))
			charClass=DIGIT;
		else
			charClass=UNKNOWN;
	}
}

void getNoBlank(){
	while(isspace(nextChar))
		getChar();
}

int checkSymbol(char ch,char nextCh){
	switch(ch){
		case '(':
		case ')':
		case ';':
		case '+':
		case '-':
		case '[':
		case ']':
		case '.':
		case '"':
		case ':':
		case '/':
		case '\'':
		case '*':
		case '_':
		case ',':
		case '?':
			addChar();
			nextToken=ch;
			break;
		case '=':
			addChar();
			nextToken=ch;
			if(nextCh=='='){
				getChar();
				addChar();
				nextToken=EQU;
			}
			break;
		case '>':
			addChar();
			nextToken=ch;
			if(nextCh=='='){
				getChar();
				addChar();
				nextToken=GEQU;
			}
			break;
		case '<':
			addChar();
			nextToken=ch;
			if(nextCh=='='){
				getChar();
				addChar();
				nextToken=LEQU;
			}
			break;
        case '!':
            addChar();
            nextToken=ch;
            if(nextCh=='='){
                getChar();
                addChar();
                nextToken=DEQU;

            }
            break;
		case EOF:
			addChar();
			nextToken=EOF;
		default:
			printf("ERROR:unknown character'%c'. \n",ch);
			nextToken=ERROR;
	}
	return nextToken;
}

void checkKeyword(char* pword){
	int i=0;
	while(keywords[i]!=0){
		char* pkeyword=keywords[i];
		if(strcmp(pword,pkeyword)==0){
			nextToken=258+i;
			return;
		}
		i++;
	}
}

int lexer(){
	lexLen=0;
	getNoBlank();
	switch(charClass){
		case LETTER:
			addChar();
			getChar();
			while(charClass==LETTER||charClass==DIGIT){
				addChar();
				getChar();
			}
			nextToken=ID;
			break;
		case DIGIT:
			addChar();
			getChar();
			while(charClass==DIGIT){
				addChar();
				getChar();
			}
			nextToken=INT;
			break;
		case UNKNOWN:
			checkSymbol(nextChar,next2Char)	;
			getChar();
			break;
		case EOF:
			nextToken=EOF;
			lexeme[0]='E';
			lexeme[1]='O';
			lexeme[2]='F';
			lexeme[3]=0;
			break;
	}
	printf("<%6d,%s>\n",nextToken,lexeme);
	return nextToken;
}

int main(int argc, char *argv[]){
	if(argc!=2){
		printf("ERROR:input file name is needed.\n");
		system("pause");
		exit(0);
	}
	inFile=fopen(argv[1],"r");
	if(inFile==NULL){
		printf("ERROR:can not open file.\n");
		exit(0);
	}
	getChar();
	while(nextToken!=EOF)
		lexer();
}


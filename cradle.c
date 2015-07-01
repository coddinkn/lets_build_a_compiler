#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char look = '\0';

void getChar()
{
	look = getchar();		
}

void error(char* s)
{
	printf("error: %s\n", s);
}

void abortCompilation(char* s)
{
	error(s);
	exit(1);
}

void expected(char* s)
{
	abortCompilation(strcat(s, " expected\n"));
}

void match(char x)
{
	if (look == x)
	{
		getChar();
	}
	else
	{
		char* s = "'X'";
		s[1] = x;
		expected(s);
	}
}

char getName()
{
	if(!isalpha(look))
	{
		expected("name");
	}
	char c = look - 32;
	getChar();
	return c;
}

char getNum()
{
	if(!isdigit(look))
	{
		expected("integer");
	}
	char n = look;
	getChar();
	return n;
}

void emit(char* s)
{
	printf("\t%s", s);
}

void init()
{
	getChar();
}

int main(int argc, char** argv)
{
	init();
	exit(0);
}

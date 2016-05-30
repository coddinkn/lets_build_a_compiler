#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

void expression();

char look = '\0';

void getChar()
{
	do
    {
        look = getchar();		
    } while(isspace(look));
}

void error(char* s)
{
	printf("error: %s\n", s);
}

void halt(char* s)
{
	error(s);
	exit(1);
}

void expected(char* s)
{
	char t[80];
	snprintf(t, 80, "%s expected", s);
	halt(t);
}

void match(char x)
{
	if (look == x)
	{
		getChar();
	}
	else
	{
		char s[80];
		snprintf(s, 80, "'%c'", x);
		expected(s);
	}
}

int isaddop(char c)
{
    return (c == '+') || (c == '-');
}

int ismulop(char c)
{
    return (c == '*') || (c == '/');
}

char getName()
{
	if(!isalpha(look))
	{
		expected("name");
	}
	char c = toupper(look);
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

void emitLine(char* s)
{
	emit(s);
	printf("\n");
}

void init()
{
	getChar();
}

void factor()
{
    if(look == '(')
    {
        match('(');
        expression();
        match(')');
    }
    else
    {
        char s[80];
        snprintf(s, 80, "mov $%c, %%rax", getNum());
        emitLine(s);
    }
}

void multiply()
{
    match('*');
    factor();
    emitLine("pop %rbx");
    emitLine("mul %rbx");
}

void divide()
{
    match('/');
    factor();
    emitLine("pop %rdx");
    emitLine("mov %rax, %rbx");
    emitLine("mov %rdx, %rax");
    emitLine("xor %rdx, %rdx");
    emitLine("div %rbx");
}

void term()
{
    factor();
    while(ismulop(look))
    {   
    	emitLine("push %rax");
        switch(look)
        {
            case '*':
                multiply();
                break;
            case '/':
                divide();
                break;
            default:
                expected("mulop");
                break;
        }
    }
}

void add()
{
	match('+');
	term();
	emitLine("pop %rbx");
	emitLine("add %rbx, %rax");
}

void subtract()
{
	match('-');
	term();
	emitLine("pop %rbx");
	emitLine("sub %rbx, %rax");
	emitLine("neg %rax");
}

void expression()
{
    if(isaddop(look))
        emitLine("xor %rax, %rax");
    else
        term();
    while(isaddop(look))
    {
        emitLine("push %rax");
        switch(look)
        {
            case '+':
                add();
                break;
            case '-':
                subtract();
                break;
            default:
                expected("addop");
                break;
        }
    }
}

int main(int argc, char** argv)
{
	init();
	expression();
	exit(0);
}

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

void error(char* string)
{
	printf("error: %s\n", string);
}

void halt(char* string)
{
	error(string);
	exit(1);
}

void expected(char* string)
{
	char error[80];
	snprintf(error, 80, "%s expected", string);
	halt(error);
}

void match(char character)
{
	if (look == character)
	{
		getChar();
	}
	else
	{
		char string[80];
		snprintf(string, 80, "'%c'", character);
		expected(string);
	}
}

int isaddop(char character)
{
    return (character == '+') || (character == '-');
}

int ismulop(char character)
{
    return (character == '*') || (character == '/');
}

char getName()
{
	if(!isalpha(look))
	{
		expected("name");
	}
	char name = toupper(look);
	getChar();
	return name;
}

char getNum()
{
	if(!isdigit(look))
	{
		expected("integer");
	}
	char number = look;
	getChar();
	return number;
}

void emit(char* string)
{
	printf("%s", string);
}

void emitLine(char* string)
{
	emit(string);
	printf("\n");
}

void init()
{
	getChar();
    emitLine(".text");
    emitLine(".globl _start");
    emitLine("_start:");
}

void finish()
{
    emitLine("mov %rax, %rdi");
    emitLine("mov $60, %rax");
    emitLine("syscall");
}

void assignment()
{
    char name = getName();
    char string[80];
    match('=');
    expression();
    snprintf(string, 80, ".comm %c,4,4", name);
    emitLine(string);
    snprintf(string, 80, "lea %c(%%rip), %%rbx", name);
    emitLine(string);
    emitLine("mov %rax, (%rbx)");
}


void identify()
{
    char name = getName();
    char string[80];
    if(look == '(')
    {
        match('(');
        match(')');
        snprintf(string, 80, "jmp %c", name);
        emitLine(string);
    }
    else
    {
        snprintf(string, 80, "mov %c(%%rip), %%rax", name);
        emitLine(string);
    }
}

void factor()
{
    if(look == '(')
    {
        match('(');
        expression();
        match(')');
    }
    else if(isalpha(look))
    {
        identify();
    }
    else
    {
        char string[80];
        snprintf(string, 80, "mov $%c, %%rax", getNum());
        emitLine(string);
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
    assignment();
    finish();
	exit(0);
}

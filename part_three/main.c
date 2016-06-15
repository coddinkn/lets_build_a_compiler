#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

void expression();

char look = '\0';

int iswhitespace(char character)
{
    return (character == ' ') || (character == '\t');
}

void getChar()
{
	do
    {
        look = getchar();		
    } while(iswhitespace(look));
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

char* getName()
{
    if(!isalpha(look))
	{
		expected("name");
	}
	char* name = malloc(sizeof(char) * 40);
    int letter = 0;
    while(isalnum(look) && letter < 40)
    {
        name[letter] = look;
        getChar();   
        letter++;
    }
    name[letter] = '\0';
    if(letter == 40 && isalnum(look))
    {
        halt("name too long");      
    }
	return name;
}

char* getNum()
{
	if(!isdigit(look))
	{
		expected("number");
	}
	char* number = malloc(sizeof(char) * 40);
    int digit = 0;
    while(isdigit(look) && digit < 40)
    {
        number[digit] = look;
        getChar();   
        digit++;
    }
    number[digit] = '\0';
    if(digit == 40 && isdigit(look))
    {
        halt("number too large");      
    }
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
    char* name = getName();
    char string[80];
    match('=');
    expression();
    snprintf(string, 80, ".comm %s,4,4", name);
    emitLine(string);
    snprintf(string, 80, "lea %s(%%rip), %%rbx", name);
    emitLine(string);
    emitLine("mov %rax, (%rbx)");
    free(name);
}


void identify()
{
    char* name = getName();
    char string[80];
    if(look == '(')
    {
        match('(');
        match(')');
        snprintf(string, 80, "jmp %s", name);
    }
    else
    {
        snprintf(string, 80, "mov %s(%%rip), %%rax", name);
    }
    emitLine(string);
    free(name);
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
        snprintf(string, 80, "mov $%s, %%rax", getNum());
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

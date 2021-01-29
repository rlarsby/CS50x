#include <stdio.h>
#include <cs50.h>

// asks for name and then says hello "name"
int main(void)
{
    string name = get_string("What is your name?\n");
    printf("hello, %s\n", name);
}
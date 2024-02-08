#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int subtract (int, int);
int add (int, int);

int main() {
    int flag = O_WRONLY | O_APPEND;
    int fd = open("test.txt", flag);
    char buffer[] = "I am appending to the end.";
    write(fd, buffer, sizeof(buffer) - 1);
    return 0;
}

int subtract(int a, int b)
{
    return a - b;
}

int add(int a, int b)
{
    return a + b;
}
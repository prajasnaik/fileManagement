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
int TestFunction(ssize_t (*func) (int, void *, size_t), void *buffer, int flag, char *filePath);
int Rename(char *oldFileName, char *newFileName);

int main() {
    // int flag = O_WRONLY | O_APPEND;
    // //char buffer[] = "I am appending to the end.";
    // int buffer2[] = {1, 2, 3, 4, 5, 6};
    // int error = TestFunction(&write, buffer2, flag, "test.txt");
    // int fd = open("test.txt", O_RDONLY);
    // int error = 4;
    // int buffer[1];
    // while(error != 0)
    // {
    //     error = read(fd, buffer, 4);
    //     printf("%d", buffer[0]);
    // }

    Rename("test.txt", "new-text.txt");

    return 0;
}

int TestFunction(ssize_t (*func) (int, void *, size_t), void *buffer, int flag, char *filePath)
{
    int fd = open(filePath, flag);
    int error = (*func) (fd, buffer, 24);

    return error;
}

int Rename(char *oldFileName, char *newFileName)
{
    int status = link(oldFileName, newFileName);
    status = unlink(oldFileName);
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
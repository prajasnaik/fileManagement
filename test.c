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
int rename2(char *oldFileName, char *newFileName);


int main() {
    // int flag = O_WRONLY | O_APPEND;
    // char buffer[] = "I am appending to the end.";
    // // int buffer2[] = {1, 2, 3, 4, 5, 6};
    // int error = TestFunction(&write, buffer, flag, "my_fm");
    printf("%d", sizeof(unsigned short int));
    // while(error != 0)
    // {
    //     error = read(fd, buffer, 4);
    //     printf("%d", buffer[0]);
    // }

    // int ec = rename2("/home/prajas/C_C++/Systems Programming/Assignment-2/test1", "/home/prajas/C_C++/Systems Programming/Assignment-2/test2");

    return 0;
}

int TestFunction(ssize_t (*func) (int, void *, size_t), void *buffer, int flag, char *filePath)
{
    int fd = open(filePath, flag);
    printf("%d", errno);
    int error = (*func) (fd, buffer, 24);

    return error;
}

int Rename(char *oldFileName, char *newFileName)
{
    int status = link(oldFileName, newFileName);
    status = errno;
    status = unlink(oldFileName);
    return 0;
}

int rename2(char *oldFileName, char *newFileName)
{
    int status = rename(oldFileName, newFileName);
    
    return errno;
}

int subtract(int a, int b)
{
    return a - b;
}

int add(int a, int b)
{
    return a + b;
}
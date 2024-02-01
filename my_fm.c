#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#define E_OK    0

int ec = E_OK;


int ProcessCommandLine (char **);
int CheckDirectory(char *);


int main(int argc, char *argv[])
{
    return ec;
}


int ProcessCommandLine(char *commandLineArguments[])
{
    int argno = 1;
    while(argno < 5)
    {
        switch (commandLineArguments[argno][1])
        {
        case 'c':
            argno += 2;
            break;
        
        case 'w':
            argno += 2;
            break;
        case 'd':
            argno += 2;
            break;
        case 'r':
            argno += 2;
            break;
        case 'a':
            argno += 2;
            break;
        default:
            return -1;
            break;
        }
    } 
    return E_OK; 
}

int CheckDirectory(char *filePath)
{
    struct stat *fileInfo;
    int error = stat(filePath, fileInfo);
    if (error == -1)
    {
        ec = errno;
    }
    else 
    {
        if(S_ISDIR(fileInfo->st_mode))
        {
            return -1;
        }
        else return 0;
    }

    return 0;
}


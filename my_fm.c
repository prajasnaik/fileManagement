#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define N_BYTES 50
#define MAX_APPEND_SIZE 50

#define E_OK    0
#define E_GENERAL -1

#define IS_FILE 0
#define IS_DIRECTORY 1



int ec = E_OK;

char readBuffer[MAX_APPEND_SIZE];

char writeBuffer[MAX_APPEND_SIZE];


int ProcessCommandLine (char **);
int CheckDirectory(char *);
int NonBlockingOperation(ssize_t (*operation) (int, void *, size_t), int flag, char *filePath, void* buffer);
int AppendOddNumbers(int startNumber, char *filePath);
int AppendText(char *text, char* filePath);
int CreateFile(char *pathName);
int CreateDirectory(char *pathName);
int removeFile(char *filePath);
int removeDirectory(char *directoryPath);


int main(int argc, char *argv[])
{

    ec = ProcessCommandLine(argv);
    return ec;
}


int ProcessCommandLine(char *commandLineArguments[])
{
    int argno = 1;
    while(argno < 2)
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
        ec =  errno;
        return E_GENERAL;
    }
    else 
    {
        if(S_ISDIR(fileInfo->st_mode))
        {
            return IS_DIRECTORY;
        }
        else 
            return IS_FILE;
    }

}


int NonBlockingOperation(ssize_t (*operation) (int, void *, size_t), int flag, char *filePath, void* buffer) //will cause warning with write function as argument, but this is not an issue
{
    int fd;
    if (strcmp(filePath, "stdout") == 0)
        fd = 1;
    else
        fd = open(filePath, flag | O_NONBLOCK);
    int status = E_OK;
    if (fd == E_GENERAL)
    {
        return errno;
    }
    else
    {
        status = (*operation)(fd, buffer, N_BYTES);
        if (status == E_GENERAL)
            status = errno;
        while (status == EAGAIN)
        {
            status = (*operation)(fd, buffer, N_BYTES);
            if (status == -1)
            {
                status == errno;
                if (status != EAGAIN)
                {
                    if (fd <= 2)
                        return status;
                    int error = close(fd);
                    if (error == E_GENERAL)
                        status = errno;
                    return status;
                }
            }
        }
        if (fd <= 2)
            return status;
        status = close(fd);
        if (status == E_GENERAL)
            status = errno;
        return status;
    }
}

int AppendOddNumbers(int startNumber, char *filePath)
{
    int oddNumbers[13];
    int bytesToWrite = 50;
    for (int i = 0; i < 13; i ++)
    {
        if (startNumber > 200)
            {
                bytesToWrite = --i * 4;
                break;
            }
        oddNumbers[i] = startNumber;
        startNumber += 2;
    }
    int status = NonBlockingOperation(&write, O_WRONLY | O_APPEND, filePath, oddNumbers);
    return status;
}

int AppendText(char *text, char* filePath) //Wrapper function
{
    int status = NonBlockingOperation(&write, O_WRONLY | O_APPEND, filePath, text);
    return status;
}

int PrintFirstNBytes(char *fileName)
{
    char buffer[MAX_APPEND_SIZE];
    int status = NonBlockingOperation(&read, O_RDONLY, fileName, buffer);
    if (status != E_OK)
    {
        return status;
    }
    else
    {
        status = NonBlockingOperation(&write, 0, "stdout", buffer);
        return status;
    }
}


int CreateFile(char *pathName)
{
    int fd = creat(pathName, S_IRWXU);
    if (fd == E_GENERAL)
    {
        ec =  errno;
    }
    else 
    {
        return fd;
    }
    
}

int renameFile(char *oldFilePath, char *newFilePath)
{
    if (link(oldFilePath, newFilePath) == E_GENERAL)
    {
        return errno;
    }
    else 
    {
        if (unlink(oldFilePath) == E_GENERAL)
        {
            return errno;
        }
        else return E_OK;
    }
}

int CreateDirectory(char *pathName)
{
    int status = mkdir(pathName, S_IRWXU);
    if (status == E_GENERAL)
    {
        return errno;
    }
    return E_OK;
}

int removeFile(char *filePath)
{
    int status = unlink(filePath);
    if (status == E_GENERAL)
    {
        return errno;
    }
    else 
        return E_OK;
}
int removeDirectory(char *directoryPath)
{
    struct stat *directoryInfo;
    int status = stat(directoryPath, directoryInfo);
    status = rmdir(directoryPath);
    if (status == E_GENERAL)
    {
        return errno;
    }
    return E_OK;
}



#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define N_BYTES 50
#define MAX_APPEND_SIZE 50

#define E_OK    0
#define E_GENERAL -1

#define IS_FILE 0
#define IS_DIRECTORY 1



int ec = E_OK;

char readBuffer[MAX_APPEND_SIZE];

char *writeBuffer;
char *readBuffer;

int ProcessCommandLine (char **);
int CheckDirectory(char *);


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
        return NULL;
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


int NonBlockingOperation(int (*operation)(int, char*, int), int flag, char *filePath, char* buffer)
{
    int fd = open(filePath, flag | O_NONBLOCK);
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
                    int error = close(fd);
                    if (error == E_GENERAL)
                        status = errno;
                    return status;
                }
            }
        }
        status = close(fd);
        if (status == E_GENERAL)
            status = errno;
        return status;
    }
}


int WriteFirstNBytes(char *filePath)
{
    int fd = open(filePath, O_WRONLY | O_NONBLOCK);
    int status = E_OK;
    if (fd == E_GENERAL)
    {
        return errno;
    }
    else
    {
        status = write(fd, writeBuffer, N_BYTES);
        if (status == E_GENERAL)
            status = errno;
        while (status == EAGAIN)
        {
            status = write(fd, writeBuffer, N_BYTES);
            if (status == -1)
            {
                status == errno;
                if (status != EAGAIN)
                {
                    int error = close(fd);
                    if (error == E_GENERAL)
                        status = errno;
                    return status;
                }
            }
        }
        status = close(fd);
        if (status == E_GENERAL)
            status = errno;
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



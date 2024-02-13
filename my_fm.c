// @authors: Prajas Naik and Kalash Shah
// Simple file management program

// Include Statements
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


//Define Statements
#define     N_BYTES                 50
#define     MAX_APPEND_SIZE         50
#define     MAX_BUF_SIZE            100
#define     E_OK                    0
#define     E_GENERAL               -1
#define     IS_FILE                 0
#define     IS_DIRECTORY            1
#define     ENABLE                  1
#define     DISABLE                 0

// Global Variable for Error Code
int         ec          =           E_OK;

// Global Variable Flags for deciding which operation to perform
int         fCreate     =           DISABLE;
int         fWrite      =           DISABLE;
int         fDelete     =           DISABLE;
int         fRename     =           DISABLE;
int         fAppend     =           DISABLE;
int         fBinary     =           DISABLE;
int         fPath       =           DISABLE;
int         fDirectory  =           DISABLE;

// Buffers for storing paths for each function
char        *createPath;
char        *deletePath;
char        *oldPath;
char        *newPath;
char        *appendPath;
char        *writePath;
char        *appendBuffer;

// Buffer for storing values to read and write
char        readBuffer              [MAX_APPEND_SIZE];
char        writeBuffer             [MAX_APPEND_SIZE];

// Function Declarations
int         ProcessCommandLine      (char **, int);
int         CheckDirectory          (char *);
int         NonBlockingOperation    (ssize_t (*) (int, void *, size_t), int, char *, void*, int );
int         AppendOddNumbers        (int, char *);
int         AppendText              (char *, char*);
int         CreateFile              (char *);
int         CreateDirectory         (char *);
int         RemoveFile              (char *);
int         RemoveDirectory         (char *);
int         RenameDirectory         (char *, char *);
int         RenameFile              (char *, char *);
int         PerformOperations       ();
int         PrintFirstNBytes        (char *);

// Main function
int main(int argc, char *argv[])
{
    char *args[] = {"my_fm", "-c", "directory10", "-f", "-a", "test.txt", "Random Stuff"};
    argc = 7;
    ec = ProcessCommandLine(args, argc);
    ec = PerformOperations();
    return ec;
}

// function: ProcessCommandLine
//      This function takes the command line arguments and appropriately sets 
//      flags for which operations need to be performed. It also extracts 
//      appropiate pointers
//  @param: commandLineArguments - Pointer to array containing command line 
//          arguments
//  @param: argCount - Integer count of total  number of command line arguments
//  @return: Integer error code
int ProcessCommandLine(char *commandLineArguments[], int argCount)
{
    int argno = 1;
    while(argno < argCount)
    {
        switch (commandLineArguments[argno][1])
        {
        case 'c':
            fCreate = ENABLE;
            createPath = commandLineArguments[argno + 1];
            argno += 2;
            break;
        case 'w':
            fWrite = ENABLE;
            writePath = commandLineArguments[argno + 1]; 
            argno += 2;
            break;
        case 'd':
            fDelete = ENABLE;
            deletePath = commandLineArguments[argno + 1];
            argno += 2;
            break;
        case 'r':
            fRename = ENABLE;
            oldPath = commandLineArguments[argno + 1];
            newPath = commandLineArguments[argno + 2];
            argno += 3;
            break;
        case 'a':
            fAppend = ENABLE;
            appendPath = commandLineArguments[argno + 1];
            appendBuffer = commandLineArguments[argno + 2]; 
            argno += 3;
            break;
        case 'b':
            fBinary = ENABLE;
            argno += 2;
            break;
        case 'f':
            fDirectory = ENABLE;
            argno += 1;
            break;
        default:
            return -1;
            break;
        }
    } 
    return E_OK; 
}

//  function: PerformOperations
//      This function calls and executes appropriate functions based on
//      which flags were set after processing command line.
//  @param: None
//  @return: Integer Error Code
int PerformOperations()
{
    int status = E_OK;
    
    if (fCreate)
    {
        if(fDirectory)
        {
            status = CreateDirectory(createPath);
            if (status != E_OK)
                return status;
            fDirectory = DISABLE;
        }
        else 
        {
            status = CreateFile(createPath);
            if(status != E_OK)
                return status;
        }
    }
    if (fAppend)
    {
        status = CheckDirectory(appendPath);
        if (status != E_OK)
            return status;

        if (!fDirectory)
        {
            if (fBinary)
            {
                int startNumber = strtol(appendBuffer, NULL, 0);
                AppendOddNumbers(startNumber, appendPath);
                if (status != E_OK)
                    return status;
            }
            else
            {
                status = AppendText(appendBuffer, appendPath);
                if (status != E_OK)
                    return status;
            }
        }
        else
            fDirectory = DISABLE;
    }
    if (fDelete)
    {
        status = CheckDirectory(deletePath);
        if (status != E_OK)
            return status;

        if (fDirectory)
        {
            status = RemoveDirectory(deletePath);
            if (status != E_OK)
                return status;
            fDirectory = DISABLE;
        }
        else
        {
            status = RemoveFile(deletePath);
            if (status != E_OK)
                return status;
        }
    }
    if (fWrite)
    {
        status = CheckDirectory(writePath);
        if (status != E_OK)
            return status;
        if (!fDirectory)
        {
            status = PrintFirstNBytes(writePath);
            if (status != E_OK)
                return status;
        }
        else
            fDirectory = DISABLE;

    }

    if (fRename)
    {
        status = CheckDirectory(oldPath);   
        if (status != E_OK)
            return status;
        if (fDirectory)
        {
            status = RenameDirectory(oldPath, newPath);
            if (status != E_OK)
                return status;
            fDirectory = DISABLE;
        }
        else
        {
            status = RenameFile(oldPath, newPath);
            if (status != E_OK)
                return status;
        }
    }
    return status;
}

//  function: CheckDirectory
//      This function checks if a given path is a file or directory and 
//      sets the fDirectory flag appropriately
//  @param: char pointer to the file path we want to check
//  @return: Integer error code
int CheckDirectory(char *filePath)
{
    struct stat fileInfo;
    int error = stat(filePath, &fileInfo);
    if (error == -1)
    {
        return errno;
    }
    else 
    {
        if(S_ISDIR(fileInfo.st_mode))
        {
            fDirectory = ENABLE;
            return E_OK;
        }
        else 
            return E_OK;
    }

}

//  function: NonBlockingOperation
//      This is somewhat of wrapper function used to call appropriate read/write system call
//      using function pointers. 
//  @param: Pointer to a function with the same declaration as read/write system call
//  @param: Integer flag
int NonBlockingOperation(ssize_t (*operation) (int, void *, size_t), int flag, char *filePath, void* buffer, int noOfBytes) //will cause warning with write function as argument, but this is not an issue
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
        status = (*operation)(fd, buffer, noOfBytes);
        if (status == E_GENERAL)
            status = errno;
        while (status == EAGAIN)
        {
            status = (*operation)(fd, buffer, noOfBytes);
            if (status == -1)
            {
                status = errno;
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
            return E_OK;
        status = close(fd);
        if (status == E_GENERAL)
            status = errno;
        return status;
    }
}

int AppendOddNumbers(int startNumber, char *filePath)
{
    short int oddNumbers[25];
    int bytesToWrite = 50;
    if (startNumber < 50)
        return E_GENERAL;
    else if (startNumber % 2 == 0)
        startNumber ++;
    for (int i = 0; i < 25; i ++)
    {
        if (startNumber > 200)
            {
                bytesToWrite = --i * 4;
                break;
            }
        oddNumbers[i] = startNumber;
        startNumber += 2;
    }
    int status = NonBlockingOperation(&write, O_WRONLY | O_APPEND, filePath, oddNumbers, bytesToWrite);
    return status;
}

int AppendText(char *text, char* filePath) //Wrapper function
{
    int bytesToWrite = strlen(text);
    if (bytesToWrite > N_BYTES)
        bytesToWrite = N_BYTES;
    int status = NonBlockingOperation(&write, O_WRONLY | O_APPEND, filePath, text, bytesToWrite);
    return status;
}

int PrintFirstNBytes(char *fileName)
{
    char buffer[MAX_APPEND_SIZE];
    int status = NonBlockingOperation(&read, O_RDONLY, fileName, buffer, N_BYTES);
    if (status != E_OK)
    {
        return status;
    }
    else
    {
        status = NonBlockingOperation(&write, 0, "stdout", buffer, N_BYTES);
        return status;
    }
}


int CreateFile(char *pathName)
{
    int fd = creat(pathName, S_IRWXU);
    if (fd == E_GENERAL)
    {
        return errno;
    }
    else 
    {
        int status = close(fd);
        if (status == E_GENERAL)
            return errno;
        return E_OK;
    }
    
}

int RenameFile(char *oldFilePath, char *newFilePath)
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

int RenameDirectory(char *oldDirPath, char *newDirPath)
{
    if (rename(oldDirPath, newDirPath) == E_GENERAL)
        return errno;
    else return E_OK;
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

int RemoveFile(char *filePath)
{
    int status = unlink(filePath);
    if (status == E_GENERAL)
    {
        return errno;
    }
    else 
        return E_OK;
}
int RemoveDirectory(char *directoryPath)
{
    int status = rmdir(directoryPath);
    if (status == E_GENERAL)
    {
        return errno;
    }
    return E_OK;
}




## Overview:

### Collaborators: Prajas Naik and Kalash Shah
The program only accepts the command line arguments. 
User can optionally: 
1. Create a directory OR Create a file.
2. Delete an empty directory OR Delete a text file.
3. Read and write the first 50 bytes of a file on the Standard Output.
4. Rename a file or a directory
5. Append at most 50 bytes at the end of a text file, the text to add is given as a string on the command line (is ignored if a directory argument is given). And append 50 bytes of odd numbers between [50,200] in a sequence for a binary file, where the starting number given by the user.
6. Write the first 50 bytes of a file (text file or binary file) on the standard output



### How to use the code:

#### Compiling and Execution:

```Bash
        make
        ./my_fm -c <Path> -w <Path> -a <TextFilePath> <string to append> -r <OldPath> <NewPath> -d <Path>
``` 

For information on how to create a directory or append to binary file, please read below.

_Important Note: Warnings will be raised while compiling the file because of the use of function pointers and implicit type conversion from `void *` to `const void *`. These can be ignored as they do not affect the correct functioning of the program._

###### Write
```Bash
        ./my_fm -w <textFile>  # For either a binary or a text file
```
###### Rename
```Bash
        ./my_fm -r <OldPath> <NewPath>  # For either a file or a directory 
```
###### Create (directory or file)
```Bash
        ./my_fm -c <NewFileName> # For a file
        ./my_fm -c <NewDirectoryName> -f # For a Directory
```
###### Append
```Bash
        ./my_fm -a <TextFile (Path or fileName)> "String To Append" # for a text file
        ./my_fm -a <BinaryFile (Path or fileName)> <integer> -b # For a binary file
```
###### Delete
```Bash
        ./my_fm  -d <Name to delete> # Will delete a file or a directory, can take a path as an input or relative path.  
```


### Behvaioural Choices and some explainations:
* We have chosen to write the program so that every operation can be performed on a different file. The file path for the operation is to be specified immediately after the flag specifying the operation. 
* As a result, while all five operations can be performed at the same time on either the same file (by typing the same address after every command) or on different files, the same command cannot be used again during one execution. If done so, only the file/directory specifed last with that flag will be operated on.

* For example, we can use the following commands:
```Bash
    ./my_fm -c test.txt -a test.txt "Hello! World." -w test.txt -d test.txt

    #OR

    ./my_fm -c test.txt -a test2.txt "Hello! World." #and so on

```
* Internally, since operation order is fixed, if all commands are used in the same line, one may encounter errors depending on whether it was renamed / deleted, etc.

* Append: We ask the user to give the input if the file is binary or not using the `-b` flag, as the assignment did not ask us to identify the type of the file. Any method to identify type of file will anyways be probabilistic and would involve use of magic database in linux system. One way to get around this issue is to use the built-in file command and grep the output.
        
* The use of stdlib and stdio are only for the rename system call. As they require it, however, as the man page says, the rename(2) is indeed a system call.
* For creating a file, we chose to give the user the option to specify if it is a file or directory using the `-f` flag. While this adds more responsibility on the user, it provides more functionality as it allows creation of files without any extensions. 
* For all other commands, we determine whether it is a file or directory automatically using system commands.



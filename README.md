### Collaborators: Prajas Naik and Kalash Shah
### Overview:
The program only accepts the command line arguments. 
User can optionally: 
1. Create a directory OR Create a file.
2. Delete an empty directory OR Delete a text file.
3. Read and write the first 50 bytes of a file on the Standard Output.
4. Rename a file or a directory
5. Append at most 50 bytes at the end of a text file, the text to add is given as a string on the command line (is ignored if a directory argument is given). And append 50 bytes of odd numbers between [50,200] in a sequence for a binary file, where the starting number given by the user.
6. Write the first 50 bytes of a file (text file or binary file) on the standard output

### Behvaioural Choices and some explainations:
* Append: We ask the user to give the input if the file is binary or not, as the assignment did not ask us to identify the type of the file.
        
* The use of stdlib and stdio are only for the rename system call. As they require it, however, as the man page says, the rename(2) is indeed a system call.


### How to use the code:
###### write
        -w <textFile>
###### rename
        -r <textfile or directory name> "New name"
###### create (directory or file)

```Bash
    ./my_fm -c <NewFileName> # For a file
    ./my_fm -c <NewDirectoryName> -f #For a Directory
```
-c <textfile>
-c <NewDirectoryName> -f
###### append
        if text file:
            use: ./my_fm -a <textFile (path or file name)> "string to append"
        if binary file:
            use: ./my_fm -a <binary file (path or file name)> <integer> -b
###### delete
        -d <file or directory name>




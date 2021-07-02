# mimic-delgroup
This C program mimics the 'delgroup' command in linux.
This command should delete a linux group.  
I have executed this program using the GNU GCC compiler on windows OS. Windows uses backslash (\\) as a path separator hence if the program has to be run on linux, the path separator has to be changed to forward slashes (/).  
I have used the mingw32 utility in windows os to run the make utility and the command for the same is as below:-  
mingw32-make -f makefile.mk  
  
The program handles 3 files “group” , “gshadow” and "passwd". 
These files are stored in a system folder whose path is specified by an environment variable called 'PFILE'. I have set the value of the environment variable as the path of the folder containing the three files. I have extracted the set environment variable into my program in the main function using getenv() and I have passed it's path to all the functions. I have in total used 4 functions in my program.  
  
The program takes all arguments as command line arguments (Refer man pages for the command line arguments).    
**An important point to note is that, the delgroup mimic does not delete the primary groups of any user.** 
  
The correct syntax for the command line arguments is as below:-    
delgroup GName  
(The only argument is the name of the group being deleted)


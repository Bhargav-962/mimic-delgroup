#include <stdio.h>
#include <stdlib.h>
#include "interface.h" 

// I have used 2.c files(client.c and server.c) and 1.h file(interface.h) and 1 makefile for the program code.
 
/* I have executed this program using the GNU GCC compiler on a windows OS, since Windows uses backslash (\) as a path separator,
 hence if the program has to be run on linux OS, the path separator should be changed to forward slashes(/). I have made sure
 that there are no warnings from the GNU GCC compiler while building the executable. I have used mingw32 for windows os to run
the makefile and build the executable. */
 
/*This program mimics the delgroup command of linux. I have used a total of 3 input files in my program. The three 
 files are group, gshadow and passwd. These files are stored in a system folder, whose path is specified by an
 environment variable 'PFILE'. I have set the value of the environment variable as the path of the folder containing
 the three files. I have extracted the set environment variable from the system to the main function using getenv() and I have passed it's path to all
 the functions. I have used 4 functions in my program.

An important point to note is that, the delgroup mimic does not delete the primary groups of any user. 
I have used the main() function to take the input as command line arguments. It takes one argument as input, which is
the name of the group to be deleted. I have not used any global variables throughout my program.

The correct command line arguments for main() is:- delgroup Gname 
(here Gname is the name of the group to be deleted and delgroup is the name of the executable)
*/



int main(int count_arguments, char *group_name[]) 
{
	int test,check,groupSuccess,gshadowSuccess;
	char *value;
	
	if(count_arguments!=2)									// I have checked for the command syntax here 
		printf("\nInvalid Command Syntax\n");

 	else
	{
		value=getenv("PFILE");								// Here I have extracted the environment variable 'PFILE' which holds the path of the folder containing the 3 files
		test=group_search(group_name[1],value);						// The group_search function checks if the entered group is valid or not.

												// Takes two arguments, name of group and path of the folder.

	 /* Here I have validated the value returned from the group_search() function, the function returns -1 
 	in case of any errors(I have handled the errors in the function), otherwise returns the group id of the entered group to main. 
	 I have elaborated all the functions further in the server.c file. */



		if (test != -1)
		{
			check=group_check(group_name[1],value,test); 				// Call of group_check function, which checks if the entered group is a primary group of a user or not.
												// Takes 3 arguments group name, path of folder and group id of the group to be deleted
			if (check == -1)							
				printf("\n Error in opening file \'passwd\' \n");		// It returns 0 if it is the primary group of a user, else returns 1. 
												// Returns -1 if there is an error in opening a file.
			else if (check == 0)
				printf("\n delgroup:Cannot remove the primary group \'%s\' \n",group_name[1]);

			else										
			{


	 /* The control enters this block if the group is not a primary group or if group_check() returns 1. The functions delgroup_gfile ()
	  and delgroup_gsfile() are called. They return -1 if there was an error in opening the respective files. */



				groupSuccess=delgroup_gfile(group_name[1],value);	 	//For deleting the group from the 'group' file 

				gshadowSuccess=delgroup_gsfile(group_name[1],value);		// For deleting the group from the 'gshadow' file 

				if (groupSuccess == 1 && gshadowSuccess == 1)			// They return 1, if groups are deleted successfully
					printf("Group deleted\n");

				if (groupSuccess == -1)						// Error handling
					printf("\n Error in opening file \'group\' \n");

				if (groupSuccess == 0)						
					printf("\n Group could not be deleted from file \'group\' \n");	// They return 0, if the groups couldn't be deleted.

				if (gshadowSuccess == -1)
					printf("\n Error in opening file \'gshadow\' \n");

				if (gshadowSuccess == 0)
					printf("\n Group could not be deleted from file \'gshadow\' \n");



			}
		}

			
			
	}

	return 0;
}


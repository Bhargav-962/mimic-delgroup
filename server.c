#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "interface.h"

 // group_search() checks whether the entered group exists or not in the 'group' file.

int group_search(char group_name[], char *value)				//Group name and folder path are the parameters
{
	int groupno,flag=-1;
	char gname[32],password[5],secondary_users[50];
	char file[]="\\group";							// I have stored the name of the file to be accesed in the variable 'file'
	char path[50],ch='0';							// the variable 'path' holds the location path of the 'group' file
	FILE *group;								// file pointer to the file 'group'

	strcpy(path,value);
	strcat(path,file);

  /* Here, I have concatenated "\\group" at the end of the folder path so that the variable 'path' points to the 'group' file. */

	group=fopen(path,"r");
	if (group == NULL)
	{
		printf("\n Error in opening file!!\n");
		return -1;							// Error handling of fopen. The error statement is printed in this function.
	}


	while(ch != EOF)
	{



	/* I have used fscanf function to read the data of the 'group' file.
	I would like to mention the bug I faced while reading files and how I fixed it. After the file pointer reaches the '\n'
 	character at the end of every group entry, in the next iteration, the fscanf statement read the newline character of the previous line
 	along with the group name of the next line and stored it in the 'gname' variable. Hence, I used the variable 'ch' to store '\n'
	character at the end of every line. This way, I was able to fix the bug.

	I have made use of a special format specifier %[^:] for fscanf which reads a string from the file until it encounters a colon(:)
	Hence, I have used this to split the data of each line.
	 */  


		if ((fscanf(group, "%[^:]:%[^:]:%d%s", gname,password,&groupno,secondary_users)) == 4) // To compare only if the read is successful
		{
		
			if (strcmp(gname,group_name)==0)			// Group name entered is compared with all the groups in the file
			{
				flag=groupno;					// Storing group number if there is a match
				break;
			}
		}

		ch=fgetc(group);
	}

	if (flag == -1)								// Since the default value of variable 'flag' was set as -1
		printf("\n Specified group does not exist\n");

	fclose(group);
	return flag;

}


	/* group_check() checks if the group entered is a primary group of a user or not. 
	The passwd file stores the primary group id of every user. Hence this function compares the id of the group to be deleted with the primary group id's 
	of all the users and checks for a match. If a match is found it returns 0. This is done so that the primary group of any user is not deleted. */

	 
int group_check(char group_name[],char *value, int groupid)			// three parameters, group name, path and group id of the group to be deleted
{
	int primaryId,result=1;
	char file[]="\\passwd";							// The name of file to be accessed
	char path[50],ch='0';

	char s[500]; // It acts like a buffer variable. I have used this to make fscanf read upto the end of every line, so that fscanf reads from the next line in the next iteration. 

	FILE *passwd;								// file pointer to 'passwd' file

	strcpy(path,value);							// Copied file name to be accessed to path
	strcat(path,file);

	if ((passwd=fopen(path,"r")) == NULL)					// error handling
		return -1;

	while(ch!= EOF)
	{
	// I have used a special format specifier %*[^:], the asterick(*) tells the fscanf to read a string from the file without storing it.

		if ((fscanf(passwd,"%*[^:]:%*[^:]:%*[^:]:%d:%[^\n]",&primaryId,s)) == 2) //fscanf reads two arguments
		{
			if (primaryId == groupid)				// The primary group id is compared with the group id of the group to be deleted
			{
				result=0;
				break;
			}
		}

		ch=fgetc(passwd);
	}

	fclose(passwd);
	return result;
}


// delgroup_gfile() deletes all entries of the group from the 'group' file. Returns the outcome of deletion to main().

int delgroup_gfile(char group_name[], char *value)                 //Defining the fucntion to delete the group  from the 'group' file
{
	int linecount=1,current_line=1,flag=0;			// Line count stores the line number to be deleted

	char gname[30],password[5],groupno[5],secondary_users[30];  // Variables to store the data of every line.                                
	char ch='0',path1[50],path2[50];

	char file1[]="\\group";
	char file2[]="\\replica";				//Name of a new file.
	FILE *group,*temp_group;

	strcpy(path1,value);					
	strcat(path1,file1);

	strcpy(path2,value);
	strcat(path2,file2);					// setting the path for opening the files

	if ((group=fopen(path1,"r")) == NULL)
		return -1;


	while(ch!= EOF)
	{
		if ((fscanf(group, "%[^:]:%[^:]:%[^:]%s", gname,password,groupno,secondary_users)) == 4)  // fscanf takes 4 values
		{
			if (strcmp(gname,group_name)==0)
			{
				flag=1;
				break;
			}
		}

		linecount++;					// linecount keeps track of the line number at which the group to be deleted is present
		ch=fgetc(group);	
	}

	if (flag==1)						// This loop is entered only if the group name is found
	{
		rewind(group); 					// Sets the file pointer to start of the file 'group'
		temp_group=fopen(path2,"w");			// Creating a new file in write mode

		while ((ch=fgetc(group))!= EOF)                // copying all the lines of the 'group' file to the new file except the line to be deleted
		{
			if (ch=='\n')
      				current_line++;

    			if (current_line != linecount)
      				fputc(ch, temp_group);
  		}

		fclose(temp_group);
		fclose(group);

  		remove(path1);
		rename(path2,path1);			// Renaming the new file to 'group'
	}

	else
		fclose(group);

	return flag;
}


// delgroup_gsfile() deletes all entries of the group from the 'gshadow' file. Returns the outcome of deletion to main()

int delgroup_gsfile(char group_name[], char *value)
{
	int linecount=1,current_line=1,flag=0;
	char gname[30],password[5],users[30];                                  // variables to segregate the data of every line
	char ch='0',path1[50],path2[50];
	
	char file1[]="\\gshadow";
	char file2[]="\\replica";						// name of the new file
	FILE *gshadow,*temp_gshadow;

	strcpy(path1,value);
	strcat(path1,file1);

	strcpy(path2,value);
	strcat(path2,file2);							// setting the path for opening the files

	if ((gshadow=fopen(path1,"r")) == NULL)
		return -1;

	while(ch != EOF)
	{
		if ((fscanf(gshadow, "%[^:]:%[^:]:%s", gname,password,users)) == 3) 	// fscanf reads 3 arguments per line
		{
			if (strcmp(gname,group_name)==0)
			{
				flag=1;
				break;
			}
		}

		linecount++;				// linecount keeps track of the line number at which the group to be deleted is present
		ch=fgetc(gshadow);	
	}

	if (flag == 1)
	{

		rewind(gshadow);					// Sets file pointer to the start of the file 'gshadow'
		temp_gshadow=fopen(path2,"w");

		while ((ch=fgetc(gshadow))!= EOF)                      // The loop copies the content of existing file to a new file except the line to be deleted
		{
			if (ch=='\n')
      				current_line++;

    			if (current_line != linecount)
      				fputc(ch, temp_gshadow);
  		}

		fclose(temp_gshadow);
		fclose(gshadow);

		remove(path1);
		rename(path2,path1);					// Renaming the new file to 'gshadow'
	}

	else
		fclose(gshadow);

	return flag;

}

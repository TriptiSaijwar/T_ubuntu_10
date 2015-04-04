#include "system.h"
int i,j,k;
void init()
{
 	for(i=0;i<10;i++)
	{ 
    	direct_no[i]=0; 
  	}
	size=0;
	arr[0][0] = 'r';
	arr[0][1] = 'o';
	arr[0][2] = 'o';
	arr[0][3] = 't';
	curr_dir=0;
}
/** make new directory **/
void mkdir(char* xyz1,int l)
{
    settextcolor(4,0);
    puts("New directory: ");
    for(i = 6;i <l-1;i++)
    {
		arr[size+1][i-6] = xyz1[i];
		putch(xyz1[i]);
    }
    size++;
    puts("\n");
    settextcolor(2,0);
}


/** moves to current directory **/
void cd(char* xyz1,int l)
{
    settextcolor(4,0);
    if(curr_dir)
    {
		if(xyz1[3] == '.' && xyz1[4] == '.')
		{
	    	curr_dir = 0;
	    	puts("Current directory: root");
       	    puts("\n");
		}
		else
	    	puts("Error\n");
    }
    else
    {
		if(xyz1[3] == '.' && xyz1[4] == '.')
		{
	    	puts("Current directory: root");
            puts("\n");
		}
		else
		{
	    	char xyz[30] = {};
	    	for(i =3;i < l-1;i++)
	    		xyz[i-3]=xyz1[i];
	    	for(i=1;i<size+1;i++)
	    	{
				if(strcmp(xyz,arr[i]) == 0)
				{
	            	curr_dir=i;
			    	puts("Current directory: ");
			    	puts(xyz);
	            	puts("\n");
		    		break;
				}
	    	}
		}
	
    }
    settextcolor(2,0);	
}
/** makes a new file **/
void mkfil(char* xyz1,int l)
{
    settextcolor(4,0);	
    if(curr_dir)
    {
		i=curr_dir*10+direct_no[curr_dir];
		puts("New file :");
		for ( j =6;j<l-1;j++)
		{
	    	arr[i][j-6]=xyz1[j];
	    	putch(xyz1[j]);
		}
		puts("\n");
		direct_no[curr_dir]=direct_no[curr_dir]+1;
    }
    settextcolor(2,0);	
}



/** list contents **/ 
void ls()
{
    settextcolor(4,0);	
    if(curr_dir)
    {
		j=curr_dir*10+direct_no[curr_dir];
		i = curr_dir*10;	
		while(i < j)
		{
			k=0;
			while(arr[i][k])
			{
				putch(arr[i][k]);
				k = k+1;
			}
			if(k>1)
				puts("\n");
			i++;
		}
	}
	else
	{
		for(i=1;i<=size;i++)
		{
			k=0;
			while(arr[i][k])
			{
				putch(arr[i][k]);
				k = k+1;
			}
			if(k>1)
				puts("\n");
		}
	}
	settextcolor(2,0);
}	
/** removes file or directory **/
void rm(char*xyz1,int l)
{
	settextcolor(4,0);	
	if(curr_dir)
	{
		char xyz[30] = {};
		for(i =3;i < l-1;i++)
		{
			xyz[i-3]=xyz1[i];
		}
		j=curr_dir*10+direct_no[curr_dir];
		i = curr_dir*10;	
		for(;i<j;i++)
		{
			if(strcmp(xyz,arr[i]) == 0)
			{
				arr[i][0]='\0';
				puts("File ");
				puts(xyz);
				puts(" removed\n");
		
			}
		}
	}
	else
	{
		char xyz[30] = {};
		for(i =3;i < l-1;i++)
		{
			xyz[i-3]=xyz1[i];
		}	
		for(i=1;i<=size;i++)
		{
			if(strcmp(xyz,arr[i]) == 0)
			{
				arr[i][0]='\0';
				puts("Directory ");
				puts(xyz);
				puts(" removed\n");
		
			}
		}
	}
	settextcolor(4,0);	
}


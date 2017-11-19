#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 80 
#define BUFFER_SIZE 50
#define buffer "\n\Shell Command History:\n"

char history[BUFFER_SIZE][BUFFER_SIZE]; 
int count = 0;


int formatCommand(char inputBuffer[], char *args[],int *flag)
{
   	int length; 
    	int i; 
    	int start;  
    	int ct = 0; 
    	int hist;
	int sayac = 0,sayac2 = 0;
	int j = 0,k=0;

 	length = read(STDIN_FILENO, inputBuffer, MAX_LINE);	
 
	    start = -1;
	    if (length == 0)
		exit(0);  
	    if (length < 0)
	    {
		printf("Command not read\n");
		exit(-1); 
	    }
    
   
	    for (i=0;i<length;i++)
	    {
		switch (inputBuffer[i])
		{
		    case ' ':
		    case '\t' :              
		        if(start != -1)
		        {
		            args[ct] = &inputBuffer[start];    
		            ct++;
		        }
		        inputBuffer[i] = '\0'; 
		        start = -1;
		        break;
		        
		    case '\n':                 
		        if (start != -1)
		        {
		            args[ct] = &inputBuffer[start];
		            ct++;
		        }
		        inputBuffer[i] = '\0';
		        args[ct] = NULL; 
		        break;
		    case '&' :
			    *flag  = 1;
		            inputBuffer[i] = '\0';
                	break;
		    default :           
		        if (start == -1)
		            start = i;
			break;
		 
		}
	    }
	    
	    args[ct] = NULL; 
	
	if(strcmp(args[0],"history")==0)
		{
		int index[count];
		int temp;
		char command[count][count];
		int command2 = 0;
		       if(count>0)
			{
			for(i = 0; i<count; i++)
			{
			        sayac = 1;
				sayac2 =1;
				if(i>0)
				{
				for(k = i-1; k>=0 ; k--)
				{
					if(strcmp(history[i],history[k]) == 0)
				        {
						sayac2 = 0;
						break;
				        }
					else
						sayac2 = 1;
									
				}
				}
				if(sayac2 == 1)
				{
				for(j = i+1; j<count; j++)
			        {	
					if(strcmp(history[i],history[j]) == 0)
					{
					    sayac++;
					}
				}
				index[i] = sayac;
				strcpy(command[i],history[i]);
				command2++;
				printf("Komut:%s --> %d\n", command[i],index[i]);
				
				}

			}

			/*for(i = 0; i<command2; i++)
			{
				for(j = i+1; j<command2; j++)
				{
				if (index[i] < index[j])
                    		{
				        temp = index[j];

				        index[j] = index[i];

				        index[i] = temp;
                   		 }
				}

			}*/
			
			char temp2[command2][command2];
			for(i = 1; i<command2 ; i++)
			{
				temp = index[i];
				strcpy(temp2[i],history[i]);
				j = i;
				while(j>0 && index[j-1] < temp)
				{
					index[j] = index[j-1];
					strcpy(history[j],history[j-1]);
					j--;

				}
				index[j] = temp;
				strcpy(history[j],temp2[i]);
			}
			for(i = 0; i<command2; i++)
			{

				printf("Komut : %s --> %d \n", history[i], index[i]);
			}
			
			}
			else
			{
			printf("\nNo Commands in the history\n");
			}
			return -1;
		}

		else if(strcmp(args[0],"exit")==0)
		{
		exit(-1);
		}
		/*else if (args[0][0]-'!' ==0)
		{	int x = args[0][1]- '0'; 
			int z = args[0][2]- '0'; 
		
			if(x>count) 
			{
			printf("\nNo Such Command in the history\n");
			strcpy(inputBuffer,"Wrong command");
			} 
			else if (z!=-48) 
			{
			printf("\nNo Such Command in the history. Enter <=!9 (buffer size is 10 along with current command)\n");
			strcpy(inputBuffer,"Wrong command");
			}
			else
			{

				if(x==-15)
				{	 strcpy(inputBuffer,history[0]); 
				}
				else if(x==0) 
				{	 printf("Enter proper command");
					strcpy(inputBuffer,"Wrong command");
				}
			
				else if(x>=1) 
				{
					strcpy(inputBuffer,history[count-x]);

				}
			
			}
		}*/
	/* for (i = 9;i>0; i--) 
	       	strcpy(history[i], history[i-1]);
	    
			*/
	    strcpy(history[count],inputBuffer); 
	    count++;

            
		
/*
		if(count>10)
		{ count=10;
		}*/
	}



int main(void)
{
    char inputBuffer[MAX_LINE];
    int flag; 
    char *args[MAX_LINE/2 + 1];
    int should_run =1;
    
    pid_t pid;
    //int i;
   
    
    while (should_run)
    {            
        flag = 0;
        printf("osh>");
        fflush(stdout);
        if(formatCommand(inputBuffer,args,&flag) != -1) 
	{
		pid = fork();
        
        	if (pid < 0)
        	{
            		printf("Fork failed.\n");
            		exit (1);
        	}
        
       		 else if (pid == 0)
        	{      
            		if (execvp(args[0], args) == -1)
           	 	{
                		printf("Error executing command\n");
            		}
       		 }
        
        	else
        	{
            		//i++;
           	 	if (flag == 0)
           		 {
                		//i++;
                		wait(NULL);
           		 }
			
			if(flag == 1)
			{
			    printf("Parent process is terminated...");
			    exit(1);				
			}
        	}
   	 }
     }
}
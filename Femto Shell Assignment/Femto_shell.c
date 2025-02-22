#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define NoOfArgumentsOfECHO 2
int  echo(char* str)
	{
		int NoOfBytes;
	
	   		NoOfBytes = write(1,str, strlen(str));
			NoOfBytes = write(1," ",1);
                        NoOfBytes = write(1,"\n",1);
			return 0;
	}

int main()
{

   	char sentence[256];
    	char * arr;
	char firstWord[256];
        char remainingSentence[256];
	bool run = true ;
	

	while(run){
        printf("Femto shell prompt > ");
	//user enter a commmand
	arr = fgets(sentence, sizeof(sentence), stdin);
	if(arr == NULL)
	{
		printf("could not take input\n");
		exit(-1);
	}

	// Remove the newline character from the sentence
    	sentence[strcspn(sentence, "\n")] = 0;
	// First word
	sscanf(sentence, "%s", firstWord);
	int firstWordLength = strlen(firstWord);
	// Copy the remaining sentence
	strcpy(remainingSentence, sentence + firstWordLength);
	
        // Remove leading spaces from the remaining sentence
        memmove(remainingSentence, remainingSentence + strspn(remainingSentence, " "), strlen(remainingSentence));

	if(strcmp(firstWord ,"echo")== 0)
	{
		echo(remainingSentence);		
	}
	else if(strcmp(firstWord ,"exit")==0)
	{
		printf("Good Bye :)\n");
		run = false;
	}
	else
	{
		printf("Invalid Command\n");
	}
}
	return 0;
	
}

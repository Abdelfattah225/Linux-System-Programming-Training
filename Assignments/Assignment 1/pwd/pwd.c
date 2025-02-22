#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define NoOfArguments 1 

int main(int argc , char** argv)
{


	char buf[100];
	char* ptr ;
  	if( NoOfArguments > 1)
        {
                perror("Usage: %s only \n");
                exit(-1);
        }

	ptr = getwd(buf);
	if(ptr == NULL)
	{
		perror("Could not get currnet working directory\n");
		exit(-2);
	}
	int NoOfBytes = write(1,buf,strlen(buf));
	if(NoOfBytes < 0)
	{
		perror("Could not display current working directory\n");
		exit(-3);
	}	
        NoOfBytes = write(1,"\n",1);

	return 0;
}

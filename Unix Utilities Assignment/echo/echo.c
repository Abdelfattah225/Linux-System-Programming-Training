#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define NoOfArguments 2

int main(int argc , char** argv)
{

	 if(argc < NoOfArguments)
        {
		char *msg = "No Arguments Provieded\n";
                int NoOfBytes = write(1,argv[1],strlen(argv[1]));
                exit(-1);
        }
	for(int i=1 ; i< argc ; i++)
	{
	   	int NoOfBytes = write(1,argv[i], strlen(argv[i]));
		if(i < argc-1)
		{
			int noOfBytes = write(1," ",1);
		}
	}

                        int noOfBytes = write(1,"\n",1);

	return 0;

}

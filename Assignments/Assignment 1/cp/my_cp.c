#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define NoOfArguments 3
#define Count 1000
int main(int argc , char** argv)
{
	
	char buf[Count];
	int num_read ;
	int source_fd ;
	int dest_fd;
	int NoOfBytes ;
	if(argc != NoOfArguments)
	{
		printf("Usage: %s <source-file> <destination-file>\n",argv[0]);
		exit(-1);
	}

	// OPEN SOURCE
	 source_fd = open(argv[1],O_RDONLY);
	if(source_fd < 0)
	{
		printf("Could not open the file\n");
		exit(-2);
	}

	// OPEN DeSTNATION

	 	dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if(dest_fd < 0)
		{
			printf("Could not open the destantion.\n");
			exit(-3);
		}


	while((num_read = read(source_fd , buf , Count)) > 0)
	{
		//2. write info in distnation.
		NoOfBytes = write(dest_fd , buf , num_read) ;
		if(NoOfBytes < 0)
		{
			// error message.
			printf("Write Failed\n");
			exit(-4);
		}
	
	}
	close(source_fd);
	close(dist_fd);
	
	
	return 0 ;
}

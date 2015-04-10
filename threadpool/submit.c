#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "ioctl-io.h"

int main(int argc, char **argv)
{
	int i, ret;

	for(i=0; i<3; i++)
	{
		ret = fork();
		if(ret == 0)      // in child
		{
			break;
		}
		else if(ret > 0)  // in parent
		{
			continue;
		}
		else              // error
		{
			printf("fork failed\n");
			return;
		}
	}
	
	printf("This is process %d.\n", getpid());

	int fd = 0;
	int c;

	fd = open("/dev/tzvisor-vm", O_RDWR);
	if(fd < 0)
	{
		printf("Failed to open the device\n");
		return -1;
	}
	
	for(c=0; c<3; c++)
	{
		// submit the job
		if(ioctl(fd, TZV_IOSUBMIT, NULL) < 0)
		{
			printf("Failed to use ioctl\n");
			return -1;
		}
	}
	close(fd);

	return 0;
}

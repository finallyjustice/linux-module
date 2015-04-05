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
	int fd = 0;

	fd = open("/dev/tzvisor-vm", O_RDWR);
	if(fd < 0)
	{
		printf("Failed to open the device\n");
		return -1;
	}

	if(ioctl(fd, TZV_IORD, NULL) < 0)
	{
		printf("Failed to use ioctl\n");
		return -1;
	}

	close(fd);

	return 0;
}

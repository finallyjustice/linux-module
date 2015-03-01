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
	int fd_sec = 0;
	char *path = "/home/zhang/tzvfile";
	char *buf = malloc(2048);

	fd_sec = open(path, O_RDONLY);
	if(fd_sec < 0)
	{
		printf("Failed to open the file!\n");
		return -1;
	}

	int len = read(fd_sec, buf, 2048);
	close(fd_sec);

	int fd = 0;

	fd = open("/dev/tzvisor-vm", O_RDWR);
	if(fd < 0)
	{
		printf("Failed to open the device\n");
		return -1;
	}

	struct tzv_file_info tfi;
	tfi.len = len; 
	tfi.bin = buf;
	if(ioctl(fd, TZV_IOREG, &tfi) < 0)
	{
		printf("Failed to use ioctl\n");
		return -1;
	}

	close(fd);

	return 0;
}

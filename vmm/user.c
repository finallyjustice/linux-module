#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	int fd = open("/dev/linuxvmm", O_RDWR);
	if(fd < 0)
	{
		printf("Failed to open file\n");
		return 1;
	}

	int ret = ioctl(fd, 100);
	printf("RET: %d\n", ret);
	
	close(fd);

	return 0;
}

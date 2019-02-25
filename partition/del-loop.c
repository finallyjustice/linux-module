#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/loop.h>

int main(int argc, char **argv)
{
	int fd, ret;
	int id;

	if (argc != 2) {
		printf("loop-control <loop id>\n");
		return -1;
	}

	id = atoi(argv[1]);

	fd = open("/dev/loop-control", O_RDWR|O_CLOEXEC);
	if (fd <= 0) {
		printf("failed to open /dev/loop-control\n");
		return -1;
	}

	ret = ioctl(fd, LOOP_CTL_REMOVE, id);
	if (ret < 0) {
		printf("failed to remove /dev/loop%d, perhaps it is not detached\n", id);
		return -1;
	}

	printf("/dev/loop%d is removed\n", id);

	ret = ioctl(fd, LOOP_CTL_ADD, id);
	if (ret < 0) {
		printf("failed to add /dev/loop%d again: ret=%d\n", id, ret);
		return -1;
	}

	printf("/dev/loop%d is added\n", id);

	close(fd);

	return 0;
}

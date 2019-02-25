#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <linux/blkpg.h>

int main(int argc, char **argv)
{
	int fd, ret;
	struct blkpg_partition part;
	struct blkpg_ioctl_arg arg;

	char *loop;
	int partno;

	if (argc != 3) {
		printf("usage: rm-part <loop path> <partno>\n");
		printf("       rm-part /dev/loop0 1\n");
		return -1;
	}

	loop = argv[1];
	partno = atoi(argv[2]);

	fd = open(loop, O_RDWR | O_CLOEXEC);
	if (fd <= 0) {
		printf("failed to open\n");
		return -1;
	}

	memset(&part, 0, sizeof(part));
	memset(&arg, 0, sizeof(arg));

	arg.datalen = sizeof(part);
	arg.data = &part;

	part.pno = partno;
	arg.op = BLKPG_DEL_PARTITION;

	ret = ioctl(fd, BLKPG, &arg);
	if (ret < 0) {
		printf("failed to remove\n");
		return -1;
	}

	close(fd);

	return 0;
}

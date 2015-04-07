#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#include "ioctl-io.h"

int flag = 1;

void get_signal(int n, siginfo_t *info, void *unused)
{
	printf("Signal obtained!\n");
	flag = 0;
}

void set_signal_handler(void)
{
	/* setup signal handler */
	struct sigaction sig;
	sig.sa_sigaction = get_signal;
	sig.sa_flags = SA_SIGINFO;
	sigaction(SIGIO, &sig, NULL);
}

int main(int argc, char **argv)
{
	int fd;

	fd = open("/dev/tzvisor-vm", O_RDWR);
	if(fd < 0)
	{
		printf("Failed to open the device\n");
		return -1;
	}

	if(ioctl(fd, TZV_IOTEST, NULL) < 0)
	{
		printf("Failed to use ioctl\n");
		return -1;
	}

	close(fd);

	set_signal_handler();
	printf("Job is submitted!\n");
	while(flag == 1)
		;

	printf("Job is finished!\n");

	return 0;
}

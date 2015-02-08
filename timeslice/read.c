#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
	int fp;
	fp = open("/dev/temp", O_RDWR, S_IRUSR);
	if(fp < 0)
	{
		printf("Failed to open device /dev/temp!\n");
		return -1;
	}

	char buf[1024];
	int i;
	
	for(i=0; i<5; i++)
	{
		read(fp, buf, 0);
	}

	close(fp);

	return 0;
}

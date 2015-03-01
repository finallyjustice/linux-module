#include <linux/ioctl.h>

struct tzv_file_info
{
	int len;
	char *bin;
};

#define TZ_IO_MAGIC 0xAF

#define TZV_IOREG   _IOWR(TZ_IO_MAGIC, 0xE1, struct tzv_file_info)
#define TZV_IOCALL  _IOWR(TZ_IO_MAGIC, 0xE2, int)
#define TZV_IOUNREG _IOWR(TZ_IO_MAGIC, 0xE3, int)


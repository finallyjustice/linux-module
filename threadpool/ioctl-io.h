#include <linux/ioctl.h>

#define TZ_IO_MAGIC 0xAF

#define TZV_IOSUBMIT  _IOWR(TZ_IO_MAGIC, 0xE2, int)


#include <linux/ioctl.h>

#define TZ_IO_MAGIC 0xAF

#define TZV_IORD  _IOWR(TZ_IO_MAGIC, 0xE2, int)
#define TZV_IOWT  _IOWR(TZ_IO_MAGIC, 0xE3, int)


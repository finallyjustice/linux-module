obj-m := wait-queue.o
wait-queue-objs += main.o 

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)
#EXTRA_CFLAGS += -I$(src)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)
	gcc read.c -o read
	gcc write.c -o write

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module.symvers modules.order
	rm -rf read write

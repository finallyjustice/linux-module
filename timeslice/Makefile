obj-m := cdev-temp.o
cdev-temp-objs += main.o 

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)
#EXTRA_CFLAGS += -I$(src)/include

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)
	gcc read.c -o read

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module.symvers modules.order
	rm -rf read

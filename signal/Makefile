obj-m := ioctl-dev.o
ioctl-dev-objs += main.o 

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)
#EXTRA_CFLAGS += -I$(src)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)
	gcc user.c -o user

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module.symvers modules.order
	rm -rf user

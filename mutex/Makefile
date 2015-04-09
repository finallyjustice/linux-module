obj-m := mutex-test.o
mutex-test-objs += main.o 

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)
#EXTRA_CFLAGS += -I$(src)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)
	gcc lock.c -o lock
	gcc unlock.c -o unlock

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module.symvers modules.order
	rm -rf lock unlock

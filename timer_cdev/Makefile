obj-m := timer-cdev.o
timer-cdev-objs += main.o 

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)
#EXTRA_CFLAGS += -I$(src)/include

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) 
	gcc test_second.c -o test_second

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module.symvers modules.order
	rm -f test_second

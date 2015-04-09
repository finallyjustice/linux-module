obj-m := threadpool.o
threadpool-objs += main.o 

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)
#EXTRA_CFLAGS += -I$(src)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)
	gcc submit.c -o submit

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions Module.symvers modules.order
	rm -rf submit

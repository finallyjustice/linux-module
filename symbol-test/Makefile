CONFIG_MODULE_SIG=n
	
ifneq ($(KERNELRELEASE),)
EXTRA_CFLAGS += -DMY_MAX_LOOP=$(MY_MAX_LOOP)
obj-m = kprobes-sample.o
else
KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules MY_MAX_LOOP=$(MY_MAX_LOOP)
endif

clean:
	rm -rf *.o *.ko *.mod.c .tmp_versions .*.cmd .depend modules.order  Module.symvers *.ko.unsigned

CONFIG_MODULE_SIG=n
	
ifneq ($(KERNELRELEASE),)
obj-m = kprobes-sample.o
else
KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules 
endif

clean:
	rm -rf *.o *.ko *.mod.c .tmp_versions .*.cmd .depend modules.order  Module.symvers *.ko.unsigned

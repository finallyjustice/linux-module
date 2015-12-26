#!/bin/bash

make 2>/dev/null 1>/dev/null
if [ "x$?" != "x0" ]; then
	echo "build kernel module failed, please make sure kernel-devel/kernel-uek-devel is installed"
	echo "NOTE: the version of kernel-devel/kernel-uek-devel should be compatible with current running kernel/kernel-uek"
	exit 1
fi

insmod kprobes-sample.ko
rmmod kprobes-sample

dmesg > kprobes-sample.log

make clean >/dev/null

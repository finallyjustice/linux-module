#!/bin/bash

kaddr=`cat /proc/kallsyms | grep '\<max_loop\>' | awk '{print $1}'`
if [ "x$kaddr" == "x" ]; then
	echo "max_loop symbol found, maybe this is not an ovm guest (DomU)"
	exit 1
fi

if [[ ! $kaddr =~ ^ffffffff ]]; then
	echo "invalid max_loop address $kaddr, maybe this is not an ovm guest (DomU)"
	exit 1
fi

make MY_MAX_LOOP=0x$kaddr 2>/dev/null 1>/dev/null
if [ "x$?" != "x0" ]; then
	echo "build kernel module failed, please make sure kernel-devel/kernel-uek-devel is installed"
	echo "NOTE: the version of kernel-devel/kernel-uek-devel should be compatible with current running kernel/kernel-uek"
	exit 1
fi

insmod kprobes-sample.ko
rmmod kprobes-sample

dmesg | tail -n 1

make clean >/dev/null

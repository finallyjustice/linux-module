#!/bin/sh

rmmod dump-block

all_bdevs_addr=`cat /proc/kallsyms | grep all_bdevs | awk '{print $1}'`
echo "all_bdevs_addr=0x$all_bdevs_addr"
insmod dump-block.ko local_all_bdevs_addr=0x$all_bdevs_addr

rm /dev/second
rmmod timer-cdev
insmod timer-cdev.ko
mknod /dev/second c 248 0

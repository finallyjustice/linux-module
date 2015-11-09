rm /dev/globalfifo
rmmod test-chrdev
insmod test-chrdev.ko
mknod /dev/globalfifo c 231 0

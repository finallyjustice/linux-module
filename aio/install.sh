rm /dev/globalfifo
rmmod test-aio
insmod test-aio.ko
mknod /dev/globalfifo c 231 0

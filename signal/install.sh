rmmod ioctl-dev
rm /dev/tzvisor-vm
insmod ioctl-dev.ko
mknod /dev/tzvisor-vm c 50 0
chmod 666 /dev/tzvisor-vm

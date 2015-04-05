rmmod wait-queue
rm /dev/tzvisor-vm
insmod wait-queue.ko
mknod /dev/tzvisor-vm c 50 0
chmod 666 /dev/tzvisor-vm

rmmod threadpool
rm /dev/tzvisor-vm
insmod threadpool.ko
mknod /dev/tzvisor-vm c 50 0
chmod 666 /dev/tzvisor-vm

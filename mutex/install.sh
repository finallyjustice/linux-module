rmmod mutex-test
rm /dev/tzvisor-vm
insmod mutex-test.ko
mknod /dev/tzvisor-vm c 50 0
chmod 666 /dev/tzvisor-vm

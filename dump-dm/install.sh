rmmod dump-dm

if [ -z $1 ] ; then
	echo "usage: $0 dmnfs_name"
	exit 0
fi

_name_buckets=`cat /proc/kallsyms | grep _name_buckets | awk '{print $1}'`
echo "_name_buckets=0x$_name_buckets"
dm_name=$1
insmod dump-dm.ko local_names_addr=0x$_name_buckets local_dm_name=$dm_name

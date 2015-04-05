#include <stdio.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define NETLINK_USER 31

// all message should have a standard nlmsghdr header
struct netlink_data 
{
	struct nlmsghdr msg;
	char data[1024];
};

int link_open(void)
{
	int saved_errno;
	int fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_USER);

	if(fd < 0)
	{
		saved_errno = errno;
		if(errno==EINVAL || errno==EPROTONOSUPPORT || errno== EAFNOSUPPORT)
		{
			printf("Error - audit support not in kernel\n");
		}
		else
		{
			printf("Error - opening audit netlink socket (%s)\n", strerror(errno));
		}
		errno = saved_errno;
		return fd;
	};

	// close the fd after exec
	if(fcntl(fd, F_SETFD, FD_CLOEXEC) == -1)
	{
		saved_errno = errno;
		close(fd);
		printf("Error - setting audit netlink socket CLOEXEC flag (%s)\n", strerror(errno));
		errno = saved_errno;
		return -1;
	}

	return fd;
}

int main(int argc, char **argv)
{
	struct netlink_data nldata;
	// standard header
	struct nlmsghdr *msg = &nldata.msg;
	int retval;
	struct sockaddr_nl addr;
	char *data = "hello world! comm\0";
	int size = strlen(data);

	int fd = link_open();

	memset(&nldata, '\0', sizeof(nldata));
	msg->nlmsg_len = NLMSG_SPACE(size);
	msg->nlmsg_type = 0;
	msg->nlmsg_flags = 0;
	msg->nlmsg_seq = 0;
	addr.nl_family = AF_NETLINK;
	addr.nl_pid = 0;
	addr.nl_groups = 0;

	memcpy(NLMSG_DATA(msg), data, size);

	retval = sendto(fd, &nldata, msg->nlmsg_len, 0, (struct sockaddr *)&addr, sizeof(addr));

	printf("len: %d data: %s\n", sizeof(NLMSG_DATA(msg)), NLMSG_DATA(msg));

	close(fd);
	return 0;
}

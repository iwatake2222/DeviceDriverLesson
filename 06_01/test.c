#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include "myDeviceDriver.h"

int main()
{
	int fd;
	struct mydevice_values values_set;
	struct mydevice_values values_get;
	values_set.val1 = 1;
	values_set.val2 = 2;

	if ((fd = open("/dev/mydevice0", O_RDWR)) < 0) perror("open");

	if (ioctl(fd, MYDEVICE_SET_VALUES, &values_set) < 0) perror("ioctl_set");
	if (ioctl(fd, MYDEVICE_GET_VALUES, &values_get) < 0) perror("ioctl_get");
	
	printf("val1 = %d, val2 = %d\n", values_get.val1, values_get.val2);

	if (close(fd) != 0) perror("close");
	return 0;
}

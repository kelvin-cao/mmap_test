#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	void *map;
	int fd, size, ret = 0;

	fd = open("/dev/mmap_dev", O_RDWR);
	if (fd < 0) {
		perror("open");
		return -1;
	}

	size = getpagesize();
	map = mmap(0, size, PROT_WRITE | PROT_READ,
		   MAP_SHARED | MAP_FIXED, fd, 0);
	if (map == MAP_FAILED) {
		perror ("mmap");
		ret = -1;
	} else {
		printf("value (offset 0x0): 0x%lx\n", (*(unsigned long *)map)++);
	}

	munmap(map, size);
	close(fd);
	return ret;
}

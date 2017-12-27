#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>

/* ペリフェラルレジスタの物理アドレス(BCM2835の仕様書より) */
#define REG_ADDR_BASE        (0x3F000000)		/* bcm_host_get_peripheral_address()の方がbetter */
#define REG_ADDR_GPIO_BASE   (REG_ADDR_BASE + 0x00200000)
#define REG_ADDR_GPIO_LENGTH 4096
#define REG_ADDR_GPIO_GPFSEL_0     0x0000
#define REG_ADDR_GPIO_OUTPUT_SET_0 0x001C
#define REG_ADDR_GPIO_OUTPUT_CLR_0 0x0028
#define REG_ADDR_GPIO_LEVEL_0      0x0034

#define REG(addr) (*((volatile unsigned int*)(addr)))
#define DUMP_REG(addr) printf("%08X\n", REG(addr));

int main()
{
	int address;	/* GPIOレジスタへの仮想アドレス(ユーザ空間) */
	int fd;

	/* メモリアクセス用のデバイスファイルを開く */
	if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
		perror("open");
		return -1;
	}

	/* ARM(CPU)から見た物理アドレス → 仮想アドレスへのマッピング */
	address = (int)mmap(NULL, REG_ADDR_GPIO_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, REG_ADDR_GPIO_BASE);
	if (address < 0) {
		perror("mmap");
		close(fd);
		return -1;
	}

	/* GPIO4を出力に設定 */
	REG(address + REG_ADDR_GPIO_GPFSEL_0) = 1 << 12;

	/* GPIO4をHigh出力 */
	REG(address + REG_ADDR_GPIO_OUTPUT_SET_0) = 1 << 4;
	DUMP_REG(address + REG_ADDR_GPIO_LEVEL_0);

	/* GPIO4をLow出力 */
	REG(address + REG_ADDR_GPIO_OUTPUT_CLR_0) = 1 << 4;
	DUMP_REG(address + REG_ADDR_GPIO_LEVEL_0);

	/* 使い終わったリソースを解放する */
	munmap((void*)address, REG_ADDR_GPIO_LENGTH);
	close(fd);

	return 0;
}

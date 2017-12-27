#include <stdio.h>

// gcc get_memory.c -L/opt/vc/lib -lbcm_host

int main()
{
	/* https://www.raspberrypi.org/documentation/hardware/raspberrypi/peripheral_addresses.md */
	extern unsigned bcm_host_get_sdram_address(void);
	printf("bcm_host_get_sdram_address = %08X\n", bcm_host_get_sdram_address());
	extern unsigned bcm_host_get_peripheral_address(void);;
	printf("bcm_host_get_peripheral_address = %08X\n", bcm_host_get_peripheral_address());

	return 0;
}

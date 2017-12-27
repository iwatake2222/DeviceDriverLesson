#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

/*** このデバイスに関する情報 ***/
MODULE_LICENSE("Dual BSD/GPL");
#define DRIVER_NAME "MyDeviceB"				/* /proc/devices等で表示されるデバイス名 */

/* ロード(insmod)時に呼ばれる関数 */
static int mydeviceb_init(void)
{
	printk("[B]: mydeviceb_init\n");

	extern void mydevicea_func(void);
	mydevicea_func();

	return 0;
}

/* アンロード(rmmod)時に呼ばれる関数 */
static void mydeviceb_exit(void)
{
	printk("[B]: mydeviceb_exit\n");
}

module_init(mydeviceb_init);
module_exit(mydeviceb_exit);

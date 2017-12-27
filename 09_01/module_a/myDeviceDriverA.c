#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

/*** このデバイスに関する情報 ***/
MODULE_LICENSE("Dual BSD/GPL");
#define DRIVER_NAME "MyDeviceA"				/* /proc/devices等で表示されるデバイス名 */

void mydevicea_func(void)
{
	printk("This is a message in mydevicea_func\n");
}
/* カーネルのシンボルテーブルに登録する。他のカーネルモジュールから呼べるようにする */
EXPORT_SYMBOL(mydevicea_func);


/* ロード(insmod)時に呼ばれる関数 */
static int mydevicea_init(void)
{
	printk("[A]: mydevicea_init\n");
	mydevicea_func();

	return 0;
}

/* アンロード(rmmod)時に呼ばれる関数 */
static void mydevicea_exit(void)
{
	printk("[A]: mydevicea_exit\n");
}

module_init(mydevicea_init);
module_exit(mydevicea_exit);

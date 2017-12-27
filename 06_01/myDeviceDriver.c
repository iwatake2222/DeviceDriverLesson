#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <asm/current.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include "myDeviceDriver.h"

/*** このデバイスに関する情報 ***/
MODULE_LICENSE("Dual BSD/GPL");
#define DRIVER_NAME "MyDevice"				/* /proc/devices等で表示されるデバイス名 */
static const unsigned int MINOR_BASE = 0;	/* このデバイスドライバで使うマイナー番号の開始番号と個数(=デバイス数) */
static const unsigned int MINOR_NUM  = 1;	/* マイナー番号は 0のみ */
static unsigned int mydevice_major;			/* このデバイスドライバのメジャー番号(動的に決める) */
static struct cdev mydevice_cdev;			/* キャラクタデバイスのオブジェクト */
static struct class *mydevice_class = NULL;	/* デバイスドライバのクラスオブジェクト */

/* ioctlテスト用に値を保持する変数 */
static struct mydevice_values stored_values;

/* ioctl時に呼ばれる関数 */
static long mydevice_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk("mydevice_ioctl\n");
	
	switch (cmd) {
	case MYDEVICE_SET_VALUES:
		printk("MYDEVICE_SET_VALUES\n");
		if (copy_from_user(&stored_values, (void __user *)arg, sizeof(stored_values))) {
			return -EFAULT;
		}
		break;
	case MYDEVICE_GET_VALUES:
		printk("MYDEVICE_GET_VALUES\n");
		if (copy_to_user((void __user *)arg, &stored_values, sizeof(stored_values))) {
			return -EFAULT;
		}
		break;
	default:
		printk(KERN_WARNING "unsupported command %d\n", cmd);
		return -EFAULT;
	}
	return 0;
}


/* open時に呼ばれる関数 */
static int mydevice_open(struct inode *inode, struct file *file)
{
	printk("mydevice_open\n");
	return 0;
}

/* close時に呼ばれる関数 */
static int mydevice_close(struct inode *inode, struct file *file)
{
	printk("mydevice_close\n");
	return 0;
}

/* read時に呼ばれる関数 */
static ssize_t mydevice_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	printk("mydevice_read\n");
	return count;
}

/* write時に呼ばれる関数 */
static ssize_t mydevice_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	printk("mydevice_write\n");
	return count;
}

/* 各種システムコールに対応するハンドラテーブル */
static struct file_operations mydevice_fops = {
	.open    = mydevice_open,
	.release = mydevice_close,
	.read    = mydevice_read,
	.write   = mydevice_write,
	.unlocked_ioctl = mydevice_ioctl,
	.compat_ioctl   = mydevice_ioctl,	// for 32-bit App
};

/* ロード(insmod)時に呼ばれる関数 */
static int mydevice_init(void)
{
	printk("mydevice_init\n");

	int alloc_ret = 0;
	int cdev_err = 0;
	dev_t dev;

	/* 空いているメジャー番号を確保する */
	alloc_ret = alloc_chrdev_region(&dev, MINOR_BASE, MINOR_NUM, DRIVER_NAME);
	if (alloc_ret != 0) {
		printk(KERN_ERR  "alloc_chrdev_region = %d\n", alloc_ret);
		return -1;
	}

	/* 取得したdev( = メジャー番号 + マイナー番号)からメジャー番号を取得して保持しておく */
	mydevice_major = MAJOR(dev);
	dev = MKDEV(mydevice_major, MINOR_BASE);	/* 不要? */

	/* cdev構造体の初期化とシステムコールハンドラテーブルの登録 */
	cdev_init(&mydevice_cdev, &mydevice_fops);
	mydevice_cdev.owner = THIS_MODULE;

	/* このデバイスドライバ(cdev)をカーネルに登録する */
	cdev_err = cdev_add(&mydevice_cdev, dev, MINOR_NUM);
	if (cdev_err != 0) {
		printk(KERN_ERR  "cdev_add = %d\n", alloc_ret);
		unregister_chrdev_region(dev, MINOR_NUM);
		return -1;
	}

	/* このデバイスのクラス登録をする(/sys/class/mydevice/ を作る) */
	mydevice_class = class_create(THIS_MODULE, "mydevice");
	if (IS_ERR(mydevice_class)) {
		printk(KERN_ERR  "class_create\n");
		cdev_del(&mydevice_cdev);
		unregister_chrdev_region(dev, MINOR_NUM);
		return -1;
	}

	/* /sys/class/mydevice/mydevice* を作る */
	for (int minor = MINOR_BASE; minor < MINOR_BASE + MINOR_NUM; minor++) {
		device_create(mydevice_class, NULL, MKDEV(mydevice_major, minor), NULL, "mydevice%d", minor);
	}

	return 0;
}

/* アンロード(rmmod)時に呼ばれる関数 */
static void mydevice_exit(void)
{
	printk("mydevice_exit\n");

	dev_t dev = MKDEV(mydevice_major, MINOR_BASE);
	
	/* /sys/class/mydevice/mydevice* を削除する */
	for (int minor = MINOR_BASE; minor < MINOR_BASE + MINOR_NUM; minor++) {
		device_destroy(mydevice_class, MKDEV(mydevice_major, minor));
	}

	/* このデバイスのクラス登録を取り除く(/sys/class/mydevice/を削除する) */
	class_destroy(mydevice_class);

	/* このデバイスドライバ(cdev)をカーネルから取り除く */
	cdev_del(&mydevice_cdev);

	/* このデバイスドライバで使用していたメジャー番号の登録を取り除く */
	unregister_chrdev_region(dev, MINOR_NUM);
}

module_init(mydevice_init);
module_exit(mydevice_exit);

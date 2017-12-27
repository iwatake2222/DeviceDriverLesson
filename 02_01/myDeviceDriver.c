#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <asm/current.h>
#include <asm/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");

#define DRIVER_NAME "MyDevice_NAME"
#define DRIVER_MAJOR 63

/* open時に呼ばれる関数 */
static int myDevice_open(struct inode *inode, struct file *file)
{
	printk("myDevice_open");
	return 0;
}

/* close時に呼ばれる関数 */
static int myDevice_close(struct inode *inode, struct file *file)
{
	printk("myDevice_close");
	return 0;
}

/* read時に呼ばれる関数 */
static ssize_t myDevice_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	printk("myDevice_read");
	buf[0] = 'A';
	return 1;
}

/* write時に呼ばれる関数 */
static ssize_t myDevice_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	printk("myDevice_write");
	return 1;
}

/* 各種システムコールに対応するハンドラテーブル */
struct file_operations s_myDevice_fops = {
	.open    = myDevice_open,
	.release = myDevice_close,
	.read    = myDevice_read,
	.write   = myDevice_write,
};

/* ロード(insmod)時に呼ばれる関数 */
static int myDevice_init(void)
{
	printk("myDevice_init\n");
	/* ★ カーネルに、本ドライバを登録する */
	register_chrdev(DRIVER_MAJOR, DRIVER_NAME, &s_myDevice_fops);
	return 0;
}

/* アンロード(rmmod)時に呼ばれる関数 */
static void myDevice_exit(void)
{
	printk("myDevice_exit\n");
	unregister_chrdev(DRIVER_MAJOR, DRIVER_NAME);
}

module_init(myDevice_init);
module_exit(myDevice_exit);

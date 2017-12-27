#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/i2c.h>
#include <asm/uaccess.h>

/*** このデバイスに関する情報 ***/
MODULE_LICENSE("Dual BSD/GPL");
#define DRIVER_NAME "MyDevice"				/* /proc/devices等で表示されるデバイス名 */

/* このデバイスドライバで取り扱うデバイスを識別するテーブルを登録する */
/* 重要なのは最初のnameフィールド。これでデバイス名を決める。後ろはこのドライバで自由に使えるデータ。ポインタや識別用数字を入れる */
static struct i2c_device_id mydevice_i2c_idtable[] = {
	{"MyI2CDevice", 0},
	{ }
};
MODULE_DEVICE_TABLE(i2c, mydevice_i2c_idtable);

static int mydevice_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	printk("mydevice_i2c_probe\n");
	printk("id.name = %s, id.driver_data = %d", id->name, id->driver_data);
	printk("slave address = 0x%02X\n", client->addr);

	/* 通常はここで、このデバドラでサポートしているデバイスかどうかチェックする */

	int version;
	version = i2c_smbus_read_byte_data(client, 0x0f);
	printk("id = 0x%02X\n", version);

	return 0;
}

static int mydevice_i2c_remove(struct i2c_client *client)
{
	printk("mydevice_i2c_remove\n");
	return 0;
}

static struct i2c_driver mydevice_driver = {
	.driver = {
		.name	= DRIVER_NAME,
		.owner = THIS_MODULE,
	},
	.id_table		= mydevice_i2c_idtable,		// このデバイスドライバがサポートするI2Cデバイス
	.probe			= mydevice_i2c_probe,		// 対象とするI2Cデバイスが認識されたときに呼ばれる処理
	.remove			= mydevice_i2c_remove,		// 対象とするI2Cデバイスが取り外されたときに呼ばれる処理
};

#if 1
/* 本デバイスドライバを、I2Cバスを使用するデバイスドライバとして登録する */
/* else側の処理と等価 */
module_i2c_driver(mydevice_driver);
#else
/* ロード(insmod)時に呼ばれる関数 */
static int mydevice_init(void)
{
	printk("mydevice_init\n");
	
	/* 本デバイスドライバを、I2Cバスを使用するデバイスドライバとして登録する */
	i2c_add_driver(&mydevice_driver);
	return 0;
}

/* アンロード(rmmod)時に呼ばれる関数 */
static void mydevice_exit(void)
{
	printk("mydevice_exit\n");
	i2c_del_driver(&mydevice_driver);
}

module_init(mydevice_init);
module_exit(mydevice_exit);
#endif

#if 0
I2Cデバイスの実体化には、下記コマンドを使う
sudo bash -c 'echo MyI2CDevice 0x18 > /sys/bus/i2c/devices/i2c-1/new_device'
sudo bash -c 'echo  0x18 > /sys/bus/i2c/devices/i2c-1/delete_device'
#endif

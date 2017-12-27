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

/*** I2Cデバイスの管理情報 ***/
/* このデバイスドライバで取り扱うI2Cデバイス */
enum mydevice_i2c_model {
	MYDEVICE_MODEL_A = 0,
	MYDEVICE_MODEL_NUM,
};

/* このデバイスドライバで取り扱うデバイスを識別するテーブルを登録する */
/* 重要なのは最初のnameフィールド。これでデバイス名を決める。後ろはこのドライバで自由に使えるデータ。ポインタや識別用数字を入れる */
static struct i2c_device_id mydevice_i2c_idtable[] = {
	{"MyI2CDevice", MYDEVICE_MODEL_A},
	{ }
};
MODULE_DEVICE_TABLE(i2c, mydevice_i2c_idtable);

static ssize_t get_version(struct device *dev, struct device_attribute *dev_attr, char * buf)
{
	printk("get_version\n");
	struct i2c_client * client = to_i2c_client(dev);

	int version;
	version = i2c_smbus_read_byte_data(client, 0x0f);
	return sprintf(buf, "id = 0x%02X\n", version);
}
static DEVICE_ATTR(version, S_IRUGO, get_version, NULL);

static int mydevice_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	printk("mydevice_i2c_probe\n");
	printk("id.name = %s, id.driver_data = %d", id->name, (int)(id->driver_data));
	printk("slave address = 0x%02X\n", client->addr);

	/* 通常はここで、このデバドラでサポートしているデバイスかどうかチェックする */

	/* このデバイスドライバの属性読み書き用のsysfsファイルを作る */
	/* (/sys/devices/platform/soc/3f804000.i2c/i2c-1/1-0018/version) を作る */
	device_create_file(&client->dev, &dev_attr_version);

	return 0;
}

static int mydevice_i2c_remove(struct i2c_client *client)
{
	printk("mydevice_i2c_remove\n");
	device_remove_file(&client->dev, &dev_attr_version);

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


/* 本デバイスドライバを、I2Cバスを使用するデバイスドライバとして登録する */
module_i2c_driver(mydevice_driver);

#if 0
I2Cデバイスの実体化には、下記コマンドを使う
sudo bash -c 'echo MyI2CDevice 0x18 > /sys/bus/i2c/devices/i2c-1/new_device'
sudo bash -c 'echo  0x18 > /sys/bus/i2c/devices/i2c-1/delete_device'
#endif

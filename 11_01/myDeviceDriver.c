#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/i2c.h>
#include <linux/of_platform.h>

/*** このデバイスに関する情報 ***/
MODULE_LICENSE("Dual BSD/GPL");
#define DRIVER_NAME "MyDevice"				/* /proc/devices等で表示されるデバイス名 */

/* このデバイスドライバで取り扱うデバイスのマッチングテーブル */
/* dts内の、下記に対応
	i2c@7e804000 {
		mydevice@18 {
			compatible = "mycompany,myoriginaldevice";
			reg = <0x18>;
		};
*/
static const struct of_device_id mydevice_of_match_table[] = {
	{.compatible = "mycompany,myoriginaldevice",},
	{ },
};
MODULE_DEVICE_TABLE(of, mydevice_of_match_table);

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
	if(id != NULL)
		printk("id.name = %s, id.driver_data = %d", id->name, id->driver_data);
	if(client != NULL)
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
		.name			= DRIVER_NAME,
		.owner			= THIS_MODULE,
		.of_match_table = mydevice_of_match_table,
	},
	.id_table		= mydevice_i2c_idtable,		// このデバイスドライバがサポートするI2Cデバイス
	.probe			= mydevice_i2c_probe,		// 対象とするI2Cデバイスが認識されたときに呼ばれる処理
	.remove			= mydevice_i2c_remove,		// 対象とするI2Cデバイスが取り外されたときに呼ばれる処理
};

module_i2c_driver(mydevice_driver);


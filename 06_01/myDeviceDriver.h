#ifndef MY_DEVICE_DRIVER_H_
#define MY_DEVICE_DRIVER_H_

#include <linux/ioctl.h>

/*** ioctl用パラメータ(第3引数)の定義 ***/
struct mydevice_values {
	int val1;
	int val2;
};


/*** ioctl用コマンド(request, 第2引数)の定義 ***/
/* このデバイスドライバで使用するIOCTL用コマンドのタイプ。なんでもいいが、'M'にしてみる */
#define MYDEVICE_IOC_TYPE 'M'

/* デバドラに値を設定するコマンド。パラメータはmydevice_values型 */
#define MYDEVICE_SET_VALUES _IOW(MYDEVICE_IOC_TYPE, 1, struct mydevice_values)

/* デバドラから値を取得するコマンド。パラメータはmydevice_values型 */
#define MYDEVICE_GET_VALUES _IOR(MYDEVICE_IOC_TYPE, 2, struct mydevice_values)


#endif /* MY_DEVICE_DRIVER_H_ */

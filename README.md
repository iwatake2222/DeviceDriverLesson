# 1回目: ビルド環境準備と、簡単なカーネルモジュールの作成

# 本連載について
組み込みLinuxのデバイスドライバをカーネルモジュールとして開発するためのHowTo記事です。本記事の内容は全てラズパイ(Raspberry Pi)上で動かせます。

Linuxデバドラ開発は最初の一歩が難しいと思います。資料も少なく、たいていの人はオライリー本に手を出して挫折すると思います。(僕がそうでした。。。)

この記事では、「Linuxデバイスドライバプログラミング (平田 豊) 」の内容に沿って進めていこうと思います。この本は2008年発行と古いので、現在(2017年12月)の環境でも動くように、実際にRaspberry Piで動かしながら確認していこうと思います。(途中から、本の内容とは離れます)

また、出来るだけ簡単にしたかったので、クロス開発環境は整えず、ラズパイ本体で開発を行います。そのため、ホストOSが何であろうと、ラズパイを持っている方はすぐに試してみることが出来ます。(コーディングをWindows等のホストPCで行って、ラズパイにSFTP転送するという開発スタイルがおすすめです)

- __[1回目: ビルド環境準備と、簡単なカーネルモジュールの作成](https://qiita.com/take-iwiw/items/1fdd2e0faaaa868a2db2)  <--------------------- 今回の内容__ 
- [2回目: システムコールハンドラとドライバの登録(静的な方法)](https://qiita.com/take-iwiw/items/580ec7db2e88beeac3de)
- [3回目: システムコールハンドラとドライバの登録(動的な方法)](https://qiita.com/take-iwiw/items/6b02494a3668f79800e6)
- [4回目: read/writeの実装とメモリのお話](https://qiita.com/take-iwiw/items/26d5f7f4894ccc4ce227)
- [5回目: ラズパイ用のGPIOデバドラの実装](https://qiita.com/take-iwiw/items/cd1d7734c8911830386d)
- [6回目: ioctlの実装](https://qiita.com/take-iwiw/items/ade0a73d4c05fc7961d3)
- [7回目: procfs用インタフェース](https://qiita.com/take-iwiw/items/548444999d2dfdc06f46)
- [8回目: debugfs用インタフェース](https://qiita.com/take-iwiw/items/1ef4b629f9b9bab4d222)
- [9回目: 他のカーネルモジュールの関数を呼ぶ / GPIO制御関数を使う](https://qiita.com/take-iwiw/items/e92a950a2cf57d1a3ded)
- [10回目: I2Cを使ったデバイスドライバを作る](https://qiita.com/take-iwiw/items/dbc544864f0e9873270a)
- [11回目: デバイスツリーにI2Cデバイスを追加する](https://qiita.com/take-iwiw/items/0d13142863d9ed064e41)
- [12回目: 作成したデバイスドライバを起動時にロードする](https://qiita.com/take-iwiw/items/b9dd02724e83e36dabc3)

## 今回の内容
まずは、環境の準備と、簡単なカーネルモジュールを作ってみます。

### 本記事に登場するソースコード全体
https://github.com/take-iwiw/DeviceDriverLesson/tree/master/01_01
https://github.com/take-iwiw/DeviceDriverLesson/tree/master/01_02

## 環境
- Raspberry Pi 2 (Raspbian stretch)

# 環境の準備
冒頭で述べたように、ラズパイ上で開発を行います。つまり、ネイティブ環境でビルドを行います。そのため、基本的にはラズパイ上のgccなどをそのまま使えるので、何の準備もいらないはずです。しかし、カーネルの機能にアクセスするためのヘッダ等が必要になります。下記コマンドでインストールします。これによってヘッダ一式とビルド用Makefileがここ(`/usr/src/linux-headers-4.9.41-v7+/`)にインストールされます。また、ここへのシンボリックリンクがここ(`/lib/modules/4.9.41-v7+/build`)に作成されます。(このディレクトリは僕の環境の場合)

```
sudo apt-get install raspberrypi-kernel-headers
```

カーネル全体をビルドするわけではないので、準備はこれだけです。

# 簡単なカーネルモジュールを作る
## コードを書く
まずは、カーネルモジュールがロード(insmod)されたときと、アンロード(rmmod)されたときの挙動だけを書いてみます。Hello Worldのようなものです。ファイル名をtest.cとします。`module_init`でinsmod用のエントリポイントを、`module_exit`でrmmod用のエントリポイントを指定します。カーネル内ではprintfは使用できないのでprintkを使用します。

```c:test.c
#include <linux/module.h>

static int test_init(void)
{
	printk("Hello my module\n");
	return 0;
}

static void test_exit(void)
{
	printk("Bye bye my module\n");
}

module_init(test_init);
module_exit(test_exit);
```

## Makefileを用意する
カーネルモジュールのビルドは、自分でgccコマンドを打ってもできると思うのですが、色々とインクルードパスの指定とかが面倒です。上述したようにビルド用のMakefileが用意されているので、それを使います。下記のようなMakefileを作り、ソースファイル名を設定して、`/lib/modules/4.9.41-v7+/build`にあるMakefileを呼びます。実際には4.9.41-v7+の部分は環境によって変わるので`uname`コマンドで取得します。

```Makefile:Makefile
obj-m := test.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
```

## ビルドして実行する
上述のtest.cとMakefileのあるディレクトリで、`make`します。すると、test.koが出来上がるはずです。その後、`insmod`でモジュールのロード、`rmmod`でモジュールのアンロードをしてみます。

```bash
make
sudo insmod test.ko
sudo rmmod test.ko
dmesg
```

最後に、dmesgでprintkで出力した結果を見てみます。すると、下記のように、実装したコードが実行されていることが分かります。

```
[ 2324.171986] Hello my module
[ 2327.753108] Bye bye my module
```

### ログについての注意点
printkをするときに`\n`がないと、dmesgのログに出力されないようです。どうも、`\n`のタイミングでdmesg用のバッファに出力しているようです。

printkは、ターミナルではなく、メモリ上のバッファにその内容を出力します。そのバッファはリングバッファのようになっているのでそのうち上書きされます。dmesgはそのバッファを出力しているだけです。ファイルとしては`cat /var/log/syslog`に保存されます。が、即時にこのファイルに書かれるのではなく、程よい頻度で書き込みが行われるようなので、ご注意ください。クラッシュした時などは、欲しいログが書き込まれていない、という可能性もあります。

# 複数ファイルから成るモジュールをビルドする
test01.cとtest02.cから、MyModule.koを作成する例です。以下のようなMakefileにします。`MyModule.o`を生成するのに必要な.oファイルを`MyModule-objs`で指定しています。そのため、`MyModule-objs`のプリフィックスは`MyModule`である必要があります。出来上がったモジュールは、`sudo insmod MyModule.ko`でロードできます。

```Makefile
CFILES = test01.c test02.c

obj-m := MyModule.o
MyModule-objs := $(CFILES:.c=.o)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
```

# おわりに
という感じで、少しずつまとめていこうと思います。ぶっちゃけ僕の勉強メモです。
何か間違えていることを書いていたら、ぜひ教えてください。



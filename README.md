# 本連載について
組み込みLinuxのデバイスドライバをカーネルモジュールとして開発するためのHowTo記事です。本記事の内容は全てラズパイ(Raspberry Pi)上で動かせます。

Linuxデバドラ開発は最初の一歩が難しいと思います。資料も少なく、たいていの人はオライリー本に手を出して挫折すると思います。(僕がそうでした。。。)

この記事では、「Linuxデバイスドライバプログラミング (平田 豊) 」の内容に沿って進めていこうと思います。この本は2008年発行と古いので、現在(2017年12月)の環境でも動くように、実際にRaspberry Piで動かしながら確認していこうと思います。(途中から、本の内容とは離れます)

また、出来るだけ簡単にしたかったので、クロス開発環境は整えず、ラズパイ本体で開発を行います。そのため、ホストOSが何であろうと、ラズパイを持っている方はすぐに試してみることが出来ます。(コーディングをWindows等のホストPCで行って、ラズパイにSFTP転送するという開発スタイルがおすすめです)

- [1回目: ビルド環境準備と、簡単なカーネルモジュールの作成](https://qiita.com/take-iwiw/items/1fdd2e0faaaa868a2db2)
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


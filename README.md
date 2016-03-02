# OnlineParty

オンライン通信ゲームのデモプログラムです。(通信できるかどうか確かめることが目的)

実行方法：
(1)https://github.com/yuni-net/OnlinePartyServerを起動する
(2)表示されたIPアドレスを、OnlineParty/data/config/Synchronizer.jsonの"server_ip"の値に書き込む
(3)OnlinePartyを起動する

ビルドには以下の2つのライブラリが必要です。
(1)https://github.com/yuni-net/simplect3D
(2)https://github.com/yuni-net/Fantomwaves

注意点：
複数のPC間で通信する場合はファイアウォールを切り、アンチウィルスソフトを無効にする必要があります

実装した工夫：
(1)UDPホールパンチング
(2)サーバーと時刻同期(Windowsの時刻を書き換えるわけではない)

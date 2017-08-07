# v4l2を用いたプログラミング

## Prepare

```bash
sudo apt-get install -y libv4l-dev v4l-utils qv4l2
```

## How to use

```bash
cd build
cmake ..
make

../v4ldevice
../listup [/dev/video*の番号]
../capture_one
```

## v4l device confirmation

```
qv4l2
```

![](image/qv4l2.png)

## v4l,opencv,TIS camera

`v4ldevice`ではThe Imaging Souce社のカメラをv4lを用いてキャプチャする．
`main.cpp`の`/dev/video`の番号を適宜いじる必要あり．
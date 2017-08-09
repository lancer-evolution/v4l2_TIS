# UDP Communication

TIS Camera with V4L driver.  
We use The Imaging Source camera "DFM22BUC03-ML".

## Prepare

1. v4l
```bash
sudo apt-get install -y libv4l-dev v4l-utils qv4l2
```
2. OpenCV
```bash
sudo apt-get update
sudo apt-get install libopencv-dev #2.4.9
```

## How to use

Build
```bash
cd build
cmake ..
make
```

Execute
* UDP sender  
raspiでは`imshow`しないようにしている．  
```bash
../streaming_send
or  
../raspberry_send  # For RaspberryPi3
```

* UDP Reciever
```bash
../streaming_recieve
```

それぞれ、カメラ画像を送りたいPCと受け取りたいPCで実行する．  
ただし，事前に設定する箇所がいくつかあるため，以下を参照する．

### 設定項目

1. IP adress (default: 192.168.0.236)  
streaming_send.cpp: `inet_addr`  
確認方法は受け取り側のPCで`ifconfig`し，inetアドレスを見る．
2. Video device (default: /dev/diveo0)  
streaming_send.cpp: `device_name`  
`ls /dev/video*`により確認できる．
3. Camera情報 (Width, Hight, Exposure, Gain, Format)  
v4lとOpenCVでキャプチャするために指定する．  
・ streaming_send.cpp: `init_device`,`cv::Mat src`,`imecode`  
・ v4ldevice.cpp: `fmt.fmt.pix.pixelformat`  
`init_device`の引数はwidth,hight,exposure．gainはデフォルトでmax:63が入っている．

**その他**  
* CV_IMWRITE_JPEG_QUALITY (max: 100)


## Only V4L

Open camera device and show
```bash
../v4ldevice
```
List camera information
```bash
../listup [/dev/video*の番号]
```
忘れた
```bash
../capture_one
```

## v4l device GUI confirmation

```bash
qv4l2
```

![](image/qv4l2.png)

#define Esc 27

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <vector>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "v4ldevice.h"

using namespace std;
using namespace cv;

int main(int argc, char** args){
  //ソケットの設定
  int sock;
  struct sockaddr_in addr;

  sock = socket(AF_INET, SOCK_DGRAM, 0);

  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);  //ポート番号
  addr.sin_addr.s_addr = inet_addr("192.168.0.236"); //送信先IPアドレス

  //カメラの設定
  // VideoCapture cap(0);
  // cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
  // cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
  // if(!cap.isOpened()){
  //     cout << "Camera not Found!" << endl;
  //     return -1;
  // }
  const char *device_name = "/dev/video0";
  open_device(device_name);
  init_device(744, 480, 50);
  start_capturing();
  unsigned char* ImageBuffer = NULL;
  cv::Mat src(480, 744, CV_8UC1);
  cv::Mat dst;

  Mat frame;
  Mat jpgimg;
  static const int sendSize = 65500;
  char buff[sendSize];
  vector<unsigned char> ibuff;
  vector<int> param = vector<int>(2);
  param[0] = CV_IMWRITE_JPEG_QUALITY;
  param[1] = 85;

  cv::namedWindow("Send", CV_WINDOW_AUTOSIZE);

  cout << "Capture Start" << endl;
  
  while(true){
	
	ImageBuffer = snapFrame();
	
	if (ImageBuffer != NULL) {
	  memcpy(src.data, ImageBuffer, src.step * src.rows); //バッファーをMatクラスに受け渡す．
	  cvtColor(src, dst, CV_BayerGB2RGB); //ここがBayer変換
	}else {
	  std::cerr << "no image buffer retrieved!" << std::endl;
	}
	cv::imshow("Send", dst);

	if ((const char)cv::waitKey(1) == Esc) {
	  cv::destroyAllWindows();
	  break;
	}

	// 送信
	imencode(".jpg", dst, ibuff, param);
	if(ibuff.size() < sendSize ){
	  for(int i=0; i<ibuff.size(); i++) buff[i] = ibuff[i];
	  sendto(sock, buff, sendSize, 0, (struct sockaddr *)&addr, sizeof(addr));
	  jpgimg = imdecode(Mat(ibuff), CV_LOAD_IMAGE_COLOR);
	}
	ibuff.clear();
	
  }
  close(sock);
  cout << "Capture Stop" << endl;
}

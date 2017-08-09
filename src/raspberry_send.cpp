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

#include <stdlib.h>
#include <signal.h>

#include "v4ldevice.h"

using namespace std;
using namespace cv;

class UDP_sender{
public:
  static void SigHandler(int p_signame);
  void SetSignal(int p_signame);
  void send(Mat dst);

  UDP_sender();
  ~UDP_sender();

  //ソケットの設定
  int sock;
  struct sockaddr_in addr;

  Mat jpgimg;
  static const int sendSize = 65500;
  char buff[sendSize];
  vector<unsigned char> ibuff;
  vector<int> param;
};
UDP_sender::UDP_sender(){
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock == -1){
	cout << "Could not create socket" << endl;
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);  //ポート番号
  addr.sin_addr.s_addr = inet_addr("10.42.0.240"); //送信先IPアドレス

  //sendSize = 65500;
  param = vector<int>(2);
  param[0] = CV_IMWRITE_JPEG_QUALITY;
  param[1] = 85;
 
}
UDP_sender::~UDP_sender(){
  close(sock);
  cout << "Capture Stop" << endl;
}

void UDP_sender::SetSignal(int p_signame)
{

  if(signal(p_signame, UDP_sender::SigHandler) == SIG_ERR) {
	/* シグナル設定エラー  */
	//printf('シグナルの設定が出来ませんでした。終了します\n');
	exit(1);
  }

  return;
}

/* シグナル受信/処理 */
void UDP_sender::SigHandler(int p_signame)
{

  cout << "Capture Stop" << endl;
  exit(0);
  // static int   sig_cnt = 0;

  // ++sig_cnt;
  // if (sig_cnt <= 2) {
  // 	printf('%d回目の割り込みです。無視します\n'， sig_cnt);
  // }
  // else {
  // 	printf('%d回目の割り込みです。終了します\n'， sig_cnt);
  // 	exit(0);
  // }

  /* シグナルの再設定 */
  //SetSignal(p_signame);

  return;
}
void UDP_sender::send(Mat dst){
  imencode(".jpg", dst, ibuff, param);
  if(ibuff.size() < sendSize ){
	for(int i=0; i<ibuff.size(); i++) buff[i] = ibuff[i];
	sendto(sock, buff, sendSize, 0, (struct sockaddr *)&addr, sizeof(addr));
	jpgimg = imdecode(Mat(ibuff), CV_LOAD_IMAGE_COLOR);
  }
  ibuff.clear();
  
}


int main(int argc, char** args){
  UDP_sender udp_sender;

  //カメラの設定
  const char *device_name = "/dev/video0";
  open_device(device_name);
  init_device(744, 480, 50);
  start_capturing();
  unsigned char* ImageBuffer = NULL;
  cv::Mat src(480, 744, CV_8UC1);
  cv::Mat dst;

  //cv::namedWindow("Send", CV_WINDOW_AUTOSIZE);

  cout << "Capture Start" << endl;
  
  while(true){
	udp_sender.SetSignal(SIGINT);
	
	ImageBuffer = snapFrame();
	
	if (ImageBuffer != NULL) {
	  memcpy(src.data, ImageBuffer, src.step * src.rows); //バッファーをMatクラスに受け渡す．
	  cvtColor(src, dst, CV_BayerGB2RGB); //ここがBayer変換
	  //cvtColor(dst, dst, CV_RGB2GRAY);// gray scale
	}else {
	  std::cerr << "no image buffer retrieved!" << std::endl;
	}
	//cv::imshow("Send", dst);

	if ((const char)cv::waitKey(1) == Esc) {
	  cv::destroyAllWindows();
	  break;
	}

	// 送信
	udp_sender.send(dst);
	//cout << "sended" << endl;
  }

}

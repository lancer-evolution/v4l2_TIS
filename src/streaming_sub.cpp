#define Esc 27

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <vector>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
// エラー処理関係
#include <errno.h>
#include <sys/ioctl.h>


using namespace std;
using namespace cv;
int main() {
  int sock, numrcv, val;
  struct sockaddr_in addr;
  
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  
  addr.sin_family = AF_INET;
  addr.sin_port = htons(12345);
  addr.sin_addr.s_addr = /*inet_addr("10.42.0.1");*/INADDR_ANY;

  // ソケットの生成
  bind(sock, (struct sockaddr *)&addr, sizeof(addr));
  val = 1;
  ioctl(sock, FIONBIO, &val); // ノンブロッキング

  cvNamedWindow("Receive", CV_WINDOW_AUTOSIZE);
  cv::Mat image;// = cv::Mat(480,744,CV_8UC3);
  
  static const int receiveSize = 65500;
  static char buff[receiveSize];
  vector<uchar> ibuff;


  while(numrcv < 1){
	numrcv = recv(sock, buff, receiveSize, 0);
	if (errno == EAGAIN) {
	  cout << "Waiting for data." << endl;
	  sleep(1);
	} else {
	  cerr << "recv" << endl;
	  break;
	}
  }
  
  while(true){
	//while(cvWaitKey( 10 ) == -1){

	numrcv = recv(sock, buff, receiveSize, 0);

	for(int i=0; i<sizeof(buff) ; i++){
	  ibuff.push_back((uchar)buff[i]);
	}
	//cout << "numrcv:" <<numrcv << endl;
	// if(numrcv == -1){
	//   cout << "No buffer" << endl;
	//   break;
	// }
	  
	image = imdecode(Mat(ibuff), CV_LOAD_IMAGE_COLOR);//CV_LOAD_IMAGE_GRAYSCALE
	cv::imshow("Receive", image);
	ibuff.clear();

	
	// escで終了
	if ((const char)cv::waitKey(1) == Esc) {
	  cv::destroyAllWindows();
	  break;
	}
	
  }
  ibuff.clear();
  close(sock);
  cout << "Reciever Stoped" << endl;

  return 0;
}

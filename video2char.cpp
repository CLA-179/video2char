#include <iostream>
#include <cstring>
#include <windows.h> 
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

#define FPS_INTERVAL 1//帧间隔
#define W 75//宽
#define H 42//高
const char* filepath = "./*.mp4";//视频路径

HWND hwnd=GetForegroundWindow();
HANDLE hOutput;
COORD coord={0,0};
//根据灰度对应字符列表
const char *charList = "$@B%8&WM#*ZO0QLCJUYXhkwmxzvaunocr/\\|()1{}[]?Ili+!;:-_,\"^`'.  ";
int main(){
    hOutput=GetStdHandle(STD_OUTPUT_HANDLE);
    VideoCapture cap;
    cap.open(filepath);
    if (!cap.isOpened())//判断是否正常打开
	{
		cout << "cannot open video!" << endl;
		exit(0);
    }
    //获取视频参数及计算
    double fps = cap.get(CAP_PROP_FPS);
    int timeSleep = int(((1 / fps) * FPS_INTERVAL) * 1000) - 11;
    int len = strlen(charList);
    double unit = (256.0 + 1) / len;
    int i, j, k = 0, grayNum;
    Mat frame, image, gray;
    char text[W * (H * 2 + 1) + 1] = {'\0'};
    //cout << charList << endl << fps;
    ShowWindow(hwnd,SW_MAXIMIZE);//窗口最大化
    Sleep(500);
    for (int fpsCount = 1; 1; fpsCount++)
    {
        if (cap.read(frame) && fpsCount % FPS_INTERVAL == 0)//读取一帧
        {
            resize(frame, image, Size(W * 2, H));//宽度为设定值的两倍
            cvtColor(image, gray, COLOR_BGR2GRAY);   
            for ( i = 0; i < H; i++, k++)
            {
                for ( j = 0; j < W * 2; j++, k++)
                {
                    grayNum = gray.data[i * W * 2 + j];//(i,j)位置的灰度值，也可用gray.at<uchar>(i, j);
                    text[k] = charList[int(grayNum / unit)];//根据灰度对应字符
                }
                text[k] = '\n';
            }
            text[k] = '\0';
            k = 0;
            SetConsoleCursorPosition(hOutput, coord);//使光标位于(0,0)代替CLS，避免闪屏
            //system("cls");
            cout << text;
            Sleep(timeSleep);
        }else if (!cap.read(frame)) break;
    }
    cap.release();
}




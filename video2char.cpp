#include <iostream>
#include <cstring>
#include <ctime>
#include <windows.h> 
#include <opencv2/opencv.hpp>
#include <chrono>


using namespace std;
using namespace cv;

#define FIRST_DELAY_TIME 500
#define RUN_DELAY 0 //主要是Sleep函数产生的延迟，根据播放效果调整
#define FPS_INTERVAL 1//帧间隔
#define W 75//宽
#define H 42//高
const char* filepath = "";//视频路径
HWND hwnd=GetForegroundWindow();
HANDLE hOutput, hOutBuf;
COORD coord={0,0};
//根据灰度对应字符列表
const char *charList = "$@B%8&WM#*ZO0QLCJUYXhkwmxzvaunocr/\\|()1{}[]?Ili+!;:-_,\"^`'.  ";
void DubleBufInit();
void HideCursor();//隐藏光标函数
void delay(int timeout_ms);


int main(){
    HideCursor();
    hOutput=GetStdHandle(STD_OUTPUT_HANDLE);
    VideoCapture cap;
    cap.open(filepath);
    if (!cap.isOpened())//判断是否正常打开
	{
		cout << "cannot open video!" << endl;
        system("pause");
		exit(0);
    }
    //获取视频参数及计算
    double fps = cap.get(CAP_PROP_FPS);
    int perFpsTime = int(((1 / fps) * FPS_INTERVAL) * 1000);
    int sleepTime = perFpsTime; 
    int len = strlen(charList);
    double unit = (256.0 + 1) / len;
    int i, j, k = 0, grayNum;
    Mat frame, image, gray;
    DWORD bytes=0;
    char text[W * (H * 2 + 1) + 1] = {'\0'};
    char data[W * (H * 2 + 1) + 1];
    //cout << charList << endl << fps;
    ShowWindow(hwnd,SW_MAXIMIZE);//窗口最大化
    Sleep(FIRST_DELAY_TIME);
    for (int fpsCount = 1; 1; fpsCount++)
    {
        clock_t t1 = clock();
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
            SetConsoleCursorPosition(hOutput, coord);//使光标位于(0,0)
            // system("cls");
            cout << text;
            ReadConsoleOutputCharacterA(hOutput, data, W * (H * 2 + 1) + 1, coord, &bytes);
            WriteConsoleOutputCharacterA(hOutBuf, data, W * (H * 2 + 1) + 1, coord, &bytes);//双缓冲技术
            clock_t t2 = clock();
            sleepTime = perFpsTime - (t2 - t1);
            // cout << "\n" << timeSleep - (t2 - t1) << "\t" << (t2 - t1) ;
            delay(sleepTime);
        }else if (!cap.read(frame)) break;
        
    }
    cap.release();
}
void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor;    
	cursor.bVisible = FALSE;    
	cursor.dwSize = sizeof(cursor);    
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);    
	SetConsoleCursorInfo(handle, &cursor);
}
void delay(int timeout_ms)
{
  auto start = std::chrono::system_clock::now();
  while (true)
  {
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count();
    if (duration > timeout_ms)
    {
      //LOGGING_ERROR("timeout occurred,timeout %d ms", timeout_ms);
      break;
    }
}
}
void  DubleBufInit(){
    hOutput=GetStdHandle(STD_OUTPUT_HANDLE);
 
    //创建新的缓冲区
    hOutBuf = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE, 
        FILE_SHARE_READ | FILE_SHARE_WRITE, 
        NULL, 
        CONSOLE_TEXTMODE_BUFFER, 
        NULL
    );
 
    //设置新的缓冲区为活动显示缓冲
    SetConsoleActiveScreenBuffer(hOutBuf);
 
    //隐藏两个缓冲区的光标
    CONSOLE_CURSOR_INFO cci;
    cci.bVisible=0;
    cci.dwSize=1;
    SetConsoleCursorInfo(hOutput, &cci);
    SetConsoleCursorInfo(hOutBuf, &cci);
}

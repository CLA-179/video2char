# video2char
Play a video in console。
在命令行以字符画的方式播放视频。
<br><br>
需要安装python或C++版本的opencv
<br>
需要修改以下内容
##python
```python
interval = 1  # 帧间隔
path = "./op.mp4" # 地址
W = 75 # 宽
H = 42 # 高
```
##C++
```C++
#define FPS_INTERVAL 1//帧间隔
#define W 75//宽
#define H 42//高
const char* filepath = "./op.mp4";//视频路径
```
帧间隔设为n即每n帧播放一次，最小为1.

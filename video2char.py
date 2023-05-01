import cv2
import numpy as np
import os
import time

interval = 1  # 帧间隔
path = "./*.mp4"
W = 75
H = 42
# 根据灰度对应字符列表
ascii_char = list("$@B%8&WM#*ZO0QLCJUYXhkwmxzvaunocr/\\|()1{}[]?Ili+!;:-_,\"^`'.  ") 
# 打开视频
cap = cv2.VideoCapture(path)  
fps = cap.get(cv2.CAP_PROP_FPS)
timeSleep = (1 / fps) * interval - 0.019
#字符
def GetChar(gray_number): 
    length = len(ascii_char) 
    unit = (256.0 + 1) / length 
    return ascii_char[int(gray_number / unit)]  #灰度对应字符
fpsCount = 0
while (True):
    fpsCount += 1
    (frameState, frame) = cap.read()  # 读帧
    if frameState == True and (fpsCount % interval==0):
        image = cv2.resize(frame,(2 * W, H))  # 长宽比为原视频的二倍
        gray = cv2.cvtColor(image,cv2.COLOR_BGR2GRAY)
        txt = ""
        for i in range(image.shape[0]): 
            for j in range(image.shape[1]): 
                txt += GetChar(gray[i, j])
            txt += '\n'
        os.system("cls")
        print( txt )
        time.sleep(timeSleep)
    elif frameState == False:
        break
cap.release()
import cv2
import numpy as np
import time
import serial

ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=0.01)
cap2 = cv2.VideoCapture(0)
cap = cv2.VideoCapture(2)

cap.set(cv2.CAP_PROP_FOURCC,cv2.VideoWriter_fourcc('M','J','P','G'))
cap2.set(cv2.CAP_PROP_FOURCC,cv2.VideoWriter_fourcc('M','J','P','G'))

while(True):
   time.sleep(0.1)
   ret, frame = cap.read()
   frame = cv2.resize(frame,(320,240))
   hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
   daon_lower = np.array([49, 93, 38],np.uint8) 
   daon_upper = np.array([65,136,255],np.uint8)
   daon = cv2.inRange(hsv, daon_lower, daon_upper)
   kernal = np.ones((5, 5), "uint8")
   daon = cv2.dilate(daon, kernal)
   sobel = cv2.Laplacian(daon, cv2.CV_64F)
   gray = cv2.cvtColor(hsv, cv2.COLOR_BGR2GRAY)
   blurred = cv2.GaussianBlur(daon, (11, 11), 0)
   edged = cv2.Canny(blurred, 1, 1)
   res_daon = cv2.bitwise_and(frame, frame, mask = daon)
   (contours, hierarchy)=cv2.findContours(daon, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
   for pic, contour in enumerate(contours):
       area = cv2.contourArea(contour)
       if(area > 300):
           x, y, w, h = cv2.boundingRect(contour)
           frame = cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 255, 0), 2)
           cv2.putText(frame, "warna daun", (x, y), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 0))
   kontur = cv2.findContours(daon.copy(),cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)[-2]
   print(len(kontur))
   if len(kontur)>20:
      print("daun terdeteksi                              1")
      ser.write(chr(1).encode())
   if len(kontur)<19:
      print("tidak ada daun")
      ser.write(chr(0).encode())   
   
   key = cv2.waitKey(1) & 0xFF
   if key == ord("q"):
       break

   ret, frame2 = cap2.read()
   frame2 = cv2.resize(frame2,(320,240))
   hsv2 = cv2.cvtColor(frame2, cv2.COLOR_BGR2HSV)
   daon_lower2 = np.array([49, 93, 38],np.uint8) 
   daon_upper2 = np.array([65,136,255],np.uint8)
   daon2 = cv2.inRange(hsv2, daon_lower2, daon_upper2)
   kernal2 = np.ones((5, 5), "uint8")
   daon2 = cv2.dilate(daon2, kernal2)
   sobel2 = cv2.Laplacian(daon2, cv2.CV_64F)
   gray2 = cv2.cvtColor(hsv2, cv2.COLOR_BGR2GRAY)
   blurred2 = cv2.GaussianBlur(daon2, (11, 11), 0)
   edged2 = cv2.Canny(blurred2, 1, 1)
   res_daon2 = cv2.bitwise_and(frame2, frame2, mask = daon2)
   (contours2, hierarchy2)=cv2.findContours(daon2, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
   for pic, contour2 in enumerate(contours2):
       area2 = cv2.contourArea(contour2)
       if(area2 > 300):
           x, y, w, h = cv2.boundingRect(contour2)
           frame2 = cv2.rectangle(frame2, (x, y), (x + w, y + h), (255, 255, 0), 2)
           cv2.putText(frame2, "warna daun2", (x, y), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 0))
   kontur2 = cv2.findContours(daon2.copy(),cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)[-2]
   print(len(kontur2))
   if len(kontur2)>12:
      print("daun terdeteksi             2")
      ser.write(chr(3).encode())
   if len(kontur2)<11:
      print("tidak ada daun2")
      ser.write(chr(2).encode())   

   
   key2 = cv2.waitKey(1) & 0xFF
   if key2 == ord("a"):
       break
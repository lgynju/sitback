#!/usr/bin/env python3
# -*- coding: UTF-8 -*-

import cv2
import RPi.GPIO as GPIO
import time
import os

port = 17
thresholds = 0.125
GPIO.setmode(GPIO.BCM)
GPIO.setup(port, GPIO.OUT)

try:
    webcam = cv2.VideoCapture(0)
    webcam.set(cv2.CAP_PROP_BUFFERSIZE, 1)
    # Loading the required haar-cascade xml classifier file
    haar_cascade = cv2.CascadeClassifier(os.path.dirname(os.path.abspath(__file__)) +  '/haarcascade_frontalface_alt2.xml')
    sleep_time = 0.5
    while(True):
        ret, img = webcam.read()
        # Converting image to grayscale
        gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        gray_img = cv2.pyrDown(gray_img)
        gray_img = cv2.pyrDown(gray_img)

        # Applying the face detection method on the grayscale image
        faces_rect = haar_cascade.detectMultiScale(gray_img)

        # Get the area of max face and calculate the ratio
        max_face_area = 0
        for (x, y, w, h) in faces_rect:
            if w * h > max_face_area:
                max_face_area = w*h
        height, width = gray_img.shape
        current_ratio = max_face_area/(height * width)
        # Check the result.
        # To avoid false triggering and save power, a dynamic-change sleep_time mechanisim is designed here.
        if thresholds < current_ratio and current_ratio < 0.2:
            sleep_time = sleep_time / 3
        else:
            sleep_time = sleep_time * 1.2
        if sleep_time < 0.5: # sleep_time is the indicator to trigger notification
            sleep_time = 0.5
            GPIO.output(port,True) # Use the beep to alert user.
            time.sleep(0.002)
            GPIO.output(port,False)
        if sleep_time > 5:
            sleep_time = 5

        print("ratio: %.2f"%current_ratio, " thresholds: ", thresholds, " sleeptime: %.2f"%sleep_time)
        time.sleep(sleep_time)
except:
    print("abnormal exit.")
finally:
    GPIO.output(port,False)
    GPIO.cleanup()

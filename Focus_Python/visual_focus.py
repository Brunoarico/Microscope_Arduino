#!/usr/bin/env python3
import numpy as np
import cv2
from imutils import paths
from vidstab import VidStab
import threading



cap = cv2.VideoCapture(1)

def variance_of_laplacian(image):
	return cv2.Laplacian(image, cv2.CV_64F).var()

def getImage ():

    while(True):
        # Capture frame-by-frame
        ret, frame = cap.read()
        frame = cv2.resize(frame, None,fx=0.7, fy=0.7, interpolation = cv2.INTER_CUBIC)
        height, width, channels = frame.shape
        # Our operations on the frame come here
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        fm = variance_of_laplacian(gray)
        # Display the resulting frame
        cv2.rectangle(frame, (0, height-10), (int(fm*1.6), 1040), (0,0,255), thickness=cv2.FILLED)
        cv2.imshow('frame',frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    # When everything done, release the capture
    cap.release()
    cv2.destroyAllWindows()


getImage()

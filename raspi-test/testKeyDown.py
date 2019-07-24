#!/usr/bin/env python
#!coding=utf-8
#author:ksc

import RPi.GPIO as GPIO
import time
import os,sys
import signal


#define GPIO pin
pin_btn=19;

GPIO.setmode(GPIO.BCM)
GPIO.setup(pin_btn, GPIO.IN, pull_up_down=GPIO.PUD_UP)
def onKeyDown(channel):
    global led_status;
    os.system("shutdown -h now")
    print('Key down');

GPIO.add_event_detect(pin_btn, GPIO.FALLING, callback= onKeyDown,bouncetime=500)
try:
    while True:
        time.sleep(100000000);
except KeyboardInterrupt:
    print('User press Ctrl+c ,exit;')
finally:
    print('clean up');
    GPIO.cleanup();

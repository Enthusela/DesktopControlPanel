#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
picdir = os.path.abspath("/home/nathanw/Documents/cs50_final_project/eink/bmps")
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import logging
import epd7in3g
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

logging.basicConfig(level=logging.DEBUG)

try:        
    logging.info("wordclock_test.py")
    epd = epd7in3g.EPD()   
    logging.info("Init")
    epd.init()
    logging.info("Clear")
    epd.Clear()

    # read bmp file 
    logging.info("Clearing and displaying bmp file...")
    Himage = Image.open(os.path.join(picdir, 'capture_wc.bmp'))
    epd.display(epd.getbuffer(Himage))
    logging.info("Sleeping three seconds...")
    epd.sleep()
    time.sleep(3)

        
except IOError as e:
    logging.info(e)
    
except KeyboardInterrupt:    
    logging.info("ctrl + c:")
    epd7in3g.epdconfig.module_exit()
    exit()

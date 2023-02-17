#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
picdir = os.path.abspath("/home/nathanw/Documents/cs50_final_project/eink/bmp")
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

test_image_path = '/home/nathanw/Documents/e-Paper/RaspberryPi_JetsonNano/python/pic/7.3inch-3.bmp'

import logging
import epd7in3g
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

logging.basicConfig(level=logging.DEBUG)

try:        
    logging.info("eink_test.py")
    epd = epd7in3g.EPD()   
    logging.info("Init")
    epd.init()
    logging.info("Clear")
    epd.Clear()

    # Read bmp file 
    logging.info("Getting bmp file...")
    # Himage = Image.open(os.path.join(picdir, 'capture_nasa.bmp'))
    with Image.open(os.path.abspath(test_image_path)) as img:
        logging.info("Displaying bmp file...")
        epd.display(epd.getbuffer(img))
        logging.info("Sleeping three seconds...")
    epd.sleep()
    epd7in3g.epdconfig.module_exit()
    exit()
        
except IOError as e:
    logging.info(e)
    
except KeyboardInterrupt:    
    logging.info("ctrl + c:")
    epd7in3g.epdconfig.module_exit()
    exit()

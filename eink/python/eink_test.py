#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os

# test_image_path = '/home/nathanw/Documents/e-Paper/RaspberryPi_JetsonNano/python/pic/7.3inch-3.bmp'
test_image_path = '/home/nathanw/Documents/cs50_final_project/eink/bmp/world_map.bmp'

import logging
import epd7in3f
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

logging.basicConfig(level=logging.DEBUG)

try:        
    logging.info("eink_test.py")
    epd = epd7in3f.EPD()   
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
        
except IOError as e:
    logging.info(e)
    
except KeyboardInterrupt:    
    logging.info("ctrl + c:")
    epd7in3f.epdconfig.module_exit()
    exit()

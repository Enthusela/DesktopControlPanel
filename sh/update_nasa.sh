echo "Running test nasa puppeteer script..."
node /home/nathanw/Documents/cs50_final_project/js/nasa_test.js
echo "Converting screenshot to bmp..."
convert -rotate 180 /home/nathanw/Documents/cs50_final_project/eink/png/capture_nasa.png /home/nathanw/Documents/cs50_final_project/eink/bmp/capture_nasa.bmp
echo "Pushing bmp to display..."
python /home/nathanw/Documents/cs50_final_project/eink/python/nasa_test.py
echo "Done."
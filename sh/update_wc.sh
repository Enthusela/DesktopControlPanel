echo "Running test wordclock puppeteer script..."
node /home/nathanw/Documents/cs50_final_project/js/dcp_test.js
echo "Converting wordclock screenshot to bmp..."
convert -rotate 180 /home/nathanw/Documents/cs50_final_project/eink/png/capture_wc.png /home/nathanw/Documents/cs50_final_project/eink/bmp/capture_wc.bmp
echo "Pushing wordclock to display..."
python /home/nathanw/Documents/cs50_final_project/eink/python/wordclock_test.py
echo "Done."
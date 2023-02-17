const puppeteer = require('puppeteer-core');

async function timeout(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

(async () => {
    // As the pages are hosted locally, we need to make a CORS request to get some of the data,
    // hence the need to run Chromium with the "--disable-web-security" flag.
    
    const args = puppeteer.defaultArgs();
    args.push('--disable-web-security');
    const browser = await puppeteer.launch({ignoreDefaultArgs: true, args, executablePath: '/usr/bin/chromium-browser'})
    const page = await browser.newPage();
    await page.goto('file:///home/nathanw/Documents/cs50_final_project/wordclock/wc.html');
    // 5-second timeout: allows the page to fully render before taking the screenshot
    
    await timeout(5000);

    // Browser viewport set to the dimensions of the e-ink display
    
    await page.setViewport({width: 800, height: 480});
    const date = new Date;
    const date_suffix = date.toDateString() + '_' + date.toTimeString();
    await page.screenshot({path: '/home/nathanw/Documents/cs50_final_project/screenshots/capture_wc.png'});
    await browser.close();
})();
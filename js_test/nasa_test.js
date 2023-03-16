const nasa_api_url = 'https://api.nasa.gov/planetary/apod?api_key=P4QR0evI0Lad2voq0JeB8siyXfdDyneZ1At3T7se';

async function getapi(url) {
    const response = await fetch(url);
    var data = await response.json();
    if (response) {
        return data;
    } else {
        console.log("getapi(): No response.")
        return null;
    }
}

async function timeout(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

(async () => {
    const request = require('request');
    const puppeteer = require('puppeteer-core');
    const eink_width = 800;
    const eink_height = 480;
    const scrnsht_path = '/home/nathanw/Documents/cs50_final_project/eink/png/capture_nasa.png';
    
    // As the pages are hosted locally, we need to make a CORS request to get some of the data,
    // hence the need to run Chromium with the "--disable-web-security" flag.
    const args = puppeteer.defaultArgs();
    args.push('--disable-web-security');
    const browser = await puppeteer.launch({ignoreDefaultArgs: true, args, executablePath: '/usr/bin/chromium-browser'})
    const page = await browser.newPage();
    
    // Get NASA Astronomy Image of the Day

    console.log("Getting NASA AIoD URL...");
    nasa_data = await getapi(nasa_api_url);
    
    if (nasa_data) {
        // Render image in Chromium and screenshot at e-ink display dimensions
        console.log("Going to NASA AIoD...");
        await page.goto(nasa_data.url);
        await timeout(5000); // Allows page to fully render before screenshot
        console.log('Setting viewport size...')
        await page.setViewport({width: eink_width, height: eink_height});
        console.log('Saving screenshot...')
        await page.screenshot({path: scrnsht_path});
        console.log('Done.')
    } else {
        console.log("Could not get NASA AIoD: exiting.")
    }

    await browser.close();

})();
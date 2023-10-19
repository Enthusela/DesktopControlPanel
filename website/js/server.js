const sharp = require('sharp');
const fs = require('fs');
const path = require('path');
const express = require('express');
const app = express();
app.set('view engine', 'ejs');
const port = 8000;

// Serve static files from the public folder
app.use('/public', express.static('public'));
app.set('views', __dirname + '/public/views')

// Render routes
app.get('/', (req, res) => {
  res.render('pages/index');
});

app.get('/brief', (req, res) => {
    res.render('pages/brief');
});

app.get('/gallery', (req, res) => {
    res.render('pages/gallery');
});

app.get('/parts', (req, res) => {
    res.render('pages/parts');
});

app.get('/timeline', (req, res) => {
    res.render('pages/timeline');
});

app.get('/posts/:filename', (req, res) => {
    const filename = req.params.filename;
    const filePath = path.join(__dirname, '/public/posts', filename + '.ejs');
    console.log(filePath);

    ejs.renderFile(filePath, {}, {} , (err, html) => {
        if (err) {
            res.status(500).json({ error: 'Error reading file' });
        } else {
            res.json({ content: html });
        }
    });
});

async function makeThumbnail(hiresPath, thumbPath) {
    console.log(`Making thumbnail from ${hiresPath}`);
    try {
        const metadata = await sharp(hiresPath).metadata();
        const maxDimension = 720;
        const scaleFactor = Math.min(maxDimension / metadata.width, maxDimension / metadata.height);
        // failOn: 'error' solves issue loading jpegs from Samsung phones
        await sharp(hiresPath, {failOn: 'error' })
        .resize({
            height: Math.round(metadata.height * scaleFactor),
            width: Math.round(metadata.width * scaleFactor)
        })
        .withMetadata()
        .toFile(thumbPath);
    } catch (error) {
        throw new Error(`Error: makeThumbnail(): ${error}`);
    }
}

async function readImageThumbs(highResDir, thumbDir) {
    fs.readdir(highResDir, async function (err, files) {
        if (err) {
            console.error("Could not list the directory.", err);
            process.exit(1);
        }
        files.forEach(async function (file, index) {
            var thumbPath = path.join(thumbDir, file);             // for use in JS
            if(fs.existsSync(thumbPath)) {
                return;
            }
            
            var hiresPath = path.join(highResDir, file);
            if (!fs.existsSync(hiresPath)) {
                console.error(`Thumbnail and high-res image missing for ${thumbPath}: cannot generate thumbnail.`)
                return;
            }

            try {
                console.log(`Thumbnail missing for ${thumbPath}: generating...`)
                await makeThumbnail(hiresPath, thumbPath);
                console.log(`Thumbnail generation for ${thumbPath} succeeded.`)
                // console.log(`thumbsHTMLPath: ${thumbsHTMLPath}`)
            } catch (error) {
                console.error(`Thumbnail generation for ${thumbPath} failed: ${error}`);
            }
        });
    });
    return 0;
}

async function getMissingThumbs () {
    console.log('Starting generating missing thumbnails...');
    const highResDir = path.join(__dirname, 'public/images/hires');
    const thumbDir = path.join(__dirname, 'public/images/thumbs');
    
    try {
        await readImageThumbs(highResDir, thumbDir);
        console.log(`Finished generating missing thumbnails.`);
    } catch (error) {
        console.error(`Error during generation of thumbnails: ${error}`);
    }
}


getMissingThumbs();
// Listen on port 8000
app.listen(port, () => {
    console.log(`Server listening on port ${port}.`)
});


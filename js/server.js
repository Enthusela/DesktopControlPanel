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

app.get('/thumbs/:imgname', async (req, res) => {
    const imgName = req.params.imgname;
    const thumbnailHTMLPath = `/public/imgs/thumbs/${imgName}`;
    const thumbnailAbsolutePath = path.join(__dirname, 'public/imgs/thumbs', imgName);
    if (false && fs.existsSync(thumbnailAbsolutePath)) {
        res.setHeader('Thumb-Path', thumbnailHTMLPath);
        res.end('ok');
        return;
    }
    const highResPathAbsolute = path.join(__dirname, 'public/imgs/hires', imgName);
    if (!fs.existsSync(highResPathAbsolute)) {
        res.status(500).send('High-res image not found, cannot get thumbnail.')
        return;
    }
    try {
        await makeThumbnail(highResPathAbsolute, thumbnailAbsolutePath);
        res.setHeader('Thumb-Path', thumbnailHTMLPath);
        res.end('ok');
    } catch (error) {
        res.status(500).send(`/thumbs/:imgname failed: ${error}`);
    }
});

async function makeThumbnail(hiresPath, thumbPath) {
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

// Listen on port 8000
app.listen(port, () => {
    console.log(`Server listening on port ${port}.`)
});
document.addEventListener('DOMContentLoaded', async () => {
    const entryHeadings = document.querySelectorAll('.journal-heading');
    entryHeadings.forEach((heading) => {
        heading.addEventListener('click', () => toggleEntry(heading));
    });

    const monthHeadings = document.querySelectorAll('.month-heading');
    monthHeadings.forEach((heading) => {
        heading.addEventListener('click', () => toggleMonth(heading));
    });
});

async function toggleEntry(heading) {
    const entry = heading.parentElement;
    const body = getOrCreateJournalBody(entry);
    if (body.style.display === 'block') {
        hideAndClearJournalBody(body);
    } else if (body.style.display === 'none' || body.style.display === '') {
        try {
            await fetchJournalBody(entry);
            await fetchImageThumbs(entry);
            body.style.display = 'block';
        } catch (error) {
            console.error("Error: ", error);    
        }
    }
}

function getOrCreateJournalBody(journalEntry) {
    if (journalEntry.querySelector('.journal-body') === null) {
        elem = document.createElement("div");
        elem.classList.add("journal-body");
        journalEntry.appendChild(elem);
    }
    return journalEntry.querySelector('.journal-body');
}

function hideAndClearJournalBody(body) {
    // Check for body as journal-entry elems load in without a journal-body elem
    if (body === null) {
        console.warn('Warning: Attempted to hide and clear null journal-body elem');
        return;
    }
    body.style.display = 'none';
    if (body.innerHTML.trim() !== '') {
        body.innerHTML = '';
    }
}

async function fetchJournalBody(entry) {
    const body = entry.querySelector('.journal-body');
    if (body === null) {
        throw new Error("Journal-entry elem has no journal-body elem. Content will not be fetched.");
    }
    const filename = entry.getAttribute('data-filename');
    const response = await fetch(`/public/posts/${filename}` + `.ejs`);
    if (!response.ok) {
        throw new Error('Network response was not ok');
    }
    const html = await response.text();
    if (!html) {
        throw new Error('Could not parse journal body html to text');
    } else if (html.trim() === '') {
        throw new Error('Journal body html is empty');
    }
    body.innerHTML = html;
}

async function fetchImageThumbs(entry) {
    const imgs = entry.querySelectorAll('img[srcname]');
    const srcNames = [];

    imgs.forEach((img) => {
        srcNames.push(img.getAttribute('srcname'));
    });

    const responses = await Promise.all(srcNames.map(async (srcName) => {
        const response = await fetch(`thumbs/${srcName}`);
        return response;
    }));

    responses.forEach((response, i) => {
        if (response.ok) {
            const src = response.headers.get('Thumb-Path');
            imgs[i].setAttribute('src', src);
        } else {
            console.error(`Failed to fetch thumbnail for ${srcNames[i]}`);
        }
    });
}

function toggleMonth(heading) {
    const month = heading.parentElement;
    const entries = month.querySelectorAll('.journal-entry');
    toggleJournalEntryGroup(entries);
}

// Toggles groups of journal-entry elems. All child journal-body elems are hidden and cleared.
function toggleJournalEntryGroup(entries) {
    entries.forEach((entry) => {
        entry.style.display = entry.style.display === 'none' ? 'block' : 'none';
        hideAndClearJournalBody(entry.querySelector('.journal-body'));
    });
}
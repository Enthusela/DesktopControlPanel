document.addEventListener('DOMContentLoaded', () => {
    const journalHeadings = document.querySelectorAll('.journal-heading');
    journalHeadings.forEach((heading) => {
        heading.addEventListener('click', () => toggleEntry(heading));
    });

    const monthHeadings = document.querySelectorAll('.month-heading');
    monthHeadings.forEach((heading) => {
        heading.addEventListener('click', () => toggleMonthHeading(heading));
    });

});

// Implement toggleEntry_oneEntry() but set all journal-body elements in entry to block
function toggleEntry(heading) {
    const journalEntries = document.querySelectorAll('.journal-entry');
    journalEntries.forEach((entry) => {
        const entryHeading = entry.querySelector('.journal-heading');
        const entryBody = entry.querySelector('.journal-body');
        if (entryHeading === heading) {
            entryBody.style.display = entryBody.style.display === 'none' ? 'block' : 'none';
        } else {
            entryBody.style.display = 'none';
        }
    });
}

function toggleMonthHeading(heading) {
    const journalMonths = document.querySelectorAll('.journal-month');
    journalMonths.forEach((journalMonth) => {
        const entries = journalMonth.querySelectorAll('.journal-entry');
        const entryHeading = journalMonth.querySelector('.month-heading');
        if (entryHeading === heading) {
            entries.forEach((entry) => {
                entry.style.display = entry.style.display === 'none' ? 'block' : 'none';
            });
        } else {
            entries.forEach((entry) => {
                entry.style.display = 'none';
            });
        }
    });
}

// Function to search a given journal-body element for images and render their low-res versions
function renderLowResImages(entry) {
    const images = entry.querySelectorAll('img');
    images.forEach((image) => {
        const imageName = image.getAttribute('src');
        const lowResImage = getLowResImage(imageName);
        image.setAttribute('src', lowResImage);
    });
}


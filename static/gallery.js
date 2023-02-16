function openLightboxModal() {
    document.getElementById("lightboxModal").style.display = "block";
}

function closeLightboxModal() {
    document.getElementById("lightboxModal").style.display = "none";
}

function incrementLightboxImage(n) {
    let currentImageID = document.getElementById("lightboxImage").getAttribute("imageid");
    newImageID = String(parseInt(currentImageID) + n);
    showLightboxImage(newImageID);
}

function showLightboxImage(imageID) {
    // TODO: break into sub-functions
    // Sanitise image id based on number of available images
    let images = document.getElementsByClassName("img-gallery");
    let newImageID = parseInt(imageID);
    let imageCount = images.length;
    if (newImageID > imageCount) {
        newImageID = 1;
    }
    else if (newImageID < 1) {
        newImageID = imageCount;
    }
    newImageID = String(newImageID);

    // Select new image from available images based on ID
    var newImage = null;
    for (image of images) {
        if (image.getAttribute("imageid") == newImageID) {
            newImage = image;
            break;
        }
    }

    // Update displayed image based on new image
    let currentImage = document.getElementById("lightboxImage");
    currentImage.src = newImage.src;
    currentImage.setAttribute("imageid", newImageID);
    let currentImageNumber = document.getElementById("lightboxImageNumber");
    currentImageNumber.innerHTML = newImageID + " / " + imageCount;
    let captionText = document.getElementById("caption");
    captionText.innerHTML = newImage.alt;

    // Update thumbnails to show active image
    var thumbs = document.getElementsByClassName("img-lightbox-thumb");
    for (i = 0; i < thumbs.length; i++) {
        thumbs[i].className = thumbs[i].className.replace(" active", "");
    }
    let activeThumbIndex = parseInt(newImageID) - 1;
    let activeThumb = thumbs[activeThumbIndex];
    activeThumb.className += " active";
}

function addImgInfoForLightbox() {
    var galleryImages = document.getElementsByClassName("img-gallery");
    var lightboxThumbnails = document.getElementById("lightboxThumbnails");
    var i = 1;
    // For each image, set id/functions for main image and thumbnail
    for (var image of galleryImages) {
        // Set image id and onclick function
        image.setAttribute("imageid", i);
        image.addEventListener("click", openLightboxImage);
        // Set image thumbnail id, image, and onclick function
        let imageThumb = document.createElement("img");
        imageThumb.setAttribute("imageid", i);
        imageThumb.className = "img-lightbox-thumb";
        imageThumb.src = image.src;
        imageThumb.addEventListener("click", openLightboxImage);
        lightboxThumbnails.appendChild(imageThumb);
        i += 1;
    }
}

function openLightboxImage() {
    openLightboxModal();
    showLightboxImage(this.getAttribute("imageid"));
}

function selectLightboxImageFromThumbnail() {
    openLightboxModal();
    showLightboxImageByNum(this.getAttribute("imageid"));
}

window.onload=addImgInfoForLightbox;
import os

import sqlalchemy
from PIL import Image
from PIL.ExifTags import TAGS
from flask import Flask, flash, redirect, render_template, request, session

IMAGE_ALTS = {}

app = Flask(__name__)

@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

@app.route("/")
def index():
    """Show homepage"""
    return render_template("index.html")

@app.route("/brief")
def brief():
    """Show Project Brief page"""
    return render_template("brief.html")

@app.route("/gallery")
def gallery():
    """Show Images Gallery page"""
    return render_template("gallery.html")

@app.route("/parts")
def parts():
    """Show Parts List page"""
    return render_template("parts.html")

@app.route("/timeline")
def timeline():
    """Show Timeline page"""
    return render_template("timeline.html")

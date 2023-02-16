import os

# from PIL import Image
# from PIL.ExifTags import TAGS
from flask import Flask, flash, redirect, render_template, request
from werkzeug.middleware.proxy_fix import ProxyFix

app = Flask(__name__)

app.wsgi_app = ProxyFix(
    app.wsgi_app, x_for=1, x_proto=1, x_host=1, x_prefix=1
)

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

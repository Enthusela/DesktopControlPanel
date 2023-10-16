from ctypes import cast, POINTER
from comtypes import CLSCTX_ALL

import pycaw.pycaw as pycaw
import os
import re
import socket
import traceback
import time

# TODO: remember why this is its own function
# TODO: try a non-blocking delay on this function
# while True:
#     try:
#         devices = pycaw.AudioUtilities.GetSpeakers()
#         interface = devices.Activate(pycaw.IAudioEndpointVolume._iid_, CLSCTX_ALL, None)
#         volume = cast(interface, POINTER(pycaw.IAudioEndpointVolume))
#         # volume.SetMasterVolumeLevelScalar(level, None)
#         print(devices, interface, volume)
#     except Exception as e:
#         print("Main volume error: " + str(e))

def main():
    while True:
        set_audio_volume_app({"VM": 1.0, "V1": 0.25, "V2": 0.75, "V3": 0.5})

def set_audio_volume_app(levels):
    audio_apps = [
        {"name": "Discord",         "id": "Discord.exe",    "group": "V1"},
        {"name": "Chrome",          "id": "chrome.exe",     "group": "V2"},
        {"name": "Edge",            "id": "msedge.exe",     "group": "V2"},
        {"name": "Firefox",         "id": "firefox.exe",    "group": "V2"},
        {"name": "Spotify",         "id": "Spotify.exe",    "group": "V2"},
        {"name": "Call Of Duty",    "id": "cod.exe",        "group": "V3"},
        {"name": "Insurgency",      "id": "InsurgencyClient-Win64-Shipping.exe", "group": "V3"},
        {"name": "Overwatch",       "id": "Overwatch.exe",  "group": "V3"}
    ]
    # Get list of valid audio sessions currently open
    try:
        audio_sessions = pycaw.AudioUtilities.GetAllSessions()
    except Exception as e:
        print("Failed to get audio interfaces: "+ str(e))
        return
    
    # Set levels for individual audio sessions
    app_ids = [app["id"] for app in audio_apps]
    for session in audio_sessions:
        session_id = get_session_id(session)
    
        if session_id in app_ids:
            audio_group = [app["group"] for app in audio_apps if app["id"] == session_id][0]
        else:
            audio_group = "VM"
    
        if audio_group == "VM":
            level = levels["VM"]
        else:
            level = levels["VM"] * levels[audio_group]
        
        try:
            session.SimpleAudioVolume.SetMasterVolume(level, None)
        except Exception as e:
            print("App volume error: " + str(e))


def get_session_id(session):
    return re.split("\\\\|%",session.Identifier)[-2]

main()
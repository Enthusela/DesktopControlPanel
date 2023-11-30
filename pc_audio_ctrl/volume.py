from ctypes import cast, POINTER
# from comtypes import CLSCTX_ALL

import pycaw.pycaw as pycaw
import os
import re
import selectors
import socket
import traceback
import time

pc_cmds = {"A": "0"}
volume_cmds = {"VM": 1.0, "V1": 0.5, "V2": 0.5, "V3": 0.5}
sel = selectors.DefaultSelector()


def static_vars(**kwargs):
    def decorate(func):
        for k in kwargs:
            setattr(func, k, kwargs[k])
        return func
    return decorate


def main():
    """Main function for the volume control script."""
    # deregister sel from the main thread
    connect_server()
    while True:
        # Process server data
        get_events()
        if pc_cmds:
            try:
                set_audio_device(pc_cmds["A"])
            except Exception as e:
                print("Error: failed to set audio device (" + str(e) + ").")
        if volume_cmds:
            try:
                set_audio_levels(volume_cmds)
            except Exception as e:
                print("Error: failed to set audio levels (" + str(e) + ").")


def get_events():
    """Gets events from the selector and calls the appropriate callback function."""
    events = None
    
    try:
        events = sel.select(0)
    except Exception as e:
        print("get_events(): Failed to get events (" + str(e) + ").")
    
    if events is None:
        connect_server()
    
    else:
        for key, mask in events:
            callback = key.data
            callback(key.fileobj, mask)


def connect_server():
    """Connects to the server and registers the socket."""
    address = "192.168.50.202"
    port = 5000
    print("Connecting to server at " + address + ":" + str(port) + "...")
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((address, port))
        sock.setblocking(False)
        sel.register(sock, selectors.EVENT_READ, read_cmds)
        print("Server socket registered.")
        print("Connected to " + str(sock.getpeername()))
    except Exception as e:
        print("connect_server(): Failed to connect (" + str(e) + ").")


def disconnect_server(sock, mask):
    """Disconnects from the server and unregisters the socket."""
    if sock.getpeername():
        print("Disconnecting from " + sock.getpeername()[0] + "...")
    else:
        print("Disconnecting from [unknown host]...")
    
    try:
        sock.send("PC disconnecting.".encode())
    except Exception as e:
        print("Warning: Failed to send disconnect message (" + str(e) + ")")
    
    if sock:
        try:
            sock.close()
            print("Server socket closed.")
            sel.unregister(sock)
            print("Server socket unregistered.")
        except Exception as e:
            print("Warning: Failed to disconnect (" + str(e) + ").")
            

def read_cmds(sock, mask):
    """Receives commands from the server and parses them."""
    try:
        sock.send("PC listening.".encode())
        server_cmds = sock.recv(1024).decode()
        parse_server_cmds(server_cmds)
        return
    except ConnectionResetError as e:
        print("Error: Server connection reset by peer (" + str(e) + ").")
    except Exception as e:
        print("Error: Failed to read commands from server (" + str(e) + ").")
        return

    disconnect_server(sock, mask)
    return


def parse_server_cmds(cmds):
    """Parses the commands received from the server and updates the appropriate variables"""
    # TODO: Update server to send commands as JSON
    if cmds:
        cmds = re.split(",", cmds)[:-1]
    
    for cmd in cmds:
        cmd = re.split(" ", cmd)
        if len(cmd) != 2:
            continue
        else:
            cmd, value = cmd
            if cmd in pc_cmds.keys():
                pc_cmds[cmd] = value
            elif cmd in volume_cmds.keys():
                volume_cmds[cmd] = prct_from_10bit(value)


@static_vars(audio_device_prev="0")
def set_audio_device(audio_device):
    """Sets the audio output device to the specified device if it is different from the previous device"""
    if audio_device != set_audio_device.audio_device_prev:
        nircmd_path = "C:\\Users\\natha\\OneDrive\\Documents\\Software\\Utilities\\nircmd.exe"
        nircmd_cmd = "cmd /c " + nircmd_path + " " + "setdefaultsounddevice "
        devices = {"0": "\"Soundcard\""
                 , "1": "\"Gaming Headphones\""}
        
        if audio_device in devices.keys():
            nircmd_cmd += devices[audio_device]
        
        try:
            os.system(nircmd_cmd)
            print("Audio device set to " + devices[audio_device])
        except Exception as e:
            print("Error: nircmd failed (" + str(e) + ")")

        set_audio_device.audio_device_prev = audio_device


def set_audio_levels(levels):
    audio_apps = [
        {"id": "Discord.exe",     "group": "V1"},
        {"id": "chrome.exe",      "group": "V2"},
        {"id": "msedge.exe",      "group": "V2"},
        {"id": "firefox.exe",     "group": "V2"},
        {"id": "Spotify.exe",     "group": "V2"},
        {"id": "cod.exe",         "group": "V3"},
        {"id": "csgo.exe",        "group": "V3"},
        {"id": "Overwatch.exe",   "group": "V3"},
        {"id": "HomeworldRM.exe", "group": "V3"},
        {"id": "r5apex.exe",      "group": "V3"},
        {"id": "InsurgencyClient-Win64-Shipping.exe", "group": "V3"}
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
            # level = levels["VM"]
            level = 1.0

        else:
            level = levels[audio_group] * 1.0 #levels["VM"]
        
        try:
            session.SimpleAudioVolume.SetMasterVolume(level, None)
        except Exception as e:
            print("App volume error: " + str(e))


def get_session_id(session):
    return re.split("\\\\|%",session.Identifier)[-2]


def prct_from_10bit(value):
    return round(float(value) / 1023.0 * 1.0, 2)


main()
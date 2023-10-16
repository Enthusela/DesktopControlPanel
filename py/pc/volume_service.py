from ctypes import cast, POINTER
from comtypes import CLSCTX_ALL
from pycaw.pycaw import AudioUtilities, IAudioEndpointVolume, ISimpleAudioVolume
import serial
import time

import win32service
import win32event
import servicemanager
import socket

def main():
    arduino = serial.Serial('COM5', 9600, timeout=.1)
    time.sleep(1)
    devices = AudioUtilities.GetSpeakers()
    interface = devices.Activate(IAudioEndpointVolume._iid_, CLSCTX_ALL, None)
    volume = cast(interface, POINTER(IAudioEndpointVolume))
    new_volume = 0
    old_volume = 0

    while True:
        data = arduino.readline()
        if data:
            new_volume = float(data)/100.0
            if new_volume != old_volume:
                volume.SetMasterVolumeLevelScalar(new_volume, None)
                old_volume = new_volume
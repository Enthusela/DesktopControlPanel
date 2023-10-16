REM stream_my_audio version 1.0 by Charlie Laub. Released 3 SEPT 2019.
ECHO OFF
CLS

REM This Windows batch script streams audio from the default Windows audio device
REM   to another computer at the IP address specified  

REM * * * * * * * * USER: set these parameters * * * * * * * *
REM blank spaces must not be present after the following values
REM   and spaces should not be used on either side of the equals sign

REM the bit depth of the audio stream (value must be 16 or 24):
   SET bit_depth=24
REM the IP address to which the audio data will be streamed:
   SET destination=192.168.2.2
REM set to NUL if no output file is desired
   SET send_gstreamer_output_to=NUL
REM duration of output buffer in milliseconds
   SET output_buffering=60


REM --------------------------- do not edit below this line -------------------------
REM BEGIN MAIN PROGRAM

SET message_flag=-1
SET gstreamer_pipeline=wasapisrc buffer-time=%output_buffering%000 ! queue ! audioconvert ! rtpL%bit_depth%pay ! udpsink host=%destination% port=1234 &

ECHO Program started at %DATE% %TIME%
ECHO -------------------------------------------------------------
ECHO(

:loop_begin
  REM check status of gstreamer and USB ether gadget
  tasklist | findstr -i gst-launch-1.0.exe >NUL
  if %ERRORLEVEL% == 0 GOTO :Process_Found
  SET gstreamer_is_running=false
  GOTO :check2
  :Process_Found
  SET gstreamer_is_running=true
  :check2
  SET connection_status=active
  ping -n 3 -w 1000 %destination% >NUL
  IF %ERRORLEVEL% == 0 GOTO :check_END
  SET connection_status=not_active
  :check_END

  IF %connection_status% == active (
    IF %gstreamer_is_running% == true GOTO :sleep_then_loop_again
  )
  IF %connection_status% == active (
    IF %gstreamer_is_running% == false GOTO :launch_then_loop_again
  ) 
  IF NOT %connection_status% == active (
    IF %gstreamer_is_running% == true GOTO :kill_then_loop_again
  )
  IF NOT %message_flag% == 0 (  
    SET message_flag=0
    ECHO the connection to %destination% is down. 
    ECHO Re-trying until the destination can be reached...
    ECHO(
  )
  CALL :SLEEP_FOR 2
  GOTO :loop_begin
REM loop end


:launch_then_loop_again
  REM LAUNCH GSTREAMER TO STREAM AUDIO DATA TO THE DESINATION IP ADDRESS 
  ECHO destination IP %destination% can be reached. Launching gstreamer at %DATE% %TIME%
  ECHO(
  START /B c:\gstreamer\1.0\X86_64\bin\gst-launch-1.0.exe -vvv %gstreamer_pipeline% > %send_gstreamer_output_to% 2>&1
  SET message_flag=2
  CALL :SLEEP_FOR 2
  GOTO :loop_begin

:kill_then_loop_again
  ECHO connection to %destination% lost at %DATE% %TIME%. Killing gstreamer process
  taskkill /IM "gst-launch-1.0.exe" /F 
  ECHO(
  SET message_flag=3
  CALL :SLEEP_FOR 1  
  GOTO :loop_begin

:sleep_then_loop_again
  IF NOT %message_flag% == 1 (  
    SET message_flag=1
    ECHO the connection is active and gstreamer is running
    ECHO(
  )
  CALL :SLEEP_FOR 10
  GOTO :loop_begin

:SLEEP_FOR
REM pauses execution for the number of seconds supplied as an argument
TIMEOUT /T %~1 /NOBREAK > NUL
EXIT /B 0

  
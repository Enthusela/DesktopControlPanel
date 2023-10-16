#!/bin/bash
# receive_my_audio version 1.0 by Charlie Laub. Released 3 SEPT 2019.

# This script should be run from a Raspberry Pi 4
# An audio stream is received and send to a local DAC
# Gstreamer is used to receive and process the audio

#program parameters. user may edit these.
IP_address=192.168.2.2
bit_depth=24
sample_rate=96000
output_audio_format=S24LE
ALSA_output_device=hw:1,0
input_buffer_size=60 #milliseonds. Set to 20-200msec
output_buffer_size=60 #milliseonds. Set to 20-200msec
inactivity_timeout=90 #seconds, if no data RX during this timespan, kill gstreamer
RX_threshold=100 #this avoids false positives. Set to 100-1000


# ------ users should not edit the script below this line ------

#code to handle kill signals, ctrl-C, etc.
clean_up() {
	# Perform program exit housekeeping
	killall gst-launch-1.0
	exit
}
trap clean_up SIGHUP SIGINT SIGTERM


function issue_message {
  #print status message, if any
  if [[ $message != "" ]]; then
    echo -e "\n$message";
  fi
  #reset message string
  message=""
}


function do_status_checks {
  #clear status messages
  message=""

  #check staus of network connection
  ping -w 1 $IP_address > /dev/null
  result=$?
  if [ $result -eq 0 ]; then
    message+="IP: up, "
    ((status_flag += 1))
  else
    message="$IP_address is unreachable, "
  fi

  #check if audio data is being received:
  #   compute how long it has been since packet RX on usb0
  #   when this rises above a threshold value, RX is active
  current_packet_count=$(cat /sys/class/net/usb0/statistics/rx_packets)
  if [ $((current_packet_count - rx_packets)) -gt $RX_threshold ]; then
    time_of_last_rx=$SECONDS
    time_since_last_rx=0
    ((status_flag += 2))
    message+="receiving audio data, "
  else
    time_since_last_rx=$((SECONDS - time_of_last_rx))
    message+="no data flow, "
  fi
  rx_packets=$current_packet_count

  #check if an instance of gstreamer is running
  ps -C gst-launch-1.0 > /dev/null
  result=$?
  if [ $result -eq 0 ]; then
    message+="gstreamer pipeline is running"
    ((status_flag += 4))
  else
    message+="gstreamer not running"
  fi
}


# begin main program:
message="Program execution began at: $(date +"%D %T" )"
clear
issue_message

#intialize status flag
status_flag=-1
#  IP activity = 0x1
#  audio data = 0x10
#  gstreamer status = 0x100
#  status=0: waiting for IP to become active
#  status=1: IP up, waiting for audio data, gstreamer not running
#  status=3: IP up, receiving audio data, gstreamer not running
#  status=4: IP down, not receiving audio data, gstreamer running
#  status=5: IP up, not receiving audio data, gstreamer running
#  status=7: IP up, receiving audio data, gstreamer running


#intial check if audio data is being received
rx_packets=$(cat /sys/class/net/usb0/statistics/rx_packets)
sleep 0.1
current_packet_count=$(cat /sys/class/net/usb0/statistics/rx_packets)
if [ $((current_packet_count - rx_packets)) -gt $RX_threshold ]; then
  time_of_last_rx=$SECONDS
  time_since_last_rx=0
else
  time_of_last_rx=0
  time_since_last_rx=$((RXthreshold + 1))
fi
rx_packets=$current_packet_count
sleep 0.1 #must wait here briefly...

#set up infinite loop to continuously check status and take action(s)
while :
do
  #save status then reset status flag
  last_status=$status_flag
  status_flag=0

  do_status_checks

  if [ $((status_flag % 2)) -eq 0 ]; then
    #the IP address cannot be contacted. It may be re-initializing. Keep checking for 30 seconds
    num_checks=0
    while :
    do
      sleep 1
      do_status_checks
      ((num_checks+=1))
      if [ $num_checks -eq 30 ] || [ ! $((status_flag % 2)) -eq 0 ]; then break; fi
    done
  fi

  #generate status message if there has been a change in status
  if [ $status_flag -ne $last_status ]; then
    message="$(date +"%D %T" )  new status:\n$message"
    issue_message
  fi

  #choose action based on status condition
  if [ $status_flag -eq 7 ] && [ $time_since_last_rx -eq 0 ]; then
    sleep 10
    continue
  fi

  if [ $status_flag -eq 5 ] && [ $time_since_last_rx -gt $inactivity_timeout ]; then
    killall gst-launch-1.0 > /dev/null
    message="action: gstreamer pipeline killed due to inactivity at $(date +"%D %T" )"
    issue_message
#    status_flag=-1
    continue
  fi

  if [ $status_flag -eq 3 ]; then
    #connection is ready, so launch gstreamer as background process
    gst-launch-1.0 -vvv udpsrc port=1234 caps='application/x-rtp,media=audio,clock-rate='"$sample_rate"',channels=2,payload=96' ! \
      rtpjitterbuffer latency=$input_buffer_size ! rtpL"$bit_depth"depay !audioconvert ! audio/x-raw,format=F32LE ! deinterleave name=input  \
      input.src_0 ! tee name=input_ch0   \
      input.src_1 ! tee name=input_ch1   \
      audiointerleave name=output0 latency=100000000 ! queue ! alsasink device="$ALSA_output_device" buffer-time="$output_buffer_size"000 provide-clock=false  \
      input_ch0. ! queue ! audioconvert ! 'audio/x-raw,format='"$output_audio_format"',channel-mask=(bitmask)0x1' ! output0.sink_0     \
      input_ch1. ! queue ! audioconvert ! 'audio/x-raw,format='"$output_audio_format"',channel-mask=(bitmask)0x2' ! output0.sink_1 \
      > gstreamer_output.txt 2>&1 &
    message="action: gstreamer pipeline launched at $(date +"%D %T" )"
    issue_message
#    status_flag=-1
    sleep 2
    continue
  fi
  if [ $((status_flag % 2)) -eq 0 ] && [ $status_flag -gt 0 ]; then
    #the IP address cannot be contacted
    message="Connection to $IP_address lost."
    if [ $status_flag -ge 4 ]; then
      #since IP is down, kill gstreamer
      message+="\naction: gstreamer pipeline terminated"
      killall gst-launch-1.0 > /dev/null
    fi
    message="$(date +"%D %T" )  new status:\n$message"
    issue_message
    sleep 2
#    status_flag=-1
  fi
done #end loop


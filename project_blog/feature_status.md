In general, the "Hardware/Software Pipeline" tasks refer to building the basic tech stack to get info from Point A to Point C; the "Feature Development" tasks refer to developing the specific functions to be built on the stack.

"Mega", "Uno", and "Raspi" refer to the Arduino Mega, Arduino Uno, and Raspberry Pi boards used, respectively.

| Feature/Pipeline                      | Order In Pipeline | Sub-function                        | Status |
|:--------------------------------------|:---:|:----------------------------------------------------|:-----|
| Cabinet                               | N/A | Faceplate VU Meter Mounts                           | Done |
| Cabinet                               | N/A | Faceplate Nixie Tube Mounts                         | Done |
| Cabinet                               | N/A | Faceplate Soundcard Mount                           | Done |
| Cabinet                               | N/A | Faceplate e-Ink Display Mount                       | Done |
| Cabinet                               | N/A | Faceplate Input Knobs                               | Done |
| Cabinet                               | N/A | Faceplate Input Knob Motors/Gears                   | Done |
| Cabinet                               | N/A | Faceplate Input Rocker Switches                     | Done |
| Cabinet                               | N/A | Faceplate Input Toggle Switches                     | Done |
| Cabinet                               | N/A | Faceplate Input Push-button Switches                | Done |
| Cabinet                               | N/A | Internal Mounting Plate                             | Done |
| Cabinet                               | N/A | Rear Plate Plate                                    | Done |
| Cabinet                               | N/A | Cabinet Top Plate                                   | Done |
| Cabinet                               | N/A | Cabinet Side Plates                                 | Done |
| Cabinet                               | N/A | Cabinet Bottom Plate                                | Mods required |
| Faceplate -> Mega Hardware Pipeline   | 1   | Faceplate Inputs                                    | Done |
| Faceplate -> Mega Hardware Pipeline   | 2   | Faceplate Input Breakout Board                      | Testing |
| Faceplate -> Mega Hardware Pipeline   | 3   | Mega Input Shield                                   | Testing |
| Mega -> Raspi Hardware Pipeline       | 1   | USB Connection                                      | Done |
| Raspi -> Uno Hardware Pipeline        | 1   | USB Connection                                      | Done |
| Raspi -> PC Hardware Pipeline         | 1   | Local Wifi Connection                               | Done |
| Input Knob Motorisation               | 1   | Mega Programming - Input Knob Control Modes         | Done |
| Input Knob Motorisation               | 2   | Motor Driver Shield                                 | Done |
| Input Knob Motorisation               | 3   | Shield -> Motor Connection                          | Done | 
| PC ARGB Control                       | 1   | Mega Programming - Input signal processing          | Done |
| PC ARGB Control                       | 2a  | Raspi Programming - Input passthrough to Uno        | Done |
| PC ARGB Control                       | 2b  | Raspi Programming - Home Assistant/Web App          | Not Started |
| PC ARGB Control                       | 3a  | Uno Programming - Manual HSV-based Color Control    | Done |
| PC ARGB Control                       | 3b  | Uno Programming - Manual Temp-based Color Control   | Done |
| PC ARGB Control                       | 3c  | Uno Programming - Auto Temp-based Color Control     | Not Started |
| PC ARGB Control                       | 4   | Uno ARGB Breakout Shield                            | To Be Ordered |
| PC ARGB Control                       | 5   | PC ARGB Devices                                     | Done |
| PC Volume Control                     | 1   | Raspi Programming - Input passthrough to PC         | Done |
| PC Volume Control                     | 3a  | PC Programming - Main Volume Control                | Done |
| PC Volume Control                     | 3b  | PC Programming - Three-channel App Volume Control   | Done |
| PC Volume Control                     | 3c  | PC Programming - Audio Output Device Selection      | Done |
| Mains Power Control                   | 1   | Mega Programming - Input Signal Processing          | In progress |
| Mains Power Control                   | 2   | Mega -> RF PCB Breakout Board                       | In progress |
| Mains Power Control                   | 3   | RF PCB -> Remote Mains Outlet Connection            | In progress |
| Mains Power Control                   | 4   | Remote Mains Outlets - Test Working                 | In progress |
| PC Audio Level Meters                 | 1   | Soundcard TRS Output Levels                         | Done |
| PC Audio Level Meters                 | 2a  | Soundcard -> Driver Board Connection                | Done |
| PC Audio Level Meters                 | 2b  | Power Supply -> Driver Board Connection             | Developing |
| PC Audio Level Meters                 | 3   | VU Meter Driver Board                               | Done |
| PC Audio Level Meters                 | 4a  | VU Meter Level Outputs                              | Done |
| PC Audio Level Meters                 | 4b  | VU Meter LED Outputs                                | Done |
| PC Audio Level Meters                 | 5a  | VU Meters                                           | Done |
| PC Audio Level Meters                 | 5b  | VU Meter LEDs                                       | Prototyped |
| PC Audio Spectrum Analyser            | 1   | Soundcard Optical Output Levels                     | Done |
| PC Audio Spectrum Analyser            | 2   | Soundcard -> Digital I/O Board Connection           | Done |
| PC Audio Spectrum Analyser            | 3   | Raspberry Pi Digital Audio I/O Board                | Testing |
| PC Audio Spectrum Analyser            | 4   | Raspberry Pi Digital Audio I/O Processing           | Developing |
| PC Audio Spectrum Analyser            | 5   | Custom Nixie Tube Driver HAT                        | Developing |
| PC Audio Spectrum Analyser            | 6   | Driver HAT -> Nixie Tube Connection                 | Done |
| PC Audio Spectrum Analyser            | 6   | Power Supply to Driver Board                        | Developing |
| e-Ink Display                         | 1   | Raspberry Pi Programming - Image Output             | Developing |
| e-Ink Display                         | 2   | Raspberry Pi HAT                                    | Done |
| e-Ink Display                         | 3   | e-Ink Display                                       | Done |
| Projec Website                        | 1   | Proxy Server (nginx)                                | Done |
| Projec Website                        | 2   | Domain Hosting                                      | Done |
| Projec Website                        | 3   | Server (node.js)                                    | Done |
| Projec Website                        | 4a  | Homepage                                            | In progress |
| Projec Website                        | 4b  | Brief                                               | In progress |
| Projec Website                        | 4c  | Timeline                                            | In progress |
| Projec Website                        | 4d  | Parts List                                          | In progress |
| Projec Website                        | 4e  | Gallery                                             | In progress |
| Projec Website                        | 4f  | Interactive Panel Map                               | Not started |
| Room ARGB Control                     | 1a  | Raspi Programming - Home Assistant Server           | Not started |
| Room ARGB Control                     | 1b  | Raspi Programming - Home Assistant Phone App        | Not started |
| Room ARGB Control                     | 2   | ESP32 Programming - Home Assistant Client           | Not started |
| Room ARGB Control                     | 3   | ESP32 ARGB Breakout Shield                          | Not started |
| Room ARGB Control                     | 4   | Room ARGB Devices - Confirm Working                 | Not started |
| e-Ink Touch Overlay                   | 1   | Touch Overlay                                       | Not started |
| e-Ink Touch Overlay                   | 2   | Touch Overlay -> Raspi Connection                   | Not started |
# The Desktop Control Panel Project

![20230719_201732_moneyShot](https://github.com/Enthusela/DesktopControlPanel/assets/112834651/f6647b0b-a4e7-4e16-8fc7-69cfead512c9)

An open-source, hand-built, tactile, home-brew smart-home hub with analog PC audio monitoring and control.

## Features

* ARGB LED device control with HSV and color-temperature control modes
* App-based volume mixing in Windows, including OS/Game/Discord channels
* Motorised knobs enable multiple control modes on a single set of inputs
* Mains power control: Toggle switches control mains power to the PC, monitors, and speakers via rental-safe RF-based remote-control mains outlets.
* Analog audio monitoring: PC soundcard connects to VU meters and IN-9 Nixie Tubes for level and 3-band spectrum monitoring, respectively
* Moog-inspired build quality: wood cabinet, black brushed-aluminium faceplate, big, satisfying switchgear, and actual Moog knobs/rockers because why not.
* "Seamless" E-ink display: White-on-black E-ink panel creates a non-light-emitting display integrated into the non-light-emitting faceplate.
* Integrated website: the website for this Control Panel project is hosted on the Raspberry Pi running inside the Control Panel
* Integration with Home Assistant for ARGB room-lighting via ESP Wifi boards
* Automatic Temperature-based color control: automatically transition room lighting from cool to warm as the sun sets here in Perth, Western Australia.
* Homebrew phone-control app: Home Assistant has this, of course, but this sounded like a good excuse to learn React Native.

<details>
<summary>Feature Development Status Table</summary>

| Feature/Pipeline                      | Order In Pipeline | Sub-function                        | Status |
|:--------------------------------------|:---:|:----------------------------------------------------|:-----|
| Faceplate -> Mega Hardware Pipeline   | 1   | Faceplate Input Hardware                            | Done |
| Faceplate -> Mega Hardware Pipeline   | 2   | Faceplate Input Breakout Board                      | Testing |
| Faceplate -> Mega Hardware Pipeline   | 3   | Mega* Input Shield                                  | Testing |
| Mega -> Raspi Hardware Pipeline       | 1   | USB Connection                                      | Done |
| Raspi -> Uno Hardware Pipeline        | 1   | USB Connection                                      | Done |
| Raspi -> PC Hardware Pipeline         | 1   | Local Wifi Connection                               | Done |
| Input Knob Motorisation               | 1   | Mega Programming - Input Knob Control Modes         | Done |
| Input Knob Motorisation               | 2   | Motor Driver Shield                                 | Done |
| Input Knob Motorisation               | 3   | Shield -> Motor Connection                          | Done | 
| PC ARGB Control                       | 1   | Mega Programming - Input signal processing          | Done |
| PC ARGB Control                       | 2a  | Raspi* Programming - Input passthrough to Uno       | Done |
| PC ARGB Control                       | 2b  | Raspi Programming - Home Assistant/Web App          | Not Started |
| PC ARGB Control                       | 3a  | Uno* Programming - Manual HSV-based Color Control   | Done |
| PC ARGB Control                       | 3b  | Uno Programming - Manual Temp-based Color Control   | Done |
| PC ARGB Control                       | 3c  | Uno Programming - Auto Temp-based Color Control     | Not Started |
| PC ARGB Control                       | 4   | Uno ARGB Breakout Shield                            | To Be Ordered |
| PC ARGB Control                       | 5   | PC ARGB Devices                                     | Done |
| PC Volume Control                     | 1   | Raspi Programming - Input passthrough to PC         | Done |
| PC Volume Control                     | 2a  | PC Programming - Main Volume Control                | Done |
| PC Volume Control                     | 2b  | PC Programming - Three-channel App Volume Control   | Done |
| PC Volume Control                     | 2c  | PC Programming - Audio Output Device Selection      | Done |
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
| PC Audio Spectrum Analyser            | 7   | Power Supply to Driver Board                        | Developing |
| e-Ink Display                         | 1   | Raspberry Pi Programming - Image Output             | Developing |
| e-Ink Display                         | 2   | Raspberry Pi HAT                                    | Done |
| e-Ink Display                         | 3   | e-Ink Display                                       | Done |
| Project Website                       | 1   | Proxy Server (nginx)                                | Done |
| Project Website                       | 2   | Domain Hosting                                      | Done |
| Project Website                       | 3   | Server (node.js)                                    | Done |
| Project Website                       | 4a  | Homepage                                            | In progress |
| Project Website                       | 4b  | Brief                                               | In progress |
| Project Website                       | 4c  | Timeline                                            | In progress |
| Project Website                       | 4d  | Parts List                                          | In progress |
| Project Website                       | 4e  | Gallery                                             | In progress |
| Project Website                       | 4f  | Interactive Panel Map                               | Not started |
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
| Cabinet                               | N/A | Cabinet Bottom Plate                                | In progress |
| Room ARGB Control                     | 1a  | Raspi Programming - Home Assistant Server           | Not started |
| Room ARGB Control                     | 1b  | Raspi Programming - Home Assistant Phone App        | Not started |
| Room ARGB Control                     | 2   | ESP32 Programming - Home Assistant Client           | Not started |
| Room ARGB Control                     | 3   | ESP32 ARGB Breakout Shield                          | Prototyped  |
| Room ARGB Control                     | 4   | Room ARGB Devices - Confirm Working                 | Not started |
| e-Ink Touch Overlay                   | 1   | Touch Overlay                                       | Not started |
| e-Ink Touch Overlay                   | 2   | Touch Overlay -> Raspi Connection                   | Not started |

* "Mega", "Raspi", and "Uno" refer to the Arduino Mega, Raspberry Pi, and Arduino Uno boards used, respectively.

</details>

## Background

This project grew out of a number of frustrations I was having building a killer PC setup that was both functional and aesthetically-pleasing: both important in a 1-by-1 apartment where said PC setup takes up a good third of the available living-room space.

<details>
<summary>ARGB controller software is not pleasant to use</summary>
<br>
The main idea for this project came out of my own frustration with existing hardware/software for controlling ARGB LEDs built into PC components. Most ARGB devices, like fans, must be plugged into a controller that interfaces with software running on the PC. This software is manufacturer-specific, closed-source, and typically quite limited in what it can do: you get some pre-loaded color patterns, but if you want it to do anything else, you're out of luck. To make matters worse, the software is often classic "bloatware": it insists on starting on login, then runs slow, chews up computer resources on functions you didn't ask for, and crashes often. Factor in multiple apps running for different manufacturers, and the user experience gets frustrating fast. While there is some scope to synchronise lighting across different manufacturer's products, these aren't ideal either: I was able to sync my CoolerMaster fans and my Corsair mouse, for instance, but only by using ARGB software tied to my motherboard, which was more limited than the software that came with either product.
</details> 


<details>
<summary>ARGB controller hardware is not special</summary>
<br>
Despite these software problems, I noticed that the physical connections across all my devices was using the same three-pin standard. A quick internet search taught me that the standard was, as expected, a simple Vcc-Data-Gnd pin layout.
</details>

<details>
<summary>Humans did not evolve to relax under rainbow lighting</summary>
<br>
While thinking about making my own ARGB controller, one feature that I particularly wanted was temperature-based color control. I liked the idea of room lighting that could change from cool white during the day to very warm, basically orange, at night. Ideally I'd have my whole apartment lit up like this to make it a well-lit space during the day that gradually becomes softer, warmer, and hopefully more sleep-conducive at night. This is pretty much the antithesis of most pre-built ARGB options, whether that's PC components or ARGB strip-lighting with controllers, which default to "rainbow vomit cyclone". Of course, if I had my own controller then I could make my color-temperature dreams come true.
</details>

<details>
<summary>I like my hardware ~ t e x t u r a l ~</summary>
<br>
I don't know about you, but I really love old-school electronics hardware, particularly audio hardware like synthesisers, amps, mixing tables, etc. The use of materials like wood and metal and glass makes them look and feel so much nicer, so much more hand-crafted, than anything you can get out of molded plastic. All that physical switchgear gives it a sense of weight and purpose; you just know there's a heap of clever analog wizardry going on in there as you move physical components about. It's kind of the opposite of the experience I was getting with the ARGB controllers, which is basically "plug in this small plastic box and hope the software will work". If I was ever going to build my own electronics hardware, something really cool, it would take a lot of design queues from that kind of old-school equipment.
</details>

<details>
<summary>I wanted a new job</summary>
<br>
This project became part of my Master Plan to pivot from Mechatronics Engineer to Software Engineer. If everything so far sounds like something I could have just bought one or more smart-home products to do, then yes, you're right, but as the feature set grew I realised varied tech stack required to build it would make it an ideal showcase project.
</details>

<details>
<summary>The Idea</summary>
<br>
So I have lighting software that doesn't work, connected to hardware that isn't complicated, pushing out colors I don't like, in a form factor that's the opposite of what I like in technology. What if I made the opposite of that? An open-source controller, accepting whatever standard plug-ins my devices used, with all of the color-control options I could think up, built into a sleek bit of hardware that makes you go "Nice." Maybe it'd look a little like this...
<br>
![20220818_153445_ControlPanelDiagram](https://github.com/Enthusela/DesktopControlPanel/assets/112834651/cc425910-ebe4-4202-9927-b99c3e888dff)

This repo contains (almost) everything I've built so far to make bring that digital finger-painting to life.
</details>

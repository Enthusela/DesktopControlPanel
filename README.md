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

## Background

This project grew out of a number of frustrations I was having building a killer PC setup that was both functional and aesthetically-pleasing: both important in a 1-by-1 apartment where said PC setup takes up a good third of the available living-room space.

### ARGB controller software is not pleasant to use

The main idea for this project came out of my own frustration with existing hardware/software for controlling ARGB LEDs built into PC components. Most ARGB devices, like fans, must be plugged into a controller that interfaces with software running on the PC. This software is manufacturer-specific, closed-source, and typically quite limited in what it can do: you get some pre-loaded color patterns, but if you want it to do anything else, you're out of luck. To make matters worse, the software is often classic "bloatware": it insists on starting on login, then runs slow, chews up computer resources on functions you didn't ask for, and crashes often. Factor in multiple apps running for different manufacturers, and the user experience gets frustrating fast. While there is some scope to synchronise lighting across different manufacturer's products, these aren't ideal either: I was able to sync my CoolerMaster fans and my Corsair mouse, for instance, but only by using ARGB software tied to my motherboard, which was more limited than the software that came with either product.

### ARGB controller hardware is not special

Despite these software problems, I noticed that the physical connections across all my devices was using the same three-pin standard. A quick internet search taught me that the standard was, as expected, a simple Vcc-Data-Gnd pin layout.

### Humans did not evolve to relax under rainbow lighting

While thinking about making my own ARGB controller, one feature that I particularly wanted was temperature-based color control. I liked the idea of room lighting that could change from cool white during the day to very warm, basically orange, at night. Ideally I'd have my whole apartment lit up like this to make it a well-lit space during the day that gradually becomes softer, warmer, and hopefully more sleep-conducive at night. This is pretty much the antithesis of most pre-built ARGB options, whether that's PC components or ARGB strip-lighting with controllers, which default to "rainbow vomit cyclone". Of course, if I had my own controller then I could make my color-temperature dreams come true.

### I like my hardware ~ t e x t u r a l ~

I don't know about you, but I really love old-school electronics hardware, particularly audio hardware like synthesisers, amps, mixing tables, etc. The use of materials like wood and metal and glass makes them look and feel so much nicer, so much more hand-crafted, than anything you can get out of molded plastic. All that physical switchgear gives it a sense of weight and purpose; you just know there's a heap of clever analog wizardry going on in there as you move physical components about. It's kind of the opposite of the experience I was getting with the ARGB controllers, which is basically "plug in this small plastic box and hope the software will work". If I was ever going to build my own electronics hardware, something really cool, it would take a lot of design queues from that kind of old-school equipment.

### I wanted a new job

This project became part of my Master Plan to pivot from Mechatronics Engineer to Software Engineer. If everything so far sounds like something I could have just bought one or more smart-home products to do, then yes, you're right, but as the feature set grew I realised varied tech stack required to build it would make it an ideal showcase project.

### The Idea

So I have lighting software that doesn't work, connected to hardware that isn't complicated, pushing out colors I don't like, in a form factor that's the opposite of what I like in technology. What if I made the opposite of that? An open-source controller, accepting whatever standard plug-ins my devices used, with all of the color-control options I could think up, built into a sleek bit of hardware that makes you go "Nice." Maybe it'd look a little like this...

![20220818_153445_ControlPanelDiagram](https://github.com/Enthusela/DesktopControlPanel/assets/112834651/cc425910-ebe4-4202-9927-b99c3e888dff)

This repo contains (almost) everything I've built so far to make bring that digital finger-painting to life.

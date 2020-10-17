# MinePlusPlus
A Minecraft-inspired game designed to run on an Arduino system with with a 128×64 Dot Matrix GLCD. May be ported to other systems in the future.
## Abstract
Mine++ is a 2-dimensional, monochromatic game heavily influenced by Minecraft. It is designed to be as true to the original game as possible on the hardware it's designed for. 
## Hardware
Mine++ is currently designed for the Arduino Mega 2560, which has 32KiB of Flash Memory, 8KiB of SRAM, and 4KiB of EEPROM (non-volatile memory). It works with a monochrome 128×64 Dot Matrix Graphic LCD display using the KS0108 driver. 
Of course, support for more Arduinos, then more displays, and perhaps even more processor types will be added.
## How to Get Playing
To play, you'll need the necessary hardware, and you'll have to build the circuit on a breadboard or other prototyping board. Alternatively, you *could* get a custom PCB. Schematics are included to help you on your way.
You'll also need to get the Arduino IDE on your computer if you don't already have it.
Once it's all built, you'll need to get the code from the `main` branch. 
Once you upload the sketch, you can start playing.
## How to Develop
If you want to contribute to the code, tinker with it, or even just look through it to figure out how it works, you'll need the development version of the code.
You can download the files from the `development` branch.
The easiest thing to do is to build the circuit and upload the sketch. However, if you're working on porting to new hardware, you might have to get creative.
Once you have the code and it's running, you can look at the code to learn your way around, and then start tinkering. If you want to help the project, feel free to make some changes and submit a pull request.
## Plans
The development of Mine++ will follow the development of Minecraft as closely as possible. Since it is currently parallel to the first development version of Minecraft Java Edition, it may be a long time (perhaps an infinite amount of time) before the game catches up with Minecraft in features. I will keep working at it, though.

-------
*Version numbers will use Semantic Versioning*: https://semver.org/

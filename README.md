# Thunderstone Marx Interlock Firmware

src/main.cpp is the main program. For Arduino IDE this will get pasted in as your main .ino file.

## Requirements:
* Use Arduino IDE, Visual Studio Code + Platformio IDE, or Platformio CLI.
* I prefer PIO CLI. Commands are:
** ```pio device list``` to find available boards
** ```pio run -t upload -e uno -v``` inside firwmare directory to upload to b oard
** ```pio device monitor``` to open a serial port
* For Arduino or PlatformioIDE, just use the basic serial monitor. Enable charging by pressing r and discharge by pressing q.

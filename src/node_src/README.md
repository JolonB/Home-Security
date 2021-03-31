# Node Source Code Directory

This directory contains the code that will be uploaded to the node devices.

## Getting Started

Begin by installing ESP-IDF.
This can be done by following the instructions in the [Espressif docs](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html).

Enable the ESP-IDF tools with `get_idf`.  
You may need to run `idf.py set-target esp32s2` to configure the target board.
This only needs to be run once.  
The board can be configured with `idf.py menuconfig`.
The menuconfig themes aren't great, so it could be a good idea to set the style to monochrome with `idf.py menuconfig --style monochrome`.  
You can build your code with `idf.py build` when in a directory with a CMakelist file.
If you just run the `flash` command (see below), there is no need to also run `build`.  
You can flash your code with `idf.py -p (port) flash`. If you run into errors, try setting the baudrate with `idf.py -p (port) -b 115200 flash`.  
The serial port can be monitored with `idf.py -p (port) monitor`.
You can leave the monitor with `Ctrl+]`.  
If you want to automatically monitor after flashing, use `idf.py -p (port) flash monitor`.

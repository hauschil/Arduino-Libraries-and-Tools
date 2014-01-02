-------------------------------------------------
| Fio v3 Arduino Addon (updated August 9, 2012) |
-------------------------------------------------

The Fio v3 bootloader has been updated to use a bootloader similar to the officially released Arduino Leonardo.

This addon includes files for both the old (Diskloader) and new (Caterina) bootloader.


-------------------------------------------------
|         Installing the Arduino Addon          |
-------------------------------------------------

Installing this Arduino Addon is required for all platforms! This will add a Fio v3 option to your Boards menu within Arduino.

To install this addon, unzip the Fiov3 folder into a 'hardware' folder withing your Arduino sketchbook. If you don't know where your sketchbook is located, you can find it's location by going to File > Preferences within Arduino.

For Windows XP users, the sketchbook is usually located in your "My Documents/Arduino" directory. So once unzipped, this readme should reside in "My Documents/Arduino/hardware/ProMicro". Windows 7, by default, is something like: C:\Users\<user name>\Arduino\hardware\ProMicro.

Once unzipped, close any Arduino windows and reopen. The following board should be listed in the Tools > Board menu now:
Fio v3 (Leonardo)

Important: Though the new bootloader is Leonardo-compatible, DO NOT select "Arduino Leonardo" from the Tools > Board menu before uploading code to the Fio v3. Because the Fio v3 runs at 8MHz, the sketch uploaded to the board must be configured for that same speed.


-------------------------------------------------
|     Installing the Drivers (Windows only)     |
-------------------------------------------------
Windows users will need to install drivers the first time you connect the Fio v3. Usually a New Hardware Wizard will pop up, you'll need to direct it to the included "Drivers" folder. The driver is "SparkFun Fio v3.inf".

The included driver should work for both old (Diskloader - VID/PID = 0x1B4F/0x0004) and new (Leonardo - VID/PID = 0x1B4F/0xF100 (bootloader) and 0xF101 (sketch)) versions of the Fio v3.


-------------------------------------------------
|        Having Trouble uploading code?         |
-------------------------------------------------

If you've got "Fio v3 (Leonardo)" selected as your board, but you're having a problem uploading to the board. Maybe you're getting an error like:

"Connecting to programmer: .avrdude: Send: . [1b] 
avrdude: ser_send(): write error: sorry no info avail"

You may have an older version of the bootloader. No problem though, the old version still works great, you've just got to do a little editing of the included "boards.txt" file to enable the older version.

In boards.txt there are a series of lines that are commented out with #'s. Remove all of the #'s in front of the "fiov3L"'s. Then close and re-open Arduino. Select the "Fio v3 (Diskloader)" board, and try uploading your code again.

Still not working? We're happy to help. Please email our tech support team (techsupport@sparkfun.com).


-------------------------------------------------
|            What files are included?           |
-------------------------------------------------

This addon should work with no need to modify files. If you're curious what we've done to the default Arduino files though, read on...

* bootloaders/caterina: This is a slightly modified version of the Caterina bootloader, which is used in the Arduino Leonardo. Defines were modified so the VID and PID were set to SparkFun-specific values (VID: 0x1B4F, PIDs: 0xF100/F101 [get it?!]). To compile this, you'll need to download LUFA, and point the makefile to that directory.

To compile the bootloader for a Fio, you must first set F_CPU and PID near the top of the makefile. F_CPU should be 8000000.  PID should be F100.

This directory also includes Caterina-fiov3.hex which is the compiled images of the Fio v3's bootloader. It won't be deleted after running a 'make clean'.

* cores/arduino: This is a mostly unchanged listing of all the core files required by the Fio v3 to compile under Arduino. The files with changes are:
	USBCore.cpp: added conditional statement for seting PLLCSR.
	
* variants/fiov3: This is mostly identical to the Leonardo pins_arduino.h. There is a slight change to the RX and TX LED macros, because the LEDs are biased towards ground, instead of VCC, we had to flop those macros.

* boards.txt: This file is what Arduino looks at when populating its Tools > Board menu.
There are two board entires in this file:
Fio v3 (Leonardo) -and-
Fio v3 (Diskloader)

The Leonardo board should work for all Fio's purchased after August 8th, 2012. The diskloader board (not enabled by default, you'll have to remove the '#' comments), remains for legacy boards.

This also defines a few variables such as clock frequency, USB VID and PID, fuses, and bootloader location.

-------------------------------------------------
|         Questions, Comments, Concerns?        |
-------------------------------------------------

Please let us know if this gives you any problems. You can contact our tech support team (techsupport@sparkfun.com), or post a comment on the product page (or even contact me by email, I'd be happy to help) and we'll try to get back to you as quickly as possible.

Have fun!
-Jim (jim at sparkfun.com)
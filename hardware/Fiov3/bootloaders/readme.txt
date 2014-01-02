Fiov3/bootloaders
------------------------------------------------------------

This directory contains two versions of the Fio v3 Bootloader:

diskloader-Fio: 
----------------
This is the old version, it's left only for legacy boards. This version of the bootloader was included with Arduino 1.0. This bootloader uses the "arduino" avrdude upload protocol.

caterina:
-----------------
This is the most current version of the bootloader. The one I would recommend, and the version that is shipping on all current Fio boards. This bootloader uses the AVR109 avrdude sketch upload protocol.

This is a slight variation of what ships on Leonardo boards. The USB VID and PIDs are modified. And, in addition, lines 126 and 130 of Caterina.c were modified to speed up reset time. For more info on this change, check out this pull request on Arduino's github page: https://github.com/arduino/Arduino/pull/93.
	
-------------------------------------------------------------------------------------------------------------

Any trouble with any of this? Please don't hesitate to email our tech support team (techsupport@sparkfun.com).

-Jim Lindblom
August 9, 2012
**Hummingbird Duo Default Firmware**

This package contains the Hummingbird Duo default firmware, used to communicate over USB to the CREATE Lab Visual Programmer and the BirdBrain Robot Server. The Hummingbird default firmware makes use of the LUFA library, and so this package is essentially a branch of Dean Camera's excellent LUFA library for using USB on the AVR platform.

The Hummingbird addition is buried fairly deep in the library, specifically in Demos->Device->Classdriver->HummingbirdV2. You can edit the files in this folder, as well as make from this folder to compile the Hummingbird firmware.

This package also contains the full LUFA library (version 130901). [Learn more about LUFA] (http://www.fourwalledcubicle.com/LUFA.php)

All files are released under [LUFA's MIT license] (http://www.fourwalledcubicle.com/files/LUFA/Doc/130901/html/_page__license_info.html)
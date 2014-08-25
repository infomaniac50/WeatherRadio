### Old Notes ###
Some notes from previous versions. They are no longer relavent with the Streaming library. They are however still quite interesting.

#### Notes ####
  **libprintf_flt.a - using printf floating point support for Arduino**
  * http://www.avrfreaks.net/index.php?name=PNphpBB2&file=printview&t=56712&start=0
  * http://www.nongnu.org/avr-libc/user-manual/group__avr__stdio.html#gaa3b98c0d17b35642c0f3e4649092b9f1
  * http://forum.arduino.cc/index.php?topic=124809.5;wap2
  * http://forum.arduino.cc/index.php?topic=124809.0
  * http://forum.arduino.cc/index.php?topic=124809.msg938573#msg938573
  * http://forum.arduino.cc/index.php/topic,124809.msg940922.html#msg940922

**Script to patch the entire ide**
You can get the `fixfp` script from [here](http://forum.arduino.cc/index.php/topic,124809.msg940922.html#msg940922).
Replace ARDUINO_DIR with the actual directory of your ide.
```bash
cd ARDUINO_DIR
cd hardware/tools/avr/avr/
PATH=`pwd`:`pwd`/bin:$PATH
cd lib/
fixfp
for DIR in avr*/; do cd $DIR && fixfp && cd ..; done;
```


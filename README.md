# TelloTalent esp32 for arduino IDE
TelloTalent Robomaster TT hardware package for programming the esp32 included with it

## Notes

1. _All this files are extracted from the Mind+ program. Which you can download [here](http://mindplus.cc/en.html)_
2. _If you want to use the RMTT_libs library , I recommend downloading the one from the releases of this repo, as it is extracted from the Mind+ program and it works better than the one supplied by dji._
# Tutorial:
1. Download the lastest release tellotalent.zip archive
2. Extract tellotalent folder and place it inside the folder "hardware" in your arduino installation directory. 
Path  ``` {arduino.installation}/hardware ```
3. Open the arduino IDE and select the board just like you would do normally

# Issues 
 
If you use my own package.json adding this url https://raw.githubusercontent.com/MCTRACO/tellotalent_arduino_package/master/tellotalent_esp32_package.json to the additional boards url , when compiling it gives this error ```c:\users\big finger.desktop-m2tpuvp\appdata\local\arduino15\packages\tellotalent\hardware\tellotalentesp32\1.0.0\tools\mpython\xtensa-esp32-elf\xtensa-esp32-elf\include\c++\5.2.0\bits\stl_algobase.h:59:28: fatal error: bits/c++config.h: No such file or directory
compilation terminated. ```

If anyone knows how to fix this or has any idea on why this happens please let me know by opening an issue. Thanks in advance!

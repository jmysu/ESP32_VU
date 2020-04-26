# ESP32_VU [![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)<br>
A simple ESP32 T-Audio I2S VU w/ NeoPixels


<img src="picture/WM8978VU0422.png"/> &nbsp;&nbsp;&nbsp;<img src="picture/wm8978vu.gif" width=240/>&nbsp;&nbsp;&nbsp;<img src="picture/Neo0426.gif" width=240/>
<br><br>
## I2S Output
<img src="picture/400px-Waveforms.svg.png"/>
To test the I2S Output, rename either _mainDuDu.txt or _mainDoReMi.txt to cpp, build and flash.<br>
The _mainDoReMi implemented 4 kinds of waveform as above picture!
<br><br>

## Play MP3
<img src="picture/playMP3_0422.png"/>
To play MP3 audio, put the file into SD card root, build _mainMP3.cpp and flash!
<br>
<br>
<br>

## References
  - [TTGO T-Audio IDF components](https://github.com/LilyGO/TTGO-TAudio) TTGO IDF code at Github
  - [Arduino FFT](https://github.com/kosme/arduinoFFT) Arduino Fast Fourier Transform for Arduino 
  - [NeoPixel library](https://github.com/adafruit/Adafruit_NeoPixel) Arduino Adafruit NeoPixel Library.
  - [FastLED library](https://github.com/FastLED/FastLED) Another faster NeoPixel Library.
  - [Helix MP3 I2S Audio](https://github.com/schreibfaul1/ESP32-audioI2S) Original ESP32 MP3 I2S player

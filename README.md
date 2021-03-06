# USB_Display [![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)<br>
Some stuff for the USB Display

<br>
<br>


<br>
<br>

__Windows setup && Python test__ <br>
Testing Windows user mode SDK w/ Python3...<br>
<img src="pic/WioTerminaImgTest802D.jpg" width=600/> <img src="pic/WioTerminalImgTest.jpg" width=300/> <br>

__USB capture for the 3bytes rpusbdisp-FILL command__<br>
<img src="pic/WioFill_WireShark.jpg" width=1024/> <br>
<br><br>

__Test for GIF animation__ SAMD51@200MHz w/o DMA, 320x240/16bit-color@8fps...<br>
gif from GIPHY:https://giphy.com/explore/donald-trump <br>
<img src="pic/TrumpTennis320.gif" width=320/> <br>
https://github.com/jmysu/USB_Display/tree/main/PlatformIO.projects/wioTerminalGifAnimate<br>
<br><br>
__Have fun with SD card slide show (jpg,png,gif)__<br>
<img src="pic/WioTerminalSlideShow.gif" width=320/> <br>
https://github.com/jmysu/USB_Display/tree/main/PlatformIO.projects/wioTerminalSdSlideShow<br>
<br><br>

__JPEG Decode, SPI TFT bulk write(TFT_eSPI) vs. DMA(lovyanGFX)__ SAMD51@200MHz/SPI-write@60MHz...<br>
506ms:374ms@320x480, 321ms:175ms@320x240<br> 
<img src="pic/TFT_DMA_Performance.jpg" width=640> <img src="pic/SdJpegDecDMA1212.gif" width=320>
https://github.com/jmysu/USB_Display/tree/main/PlatformIO.projects/wioTerminal_JPEGDEC<br>      
<br><br>
<img src="pic/TFTspi_16BIT.jpg" width=480><br>
A typical 16bit TFT data write...<br>
<br><br>

## References
  - [RoboPeak Mini USB DisplayUSB Interface Protocol Specification](http://www.robopeak.com/data/doc/rpusbdisp/RPUD03-rpusbdisp_interface_protocol-enUS.1.0.pdf) Robopeak USB display protocol PDF.
  - [SeeedWioTerminal UserMode SDK](https://wiki.seeedstudio.com/Wio-Terminal-HMI-Usermode-SDK/) SeeedStudio Wio Terminal SDK.
  - [SeeedWioTerminal USB Display](https://github.com/Seeed-Studio/Seeed_Arduino_USBDISP) Github repository.
  - [Arduino GIF Animator](https://github.com/bitbank2/AnimatedGIF) A lightweight Arduino GIF decoder.
  - [LovYanGFX](https://github.com/lovyan03/LovyanGFX) Another nice library for Arduino TFT.
  - [AnimatedGif](https://github.com/bitbank2/AnimatedGIF) A lightweight Arduino GIF decoder for playing animated files.
  - [JPEGDEC](https://github.com/bitbank2/JPEGDEC) Another optimized JPEG Decoder for Arduino.

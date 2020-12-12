// A simple image slideshow which reads all the .JPG files from the root
// directory of a SD card and shows each for 1 second on an ILI9341 display.
#include <Arduino.h>
#include <SD/Seeed_SD.h>
//#include <TFT_eSPI.h>
//TFT_eSPI tft;
#include <LovyanGFX.hpp>
LGFX lgfxTFT;

#include <JPEGDEC.h>
JPEGDEC jpeg;

#include "FreeStack.h"

extern String sFiles[];
extern int iFilesCount;
extern void setupSD();
extern int sdSizeKB();
extern int sdFreeKB();

// Setup - initialize ILI9341 display, wait for serial monitor, open SD card

// Functions to access a file on the SD card
File myfile;

void *myOpen(const char *filename, int32_t *size) {
    myfile = SD.open(filename);
    *size = myfile.size();
    return &myfile;
}
void myClose(void *handle) {
    if (myfile) myfile.close();
}
int32_t myRead(JPEGFILE *handle, uint8_t *buffer, int32_t length) {
    if (!myfile) return 0;
    return myfile.read(buffer, length);
}
int32_t mySeek(JPEGFILE *handle, int32_t position) {
    if (!myfile) return 0;
    return myfile.seek(position);
}
/*
 * TFT_eSPI.c line:2783 CHANGES
 * 
    #if defined (__SAMD51__)  && defined(ILI9341_DRIVER)
        //_com.transfer((void*)data, NULL, len << 1, true);
        while (len--) {
            tft_Write_16(*data);
            data++;
            }
    #else
    if (swap) {
        _com.transfer((void*)data, len << 1);
    } else {
        _com.transfer((void*)data, len << 1);
    }
    #endif
*/

//
// swap rb bytes in buffer
//
//void ioconvert(uint16_t* in, uint16_t* out, int w, int h)
void swapBuffer(uint16_t *in, uint16_t *out, int len) {
    uint16_t c;
    while (len--) {
        c = *(in++);
        *(out++) = (c >> 8) | (c << 8);  //convert for 2 SPI write
    }
}
//
// Function to draw pixels to the display
//
void JPEGDraw(JPEGDRAW *pDraw) {
    //Serial.printf("jpeg draw: x,y=%d,%d, cx,cy = %d,%d\n", pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
    //tft1.writeRect(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight, pDraw->pPixels);  //GFX
    //tft.pushImage(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight, pDraw->pPixels);  //TFT_eSPI

    /*
    int16_t pixels = (pDraw->iWidth) * (pDraw->iHeight);
    uint16_t outBuf[pixels];
    swapBuffer(pDraw->pPixels, outBuf, pixels);
    lgfxTFT.pushImage(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight, outBuf);  //LOVYANGFX
    */
    //lgfxTFT.setSwapBytes(true); //Enable swapbytes for Little-Endien
    lgfxTFT.pushImage(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight, pDraw->pPixels);  //LOVYANGFX
                                                                                   //lgfxTFT.drawRect(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight, *pDraw->pPixels);
}
/*
void showJpegFile(const char *fn)
{
    File entry = SD.open(fn);
    if (!entry) {
        Serial.printf("\nOpen fail:%s", fn);
        return;
        }
    if (!entry.isDirectory()) {
      const char *name = entry.name();
      const int len = strlen(name);
      if (len > 3 && strcmp(name + len - 3, "jpg") == 0) {
        Serial.print("File: ");
        Serial.println(name);
        //tft.print("File: ");
        //tft.println(name);
        jpeg.open((const char *)name, myOpen, myClose, myRead, mySeek, JPEGDraw);
        jpeg.decode(0, 0, 0);
        jpeg.close();
        }
      }
    entry.close();
}*/
int iScreenWidth;
int iScreenHeight;
void setup() {
    Serial.begin(115200);
    // Wait for USB Serial
    delay(1000);  // NOTE: Some devices require a delay after Serial.begin() before serial port can be used

    uint16_t ui1234 = 0x1234;
    Serial.printf("\nByte Order 0x1234 = (MSB:LSB)0x%02X%02X\n", (ui1234 & 0xFF00) >> 8, ui1234 & 0x00FF);

    //-------------------------------------------------------------
    setupSD();
    /*
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW);
  tft.setTextSize(2);
  tft.println("Waiting for Arduino Serial Monitor...");

  while (!Serial && millis() < 3000); // wait up to 3 seconds for Arduino Serial Monitor
  Serial.println("JPEGDEC Slideshow");
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  iScreenWidth = tft.width();
  iScreenHeight= tft.height();
  */
    lgfxTFT.init();
    lgfxTFT.setColorDepth(16);
    //lgfxTFT.setSwapBytes(true); //!!!Important!!! Enable swapbytes for Little-Endien!
    lgfxTFT.setColor(TFT_WHITE);
    lgfxTFT.startWrite();
    lgfxTFT.setAddrWindow(0, 0, lgfxTFT.width(), lgfxTFT.height());
    for (int y = 0; y < lgfxTFT.height(); ++y) {
        for (int x = 0; x < lgfxTFT.width(); ++x) {
            lgfxTFT.writeColor(lgfxTFT.color888(x << 1, x + y, y << 1), 1);
        }
    }
    lgfxTFT.print("JPG show\n");
    lgfxTFT.endWrite();
    iScreenWidth = lgfxTFT.width();
    iScreenHeight = lgfxTFT.height();
}

extern bool GetImageSize(File f, int16_t *x, int16_t *y);
// Main loop, scan for all .JPG files on the card and display them
void loop() {
    int filecount = 0;

    File dir = SD.open("/TFT");
    while (true) {
        File entry = dir.openNextFile();
        if (!entry) {
            entry.close();
            break;
        };
        if (!entry.isDirectory()) {
            const char *name = entry.name();
            const int len = strlen(name);
            if (len > 3 && strcmp(name + len - 3, "jpg") == 0) {
                //Get size from binary
                int offsetX = 0;
                int offsetY = 0;
                int16_t w, h;
                if (GetImageSize(entry, &w, &h)) {
                    if (w < iScreenWidth) offsetX = (iScreenWidth - w) >> 1;
                    if (h < iScreenHeight) offsetY = (iScreenHeight - h) >> 1;
                    Serial.printf("(%4dx%4d) ", w, h);
                }
                //Check Low Memory
                //Serial.print(F("\t FreeStack: "));
                int iFree = FreeStack();
                //Serial.println(iFree);
                if (iFree < 10000) {
                    Serial.println("\nLow freestack reset starting...");
                    delay(100);
                    NVIC_SystemReset();
                }

                //tft.fillScreen(TFT_BLACK);
                //lgfxTFT.clear();
                int iTime = millis();
                jpeg.open((const char *)name, myOpen, myClose, myRead, mySeek, JPEGDraw);
                //JPEGDEC PixelType default to Little-Endian!
                //jpeg.setPixelType(BIG_ENDIAN_PIXELS);
                jpeg.setPixelType(1);
                jpeg.decode(offsetX, offsetY, 0);
                jpeg.close();
                

                Serial.print(millis() - iTime);
                Serial.print("\t");
                Serial.println(name);

                //tft.setCursor(0, tft.height()-16);
                //tft.println(name+4);
                lgfxTFT.setCursor(0, iScreenHeight - 16);
                lgfxTFT.print(name + 4);

                filecount = filecount + 1;
                delay(50);  //!!!give SPI/DMA some break
            }
        }
        entry.close();
    }
    dir.close();
}
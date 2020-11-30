/*
 * Example use of chdir(), ls(), mkdir(), and  rmdir().
 */
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"
// SD card chip select pin.
const uint8_t chipSelect = SDCARD_SS_PIN;
//------------------------------------------------------------------------------
// File system object.
SdFat sd;
// Directory file.
//SdFile root;
// Use for file creation in folders.
//SdFile file;
// Error messages stored in flash.
#define error(msg) sd.errorHalt(F(msg))


//------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);

  // Wait for USB Serial
  while (!Serial) {
    SysCall::yield();
    }
  delay(1000);

  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!sd.begin(chipSelect, SD_SCK_MHZ(50))) {
    sd.initErrorHalt();
    }
  

  sd.ls(LS_R);
}

void showFolder(const char *foldername) {
 SdFile dir;
 SdFile file;
 //const char foldername[] = "/TFT";

  if (!dir.open(foldername)){
    error("open folder failed");
    }
  else {
    Serial.print(foldername);
    Serial.println("------------");
    while (file.openNext(&dir, O_RDONLY)) {
        char fn[32];
        file.getName(fn, 32);
        Serial.print(fn);
        int iLen = strlen(fn);
        for (int i=0; i<(32-iLen); i++)
            Serial.print(" ");
        Serial.println(file.fileSize());
        file.close();
        }
    }
   dir.close();
}

//------------------------------------------------------------------------------
void loop() {

   showFolder("/TFT");

   delay(3000);
}
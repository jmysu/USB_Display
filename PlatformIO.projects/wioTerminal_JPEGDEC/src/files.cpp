#include <Arduino.h>

#include <SD/Seeed_SD.h>

String sFiles[99];
int iFilesCount;

int FilesCount(const char *dir)
{
  File entry;
  File root = SD.open(dir);
  int iCount;

    if (!root) return -1;
    iCount=0;
    while (true) {
        entry =  root.openNextFile();
        if (!entry) break;  
        //Serial.println(entry.name());
        if (iCount <100) 
            sFiles[iCount] = entry.name();
        iCount++;
        entry.close();
        }
    root.close();
    return iCount;    
}

int sdSizeKB()
{
    return SD.totalBytes()/1024;
}

int sdFreeKB()
{
    return (SD.totalBytes()-SD.usedBytes())/1024;
}

void setupSD()
{
    Serial.println("Opening SD files...");

    bool isSDOK = SD.begin(SDCARD_SS_PIN, SDCARD_SPI, 50000000);
    if (isSDOK) {
        Serial.println("SD Ok!");
        iFilesCount = FilesCount("/TFT");
        for (int i=0;i<iFilesCount;i++) {
            Serial.printf("\n%02d %s", i, sFiles[i].c_str());
            }
        // Display free space
        Serial.printf("\nTotal MB: %6d", sdSizeKB()/1024);
        Serial.printf("\nFree  MB: %6d", sdFreeKB()/1024);
        }   
    else 
        Serial.println("SD Open Fail!!!");    
}
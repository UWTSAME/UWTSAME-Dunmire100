#ifndef SDCARD_H
#define SDCARD_H

#include <Wire.h>
#include <SPI.h>
#include <SD.h>

class SdCard {
  private:
    int myFolderNum = 0;
    String myFolderPath = "";

  public:
    SdCard();

    bool begin();
    void folderMaker();
    bool write(String theMessage);
};

#endif
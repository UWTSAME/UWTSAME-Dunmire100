#include "SdCard.h"

SdCard::SdCard(){
}

bool SdCard::begin(){
  bool setupSuccessful = false;
  if(!SD.begin()) {
    return false;
  }

  folderMaker();

  File testFile = SD.open(myFolderPath + "/Data.txt", FILE_WRITE);
  if(testFile){
    testFile.println("Beginning Data:");
    testFile.close();
    bool setupSuccessful = true;
  }
  return setupSuccessful;
}

void SdCard::folderMaker(){
  while (true) {
    folderPath = "/" + String(folderNum);

    if (!SD.exists(folderPath)) {
      SD.mkdir(folderPath);
      break;
    }
    folderNum++;
  }
}

bool SdCard::write(String theMessage){
  bool sentStatus = false;
  File dataText = SD.open(myFolderPath + "/Data.txt", FILE_WRITE);
  if(dataText) {
    dataText.println(theMessage);
    dataText.flush();
    dataText.close();
    sentStatus = true;
  }
  return sentStatus;
}
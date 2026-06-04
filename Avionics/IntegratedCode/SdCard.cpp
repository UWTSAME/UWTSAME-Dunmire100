#include "SdCard.h"

SdCard::SdCard(){
}

bool SdCard::begin(){
  bool setupSuccessful = true;
  if(!SD.begin()) {
    return false;
  }

  folderMaker();

  File testFile = SD.open(myFolderPath + "/Data.txt", FILE_WRITE);
  if(testFile){
    testFile.println("Beginning Data:");
    testFile.close();
    setupSuccessful = true;
  }
  return setupSuccessful;
}

void SdCard::folderMaker(){
  while (true) {
    myFolderPath = "/" + String(myFolderNum);

    if (!SD.exists(myFolderPath)) {
      SD.mkdir(myFolderPath);
      break;
    }
    myFolderNum++;
  }
}

bool SdCard::write(String theMessage){
  bool sentStatus = false;
  File dataText = SD.open(myFolderPath + "/Data.txt", FILE_APPEND);
  if(dataText) {
    dataText.println(theMessage);
    dataText.flush();
    dataText.close();
    sentStatus = true;
  }
  return sentStatus;
}
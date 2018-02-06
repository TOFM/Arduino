#include <SoftwareSerial.h>
SoftwareSerial RFID(8, 9); // RX and TX

int accPin = 13;

int tags[][14] = {
  {2,48,49,48,48,54,56,55,67,48,66,49,69,3},
  {2,34,23,53,34,54,24,23,57,34,21,43,12,3}
};
int emptyTag[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int numCols = sizeof(tags)/sizeof(tags[0]);

class TagFunctions {
  public:  
    bool validateTag(int inputTag[14]){
    int trueBits = 0;
    for(int i = 0; i < numCols; i++){
      for(int j = 0;j < 14;j++){
        if(inputTag[j] == tags[i][j]){
          trueBits++;
        } else {
          trueBits = 0;
          break;
        }
      }
      if (trueBits == 14){
        trueBits = 0;
        return true;
        break;
      } else {
        trueBits = 0;
        return false;
        }
      }
    }

    void eraseTag(int inputTag[14]){
      for(int i = 0;i < 14;i++){
        inputTag[i] = 0;
      }
    }
    
    void readTag() {
      int dataBit = 0;
      if(RFID.available() > 0) {
        for(int i = 0;i < 14;i++){
          dataBit = RFID.read();
          emptyTag[i] = dataBit;
        }
        RFID.flush();
      } else if(RFID.available() <= 0 || RFID.available() == 9 || RFID.available() == 4) {
        eraseTag(emptyTag);
        Serial.flush();
      }
    }
};

TagFunctions tool;

void setup() {
  RFID.begin(9600);
  Serial.begin(9600);
  pinMode(accPin,OUTPUT);
  String startupInfo = String("Total registered tags:" + numCols + '\n');
  Serial.print(startupInfo);
} 

void loop() {
  tool.readTag();
  if (tool.validateTag(emptyTag)){
    Serial.println("Accepted");
    digitalWrite(accPin, HIGH);
    delay(300);
    digitalWrite(accPin, LOW);
  } else {
    Serial.println("Rejected");
    delay(300);
  }
}


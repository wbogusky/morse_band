#include <SPI.h>
#include <RH_RF69.h>

RH_RF69 rf69(4, 3);

int touchPin = 7;
int vibePin = A0;
uint8_t before = LOW;
uint8_t now = LOW;

void setup() {  
  pinMode(touchPin, INPUT);
  pinMode(vibePin, OUTPUT);
  
  Serial.begin(115200);
  while (!Serial);

  if (!rf69.init())
    Serial.println("init failed");
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  // No encryption
  if (!rf69.setFrequency(900.0))
    Serial.println("setFrequency failed");

  rf69.setTxPower(14, true);
}

void loop() {
  now = digitalRead(touchPin);
  
  if (before < now){
    rf69.send("200", 5);
    rf69.waitPacketSent();
  } else if (before > now){
    rf69.send("0", 3);
    rf69.waitPacketSent();
  }

  before = now;
  
//  if (digitalRead(touchPin) == HIGH){
//    rf69.send("255", 5);
//  } else {
//    rf69.send("0", 3);
//  }
  

  uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  int incoming;

  rf69.waitAvailableTimeout(500);
  
  if (rf69.recv(buf, &len)){
    if (len == 5){
      incoming = 200;
      digitalWrite(vibePin, true);
    } else {
      incoming = 0;
      digitalWrite(vibePin, false);
    }
    Serial.println(incoming);
  }
//
  
//  delay(40);
//  analogWrite(vibePin, 0);
  
}

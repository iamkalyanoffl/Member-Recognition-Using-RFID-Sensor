#include <SPI.h>
#include <MFRC522.h>
#include <qrcode.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire , -1);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Approximate your card to the reader...");
  Serial.println(); 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C) ;
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5,20);
  display.println("WELCOME TO  BIONARY");
  display.display();
  display.dim(true);

}

void loop() {
 // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "9D BE FC DA") //change here the UID of the card/cards that you want to give access
  {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(9,20);
    display.println("ALREADY A MEMBER");
    display.display();
  }
  else
  {
    display.clearDisplay();
    displayQRCode("https://www.instagram.com/b.i.o.n.a.r.y/");
  }

}

void displayQRCode(const char* text){
  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  qrcode_initText(&qrcode, qrcodeData,3,0, text);

  int scale = min(OLED_WIDTH / qrcode.size , OLED_HEIGHT / qrcode.size);

  int shiftX = (OLED_WIDTH - qrcode.size*scale)/2;
  int shiftY = (OLED_HEIGHT - qrcode.size*scale)/2;
  display.fillRect(0,0,128,64,WHITE);
  for (uint8_t y=0; y< qrcode.size; y++){
    for (uint8_t x=0; x< qrcode.size; x++) {
      if (qrcode_getModule (&qrcode, x, y)) {
        display.fillRect(shiftX + x*scale, shiftY + y*scale , scale, scale, BLACK);
      }
    }
  }
  display.display();
}

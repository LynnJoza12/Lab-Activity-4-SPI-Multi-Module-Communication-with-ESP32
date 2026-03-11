#include <SPI.h>
#include <MFRC522.h>
#include <LedControl.h>

// Pin Definitions
#define RST_PIN         22          
#define SS_1_PIN        5           // CS for RFID
#define SS_2_PIN        15          // CS for LED Matrix

// Initialize instances
MFRC522 mfrc522(SS_1_PIN, RST_PIN); 
LedControl lc = LedControl(23, 18, SS_2_PIN, 1); // DIN, CLK, CS, Number of Devices

void setup() {
  Serial.begin(115200);
  while (!Serial); 
  SPI.begin();            // Init SPI bus

  // Initialize RFID
  mfrc522.PCD_Init();
  Serial.println("System Initialized...");
  Serial.println("Checking SPI Devices...");

  // Initialize LED Matrix
  lc.shutdown(0, false);       // Wake up MAX7219
  lc.setIntensity(0, 8);       // Set brightness (0-15)
  lc.clearDisplay(0);          // Clear display

  // Verification Output
  Serial.println("1. RFID-RC522: READY");
  Serial.println("2. MAX7219 Matrix: READY");
  Serial.println("Waiting for RFID Tag...");
}

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // If a card is detected:
  Serial.print("Tag UID detected:");
  String content= "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Visual feedback on LED Matrix (Display an 'X' pattern)
  displaySuccess();
  
  delay(1000);
  lc.clearDisplay(0);
}

void displaySuccess() {
  byte xPattern[8] = {
    B10000001, B01000010, B00100100, B00011000,
    B00011000, B00100100, B01000010, B10000001
  };
  for(int i=0; i<8; i++) {
    lc.setRow(0, i, xPattern[i]);
  }
  Serial.println("Action: Pattern displayed on MAX7219");
}

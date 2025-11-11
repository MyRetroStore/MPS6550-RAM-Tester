/*
  Arduino Mega 2560 HAT  - Commodore 6550 RAM

  MyRetroStore
  myretrostore.co.uk

  10/11/2025

  Compiled uaing Arduino IDE 1.8.19

  Dependency: DIO2

  Sketch to test 6550 RAM Module (1024 x 4 bit)

*/

#include <Arduino.h>
#include <DIO2.h>

#define WE_PIN 57
#define CS1 36
#define CS2 37
#define CS3 38
#define CS4 39
#define PHI 32

#define LED_Fail 54
#define LED_Pass 55
#define PWR_Detect 56

const unsigned int MEMORY_SIZE = 1024; // 1024 x 4
uint8_t DATA_PINS[] = {58, 59, 60, 61};
uint8_t ADDR_PINS[] = {24, 25, 22, 23, 26, 27, 28, 29, 30, 31};

// --------------------------------------------------------------------
// Set address
// --------------------------------------------------------------------
void setAddress(unsigned int address) {

  for (uint8_t i = 0; i < sizeof(ADDR_PINS); i++)
  {
    digitalWrite2(ADDR_PINS[i], ((address >> i) & 1));
  }
}

// --------------------------------------------------------------------
// Reset Control pins
// --------------------------------------------------------------------
void resetControlPins() {
  digitalWrite2(WE_PIN, HIGH);
  digitalWrite2(CS4, HIGH);
  digitalWrite2(PHI, LOW);
}

// --------------------------------------------------------------------
// Write byte to address (byte,address)
// --------------------------------------------------------------------
void writeByte(unsigned char byte, unsigned int address) {
  for (int n = 0; n < sizeof(DATA_PINS); n++) {
    pinMode2(DATA_PINS[n], OUTPUT);
  }

  setAddress(address);
  digitalWrite2(CS4, LOW);
  digitalWrite2(PHI, HIGH);

  for (uint8_t i = 0; i < sizeof(DATA_PINS); i++)
  {
    digitalWrite2(DATA_PINS[i], ((byte >> i) & 1));
  }

  digitalWrite2(WE_PIN, LOW);
  resetControlPins();
}

// --------------------------------------------------------------------
// Initialize
// --------------------------------------------------------------------
void Initialize()
{
  pinMode2(CS1, OUTPUT);
  digitalWrite2(CS1, HIGH);
  pinMode2(CS2, OUTPUT);
  digitalWrite2(CS2, HIGH);
  pinMode2(CS3, OUTPUT);
  digitalWrite2(CS3, LOW);
  pinMode2(CS4, OUTPUT);
  pinMode2(PHI, OUTPUT);
  digitalWrite2(PHI, HIGH);
  pinMode2(WE_PIN, OUTPUT);

  pinMode2(LED_Pass, OUTPUT);
  pinMode2(LED_Fail, OUTPUT);

  digitalWrite(LED_Pass, HIGH);
  digitalWrite(LED_Fail, LOW);

  pinMode2(PWR_Detect, INPUT);
}

// --------------------------------------------------------------------
// LED Error
// --------------------------------------------------------------------
void LED_Error() {
  digitalWrite2(LED_Pass, LOW);
  while (1) {
    digitalWrite2(LED_Fail, HIGH);
    delay(250);
    digitalWrite2(LED_Fail, LOW);
    delay(250);
  }

}

// --------------------------------------------------------------------
// Setup
// --------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);

  Serial.println("");
  Serial.println("==========================");
  Serial.println(F("    MPS6550 RAM Tester "));
  Serial.println("         Ver 1.0");
  Serial.println(F("    myretrostore.co.uk"));
  Serial.println("==========================");

  Initialize();

  // Check fuse
  boolean PWR = analogRead(PWR_Detect);
  if (PWR == false) {
    Serial.println("No Vcc power detected - check fuse");
    LED_Error();
  }

  for (int n = 0; n < sizeof(ADDR_PINS); n++) {
    pinMode2(ADDR_PINS[n], OUTPUT);
  }

  resetControlPins();
}

// --------------------------------------------------------------------
// Read byte from address
// --------------------------------------------------------------------
unsigned char readByte(unsigned int address) {
  for (int n = 0; n < sizeof(DATA_PINS); n += 1) {
    pinMode(DATA_PINS[n], INPUT_PULLUP);
  }

  setAddress(address);

  digitalWrite(CS4, LOW);
  digitalWrite2(PHI, HIGH);

  unsigned char byte = 0;

  for (int b = 0; b < sizeof(DATA_PINS); b++) {
    if (digitalRead(DATA_PINS[b])) {
      byte = byte | 1 << (b);
    }
  }

  resetControlPins();

  return byte;
}

// --------------------------------------------------------------------
// Print binary in 10 bit for address value
// --------------------------------------------------------------------
void printBin10(byte aByte) {
  for (int8_t aBit = 9; aBit >= 0; aBit--)
    Serial.write(bitRead(aByte, aBit) ? '1' : '0');
}

// --------------------------------------------------------------------
// Print binary in 4 bit for address value
// --------------------------------------------------------------------
void printBin4(byte aByte) {
  for (int8_t aBit = 3; aBit >= 0; aBit--)
    Serial.write(bitRead(aByte, aBit) ? '1' : '0');
}

// --------------------------------------------------------------------
// Pass 1 - writes 1111 to each address and reads data back
// --------------------------------------------------------------------
void pass1() {
  Serial.print("");
  Serial.print("Test 1");
  uint8_t value; //changed from int value;

  for (unsigned int i = 0; i < MEMORY_SIZE; i++)   // Loop through all addresses
  {
    writeByte(0xF, i);
    value = readByte(i);

    if (value != 0xF) {
      Serial.println(""); Serial.println(""); Serial.print("Error reading data B1111 at address B"); printBin10(i);
      Serial.print(" Read: "); printBin4(value);
      digitalWrite(LED_Pass, LOW);
      digitalWrite(LED_Fail, HIGH);

      while (1);
    }
  }
  Serial.println(" - OK");
}

// --------------------------------------------------------------------
// Pass 2 - writes 0000 to each address and reads data back
// --------------------------------------------------------------------
void pass2() {
  Serial.print("Test 2");
  uint8_t value; 

  for (unsigned int i = 0; i < MEMORY_SIZE; i++)   // Loop through all addresses
  {
    writeByte(0x0, i);
    value = readByte(i);

    if (value != 0x0) {
      Serial.println(""); Serial.println(""); Serial.print("Error reading data B0000 at address B"); printBin10(i);
      Serial.print(" Read: "); printBin4(value);
      digitalWrite(LED_Pass, LOW);
      digitalWrite(LED_Fail, HIGH);

      while (1);
    }
  }
  Serial.println(" - OK");
}


// --------------------------------------------------------------------
// Pass 3 - write 0101 and 1010 to addresses
// --------------------------------------------------------------------
void pass3() {

  int value;

  Serial.print("Test 3");

  for (unsigned int i = 0; i < MEMORY_SIZE; i++)   // Test B0101
  {
    writeByte(0B0101, i);
    value = readByte(i);

    if (value != 0B0101) {
      Serial.println(""); Serial.println(""); Serial.print("Error reading data B0101 at address B"); printBin10(i);
      Serial.print(" Read: "); printBin4(value);
      digitalWrite(LED_Pass, LOW);
      digitalWrite(LED_Fail, HIGH);

      while (1);
    }
  }

  for (unsigned int i = 0; i < MEMORY_SIZE; i++)   // Test B1010
  {
    writeByte(0B1010, i);
    value = readByte(i);

    if (value != 0B1010) {
      Serial.println(""); Serial.println(""); Serial.print("Error reading data B1010 at address B"); printBin10(i);
      Serial.print(" Read: "); printBin4(value);
      digitalWrite(LED_Pass, LOW);
      digitalWrite(LED_Fail, HIGH);

      while (1);
    }
  }

  Serial.println(" - OK");
}

// --------------------------------------------------------------------
// Pass 4 - writes 0 to 0, 1 to 1, 2 to 2  and then reads back and compares
// --------------------------------------------------------------------
void pass4() {
  Serial.print("Test 4");

  unsigned int write_value = 0;
  unsigned int read_value;

  for (unsigned int i = 0; i < MEMORY_SIZE; i++)   // Loop through all addresses
  {
    writeByte(write_value, i);
    write_value++;
    if (write_value == 0B1111) {
      write_value = 0;
    }
  }

  // put some delay here and then read data back
  delay(1000);
  digitalWrite(LED_Pass, LOW);
  digitalWrite(LED_Fail, HIGH);

  delay(1000);
  digitalWrite(LED_Pass, HIGH);
  digitalWrite(LED_Fail, LOW);

  delay(1000);
  digitalWrite(LED_Pass, LOW);
  digitalWrite(LED_Fail, HIGH);

  delay(1000);
  digitalWrite(LED_Pass, HIGH);
  digitalWrite(LED_Fail, LOW);


  write_value = 0;
  for (unsigned int i = 0; i < MEMORY_SIZE; i++)   // Loop through all addresses
  {
    read_value = readByte(i);

    if (read_value != write_value)
    {
      Serial.println(""); Serial.println(""); Serial.print("Error reading data B"); printBin4(write_value); Serial.print(" at address B"); printBin10(i);
      Serial.print(" Read: "); printBin4(read_value);
      digitalWrite(LED_Pass, LOW);
      digitalWrite(LED_Fail, HIGH);

      while (1);
    }

    write_value++;
    if (write_value == 0B1111) {
      write_value = 0;
    }
  }
  Serial.println(" - OK");
}

// --------------------------------------------------------------------
// Pass 4 - writes 0 to 0, 1 to 1, 2 to 2 etc and then reads back and compares
// --------------------------------------------------------------------

void loop()
{

  for (int pass = 0; pass < 4; pass++)
  {
    Serial.println("");
    Serial.print("Pass: ");
    Serial.println(pass + 1);
    Serial.println("");
    pass1();
    digitalWrite(LED_Pass, HIGH);
    digitalWrite(LED_Fail, LOW);

    pass2();

    digitalWrite(LED_Pass, LOW);
    digitalWrite(LED_Fail, HIGH);

    pass3();
    digitalWrite(LED_Pass, HIGH);
    digitalWrite(LED_Fail, LOW);

    pass4();
    digitalWrite(LED_Pass, LOW);
    digitalWrite(LED_Fail, HIGH);

  }

  Serial.println("");
  Serial.print(F("All Tests PASSED!"));

  digitalWrite(LED_Pass, HIGH);
  digitalWrite(LED_Fail, LOW);

  //The END
  while (1)
    ;
}

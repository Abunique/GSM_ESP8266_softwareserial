#include<Arduino.h>
#include <SoftwareSerial.h>

#define rxPin 13
#define txPin 15
#define Pk    12//powerkey

// Set up a new SoftwareSerial object.
SoftwareSerial Serial2(rxPin, txPin);

bool powerONcheck()
{
  bool responseflag = false;

  Serial2.print(F("AT\r\n"));/*(F();)It stores the string in program flash memory instead of SRAM. There is plenty of flash memory, but very little SRAM.*/

  /*available()
    Get the number of bytes (characters) available for reading from a software serial port.
    This is data that has already arrived and stored in the serial receive buffer.*/
  if (Serial2.available() > 0)
  {
    String response = Serial2.readString();
    if (response.indexOf("OK") >= 0) {
      responseflag = true;
    }
  }
  return responseflag;
}

//turning ON the GSM midule
void poweronoffGSM(char c)
{
  if (c == 'y') {
    Serial.println("Wait for the device to Turn ON");
    digitalWrite(Pk, HIGH);//Activate the powerKey to turn ON
    delay(300);
    digitalWrite(Pk, LOW);
    while (powerONcheck() == 0);
    Serial.println("Device has turned ON");
  }
  else if (c == 'n') {
    Serial.println("Wait for the device to Turn OFF");
    digitalWrite(Pk, HIGH); // Deactivate PowerKey to turn off GSM module
    delay(5000); // Wait for the module to power down (adjust as needed)
    digitalWrite(Pk, LOW);
    while (powerONcheck() == 1);
    Serial.println("Device is turned OFF");
  }
}

void send_sms ( String sms)
{
  const char phone[] = "+919710012997";
      Serial2.print("AT+CMGF=1\r");  //Set text mode
      delay(1000);
      Serial2.print("AT+CMGS=\""+String(phone)+"\"\r"); //Send message
      delay(1000);
      Serial2.print(sms);//Text message
      Serial2.println((char)0x1A); //Ctrl+Z
}

void serialfunc() {

  Serial.println("Enter the AT Command:");
  while (Serial.available() == 0); // reading input from serial monitor
  String command = Serial.readString();
  Serial2.print(command); // writing the user AT command in the softwareserial of GSM module

  while (Serial2.available() == 0);
  String response = Serial2.readString(); // reading the response of the softwareserial (gsm module)
  Serial.println("Response at the rx port");
  Serial.println(response);


}
void setup()  {

  // Define pin modes for TX and RX
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  // definig the powerkey output to turn ON GSM module.
  pinMode(Pk , OUTPUT);

  //set the baud rate for the serial monitor.
  Serial.begin(115200);

  // Set the baud rate for the SoftwareSerial object
  Serial2.begin(115200);

   poweronoffGSM('y');
}



void loop() {
  serialfunc();
 // send_sms("Test");
}
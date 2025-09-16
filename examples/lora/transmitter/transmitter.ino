/* TRANSMITTER */

#define TX 17
#define RX 16

String myMsg = "Hello World";
String msgLen = String(myMsg.length());

void sendMsg(String msg) {
  Serial2.println(msg);
  delay(500);
  // Si hay respuesta se imprime
  while(Serial2.available())
    Serial.print(char(Serial2.read()));
} 

void setup() {
  Serial.begin(115200);

  Serial2.begin(115200, SERIAL_8N1, RX, TX);

  delay(3000);
  Serial.println("Setting LORA antenna parameters");
  delay(1000);
  sendMsg("AT+ADDRESS=1");
  delay(1000);
  sendMsg("AT+NETWORKID=5");
  delay(1000);
  sendMsg("AT+BAND"); // Read the configured frequency
  delay(1000);
  sendMsg("AT+PARAMETER"); // Read the params configured
  delay(1000);
  sendMsg("AT+MODE"); // Read the mode configured
  delay(1000);
}

void loop() {
  Serial.println("Sending Message: " + myMsg);
  sendMsg("AT+SEND=2," + msgLen + "," + myMsg);
  delay(3000);
}
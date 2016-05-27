#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0x4D, 0xB5}; // MAC Address on bottom of device
byte ip[] = {172, 25, 252, 13};//Assign IP Address 69.91.147.114
EthernetServer server(80);
EthernetClient client = server.available();

// Define pins
int myLDRpins[] = {0, 1, 2, 3, 4, 5};
int pump = 6;
int laser = 7;

// Define time periods for each action (ms)
int rinse = 30000;
int measurementNumber = 20;
int shortSampleTime = 3000;
int longSampleTime = 60000;

void setup() {
  Serial.begin(9600);
  
  // Initialize ethernet system
  Ethernet.begin(mac, ip); //initialize ethernet
  server.begin(); //start to listen for clients
  
  // Initialize sensor system
  pinMode(laser, OUTPUT);
  pinMode(pump, OUTPUT);
  
  Serial.println("Initializing m-PARR...");
  Serial.println();
  
  Serial.print("Server is at ");
  Serial.println(Ethernet.localIP());
  
  Serial.println("Checking pump system. Running for 5 seconds.");
  digitalWrite(pump, HIGH);
  delay(5000);
  digitalWrite(pump, LOW);
  Serial.println();
  
  Serial.println("Checking laser and photoresistors.");
  Serial.println("initializing photoresistors...");
  photoresistors(3, 100);
  Serial.println("initializing laser...");
  digitalWrite(laser, HIGH);
  photoresistors(10, 100);
  digitalWrite(laser, LOW);
  Serial.println();
  
  Serial.println("Initialization complete...");
  Serial.println();
  Serial.println("Loading m-PARR System");
  Serial.println("System START");
  Serial.println();
}

void loop() { 
  Serial.println("m-PARR cycle beginning");
  
  client = server.available();
  
  digitalWrite(pump, HIGH);
  //delay(rinse);
  
  digitalWrite(laser, HIGH);
  if (client) {
       Serial.println("m-PARR connected to ethernet");
       boolean currentLineIsBlank=true;
       while (client.connected()) {
           if (client.available()) {
               Serial.println("client availiable");
               char c = client.read();
               if (c == '\n' && currentLineIsBlank) {
                 // Run short sample
                photoresistors(measurementNumber, shortSampleTime);
                digitalWrite(laser, LOW);
                delay(100);
                digitalWrite(pump, LOW);
                break;
               }
               if (c == '\n') {
                   currentLineIsBlank = true; 
               }
               else if (c != '\r') {
                   currentLineIsBlank = false;
               }
           }
       }
       delay(250);
       client.stop();
       delay(longSampleTime);     
   } else {
     Serial.println("Client unreachable.");
   }
}  

void photoresistors(int runNumber, int delayLength) {
  int i = 0;
  while (i < runNumber) {
    for(int x = 0; x < (sizeof(myLDRpins)/sizeof(int)); x++) {
      client.print(analogRead(myLDRpins[x]));
      client.print("   ");
    }
    client.println();
    i++;
    delay(delayLength);
  }
}


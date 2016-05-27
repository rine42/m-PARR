int myLDRpins[] = {0, 1, 2, 3, 4, 5};

int pump = 6;
int laser = 7;

int rinse = 30000;
int measurementNumber = 20;
int shortSampleTime = 3000;
int longSampleTime = 60000;

void setup() {
  Serial.begin(9600);
  pinMode(laser, OUTPUT);
  pinMode(pump, OUTPUT);
  
  Serial.println("Initializing m-PARR...");
  Serial.println();
  
  Serial.println("Checking pump system. Running for 5 seconds.");
  digitalWrite(pump, HIGH);
  delay(5000);
  digitalWrite(pump, LOW);
  Serial.println();
  
  Serial.println("Checking laser and photoresistors.");
  Serial.println("initializing photoresistors...");
  photoresistors(3, 100);
  Serial.println();
  Serial.println("initializing laser...");
  digitalWrite(laser, HIGH);
  photoresistors(10, 100);
  digitalWrite(laser, LOW);
  Serial.println();
  Serial.println();
  
  Serial.println("Initialization complete...");
  Serial.println();
  Serial.println("Loading m-PARR System");
  Serial.println("System START"); 
}

void loop() { 
  digitalWrite(pump, HIGH);
  delay(rinse);
  
  digitalWrite(laser, HIGH);
  if(laser, HIGH){
  photoresistors(measurementNumber, shortSampleTime);
  digitalWrite(laser, LOW);
  delay(100);
  digitalWrite(pump, LOW);

  delay(longSampleTime);
  }
}

void photoresistors(int runNumber, int delayLength) {
  int i = 0;
  while (i < runNumber) {
    for(int x = 0; x < (sizeof(myLDRpins)/sizeof(int)); x++) {
      Serial.print(analogRead(myLDRpins[x]));
      Serial.print("   ");
    }
    Serial.println();
    i++;
    delay(delayLength);
  }
}


#include <FlexCAN.h>

const int ledPin = 13;
const int user_indicator = 11;//LED light in the tactile button
const int butt = 9;//tactile button
const int contactor = 14;//signal to close the contactor
volatile int mode; // 1= standby, 2=charging, 3=fault.
static CAN_message_t msg; 

//Interrup Service Routine when button pressed, resets a fault or charging mode (2 or 3) back to standby (mode 2). Changes stanbdy mode(1) to charging(2)
void butt_ISR(){
  if (mode == 1){
    mode = 2;
  }else{
    mode = 1;
  }
  Serial.print("mode = ");
  Serial.println(mode);
}

//----------------/SETUP/---------------------------------------------------------------------
void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);//indicate activity at power on
  
  pinMode(6, OUTPUT);// Transceiver 3V3 pin
  pinMode(5, OUTPUT);// Transceiver GND pin
  digitalWrite(6, HIGH);// Transceiver 3V3 pin
  digitalWrite(5, LOW); // Transceiver GND pin
  
  pinMode(user_indicator, OUTPUT);
  pinMode(butt, INPUT_PULLUP);
  pinMode(contactor, OUTPUT);
  digitalWrite(contactor, LOW);//default the contactor signal to low at startup

  attachInterrupt(butt, butt_ISR , RISING);//When the tactile button is pushed, an interrupt will be triggered when the button is released. The button_pressed method is the ISR and changes the mode based on the current status.

  Can0.begin(500000);//Initiate the bus @ 500khz
  msg.len = 8;
  Serial.begin(9600);//initiate the serial, wait for it to be available before continuing. Timeout of 1 second if not available.
  while(!Serial && (millis() < 1000) ){
    delay(1);
  }
  Serial.println("--------------------Begin Receving CAN-Data from BMS--------------------");

  mode = 1;//into standby mode at startup
  
  //flash the LEDs 4 times at startup once setup is complete
  digitalWrite(ledPin, LOW);
  delay(50);
  digitalWrite(ledPin, HIGH);
  delay(50);
  digitalWrite(ledPin, LOW);
  delay(50);
  digitalWrite(ledPin, HIGH);
  delay(50);
  digitalWrite(ledPin, LOW);
  delay(50);
  digitalWrite(ledPin, HIGH);
  delay(50);
  digitalWrite(ledPin, LOW); 
}
//------------------END SETUP--------------------------------------------------------------------------------

//--Fault Handling, called when an error flag detected. Prints "FAULT" to the serial, flashes the onboard LED and changes the mode to 3(fault).
void handle_fault(){
  digitalWrite(contactor, LOW);//Signal to contactor to low to open contactor immideately
  Serial.println("FAULT");
  
  //flash the teensy LED 3 times
  digitalWrite(ledPin, HIGH);
  delay(50);
  digitalWrite(ledPin, LOW);
  delay(50);
  digitalWrite(ledPin, HIGH);
  delay(50);
  digitalWrite(ledPin, LOW);
  delay(50);
  digitalWrite(ledPin, HIGH);
  delay(50);
  digitalWrite(ledPin, LOW);
  mode = 3;
}

void no_heartbeat(){
  digitalWrite(contactor, LOW);//Signal to contactor to low to open contactor immideately
  Serial.println("NO HEARTBEAT DETECTED");
  
  //flash the teensy LED 3 times
  digitalWrite(ledPin, HIGH);
  delay(50);
  digitalWrite(ledPin, LOW);
  delay(50);
  digitalWrite(ledPin, HIGH);
  delay(50);
  digitalWrite(ledPin, LOW);
  delay(50);
  digitalWrite(ledPin, HIGH);
  delay(50);
  digitalWrite(ledPin, LOW);
  mode = 3;
}

//----main loop----------------------------------------------------------------------------------------------
void loop(){
  digitalWrite(contactor, LOW);//Contactor control signal to low when loop resets to ensure no inatvertant contactor closure
  
  while(mode == 1){//Standby Mode, light flashes at 1HZ
    digitalWrite(user_indicator, HIGH);
    delay(500);
    digitalWrite(user_indicator, LOW);
    delay(500);
  }
  while(mode == 3){//Fault Mode, light flashes at 10HZ
    digitalWrite(user_indicator, HIGH);
    delay(50);
    digitalWrite(user_indicator, LOW);
    delay(50);
  }
  int hb = millis();//time that last heartbeat was reieved
  while(mode == 2){//Charging normal mode
    
      digitalWrite(contactor, HIGH);
      digitalWrite(user_indicator, HIGH);
      
      if (Can0.read(msg)){
        if(msg.id == 0x600){
          Serial.println("Heartbeat");
          hb = millis();
        }
        if(msg.id == 0x6fd){
          for(int i = 0; i<8; i++){
            if(msg.buf[i] != 0){
              handle_fault();
            }
          }
        }
      }
      //Serial.println((millis() - hb));
      if((millis() - hb) > 2500){
        no_heartbeat();
    }
  }
}

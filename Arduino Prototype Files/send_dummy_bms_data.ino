//Russia declares war on ukraine as this is being coded. Code may be buggy as I am distracted by a reddit thread on the other screen.
#include <FlexCAN.h>

const int ledPin = 13;
int randnumber = random(10);
int counter = 1;
int fault_count = 1;

static CAN_message_t msg; 

void setup() {
  pinMode(6, OUTPUT);// Transceiver 3V3 pin
  pinMode(5, OUTPUT);// Transceiver GND pin
  digitalWrite(6, HIGH);// Transceiver 3V3 pin
  digitalWrite(5, LOW); // Transceiver GND pin
  pinMode(ledPin, OUTPUT);

  Can0.begin(500000);//Initiate the bus @ 500khz
  msg.len = 8;
  
  // The device wil flash the LED 3 times to indicate power on
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
}
//Data Packet Functions-------------------------------------------------------------------------------------------
//Data packet id and content pulled from prohelion data sheet
void heartbeat(){
  msg.id = 0x600;
  msg.buf[0] = 0;
  msg.buf[1] = 0;
  msg.buf[2] = 0;
  msg.buf[3] = 0;
  msg.buf[4] = 1;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.buf[7] = 0;
  Can0.write(msg);
}

void voltage_current(){
  msg.id = 0x6fa;
  msg.buf[0] = randnumber;
  msg.buf[1] = randnumber;
  msg.buf[2] = randnumber;
  msg.buf[3] = randnumber;
  msg.buf[4] = randnumber;
  msg.buf[5] = randnumber;
  msg.buf[6] = randnumber;
  msg.buf[7] = randnumber;
  Can0.write(msg);
}

void status_normal(){
  msg.id = 0x6fd;
  msg.buf[0] = 0;
  msg.buf[1] = 0;
  msg.buf[2] = 0;
  msg.buf[3] = 0;
  msg.buf[4] = 0;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.buf[7] = 0;
  Can0.write(msg);
}

void over_voltage(){
  msg.id = 0x6fd;
  msg.buf[0] = 0;
  msg.buf[1] = 0;
  msg.buf[2] = 0;
  msg.buf[3] = 0;
  msg.buf[4] = 0;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.buf[7] = 1;
  Can0.write(msg);
}

void under_voltage(){
  msg.id = 0x6fd;
  msg.buf[0] = 0;
  msg.buf[1] = 0;
  msg.buf[2] = 0;
  msg.buf[3] = 0;
  msg.buf[4] = 0;
  msg.buf[5] = 0;
  msg.buf[6] = 1;
  msg.buf[7] = 0;
  Can0.write(msg);
}

void over_temp(){
  msg.id = 0x6fd;
  msg.buf[0] = 0;
  msg.buf[1] = 0;
  msg.buf[2] = 0;
  msg.buf[3] = 0;
  msg.buf[4] = 0;
  msg.buf[5] = 1;
  msg.buf[6] = 0;
  msg.buf[7] = 0;
  Can0.write(msg);
}

void untrusted(){
  msg.id = 0x6fd;
  msg.buf[0] = 0;
  msg.buf[1] = 0;
  msg.buf[2] = 0;
  msg.buf[3] = 0;
  msg.buf[4] = 1;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.buf[7] = 0;
  Can0.write(msg);
}

void cmu_timeout(){
  msg.id = 0x6fd;
  msg.buf[0] = 0;
  msg.buf[1] = 0;
  msg.buf[2] = 0;
  msg.buf[3] = 0;
  msg.buf[4] = 1;
  msg.buf[5] = 0;
  msg.buf[6] = 1;
  msg.buf[7] = 0;
  Can0.write(msg);
}

void veh_timeout(){
  msg.id = 0x6fd;
  msg.buf[0] = 0;
  msg.buf[1] = 0;
  msg.buf[2] = 0;
  msg.buf[3] = 1;
  msg.buf[4] = 0;
  msg.buf[5] = 1;
  msg.buf[6] = 0;
  msg.buf[7] = 0;
  Can0.write(msg);
}

void setup_mode(){
  msg.id = 0x6fd;
  msg.buf[0] = 0;
  msg.buf[1] = 0;
  msg.buf[2] = 1;
  msg.buf[3] = 0;
  msg.buf[4] = 1;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.buf[7] = 0;
  Can0.write(msg);
}

void power_status(){
  msg.id = 0x6fd;
  msg.buf[0] = 0;
  msg.buf[1] = 1;
  msg.buf[2] = 0;
  msg.buf[3] = 1;
  msg.buf[4] = 0;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.buf[7] = 0;
  Can0.write(msg);
}

void iso_failure(){
  msg.id = 0x6fd;
  msg.buf[0] = 0;
  msg.buf[1] = 1;
  msg.buf[2] = 1;
  msg.buf[3] = 0;
  msg.buf[4] = 0;
  msg.buf[5] = 1;
  msg.buf[6] = 0;
  msg.buf[7] = 0;
  Can0.write(msg);
}

void soc_invalid(){
  msg.id = 0x6fd;
  msg.buf[0] = 1;
  msg.buf[1] = 1;
  msg.buf[2] = 0;
  msg.buf[3] = 0;
  msg.buf[4] = 1;
  msg.buf[5] = 0;
  msg.buf[6] = 0;
  msg.buf[7] = 0;
  Can0.write(msg);
}

void status_fault(int x){
  // Main loop will pass an int <=10 to cycle each fault in order
  if(x == 1){
    over_voltage();
  }
  if(x == 2){
    under_voltage();
  }
  if(x == 3){
    over_temp();
  }
  if(x == 4){
    untrusted();
  }
  if(x == 5){
    cmu_timeout();
  }
  if(x == 6){
    veh_timeout();
  }
  if(x == 7){
    setup_mode();
  }
  if(x == 8){
    power_status();
  }
  if(x == 9){
    iso_failure();
  }
  if(x == 10){
    soc_invalid();
  }
}
//--Run Loop----------------------------------------------------------------------------------------------------
void loop() {
  
  heartbeat();// heartbeat @ 1HZ
  delay(90);
  for (int i=0;i<5;i++){
    voltage_current();//voltage and current @ 10HZ
    delay(90);
  }
  if (counter % 15 == 0){
    status_fault(fault_count);// every 15 seconds a new fault is thrown
    digitalWrite(ledPin, HIGH);
    fault_count++;
    if(fault_count>10){
      fault_count = 1;
    }
  }else{
    digitalWrite(ledPin, LOW);
    status_normal();//normal status @ 1HZ
  }
  for (int i=0;i<5;i++){
    voltage_current();//voltage and current @ 10HZ
    delay(90);
  }
  counter ++;
}

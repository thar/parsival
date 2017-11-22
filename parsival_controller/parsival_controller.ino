/*
// # 
// # Editor     : Tong Hui from DFRobot, based on Lauren from DFRobot v1.0 code
// # Date       : 12.24.2012

// # Product name: Wireless Joystick v2.2 for Arduino
// # Product SKU : DFR0182
// # Code Version: 2.0 

// # Description:
// # The sketch for using the gamepad and print the button state and the analog values of the gamepad
// # to computer screen using serial monitor
 
*/

uint8_t buttonState[17];
int joystick[4];
uint8_t joystickDigital[4];
uint8_t txFrame[21];

int inputCommand = 0;

#define shackMotorPin 2

void setup()
{
  Serial.begin(115200);  //Init the Serial baudrate
  Serial1.begin(115200);  //Init the Serial baudrate
  InitIO();             // Initialize the inputs/outputs and the buffers
}

void InitIO(){ 
  for(int i = 0; i < 17; i++) 
  pinMode(i, INPUT); 
  pinMode(shackMotorPin,OUTPUT);
  digitalWrite(shackMotorPin,LOW);  // Stop shacking of the gamepad
}

unsigned long timer = 0;

void loop()
{
  if(millis() - timer > 500){  // manage the updating freq of all the controlling information
    DataUpdate();  //read the buttons and the joysticks data
    printData();   //print the datas and states
    timer = millis(); 
  }
  
  if(Serial.available()){
    char input = Serial.read();
    
    switch(input){
      case 's':
        Serial.println("Shack");
        inputCommand = input;
        digitalWrite(shackMotorPin,HIGH);
        break;
      
      case 'x':
        Serial.println("Stop");
        inputCommand = input;
        digitalWrite(shackMotorPin,LOW);
        break;
        
      default:
        break;
    }
  }
}

void DataUpdate(){
  memset(txFrame, 0, 21);
  buttonState[0] = digitalRead(A0);
  buttonState[1] = digitalRead(A1);
  txFrame[0] = !buttonState[0];
  txFrame[1] = !buttonState[1];
  for(int i = 3; i < 17; ++i) {
    buttonState[i] = digitalRead(i);
    txFrame[i] = !buttonState[i];
  }
  for(int i = 2; i < 6; ++i) {
    joystick[i - 2] = analogRead(i);
  }
  for(int i=0; i<4; ++i) {
    if (joystick[i] > 600) {
      joystickDigital[i] = 2;
    } else if (joystick[i] < 400) {
      joystickDigital[i] = 0;
    } else {
      joystickDigital[i] = 1;
    }
  }
  for(int i=0; i<4; ++i) {
    txFrame[i + 17] = joystickDigital[i];
  }
}

String Buttons[17] = {
  "J2","J1","","S2","S1","UP","LEFT","DOWN","RIGHT","1","4","2","3","RZ1","RZ2","LZ1","LZ2"};
  // Buttons Nmes

void printData(){
  Serial.print("Serial Frame:");
  for(int i = 0; i < 21; ++i) {
    Serial.print(txFrame[i]);
    Serial1.print(txFrame[i]);
  }
  Serial.println("");
  Serial.print("Button Pressed:");
  for(int i = 0; i < 17; i++) {
    if(buttonState[i] == 0) {
      Serial.print(Buttons[i]),Serial.print(",");
    }
  }
  Serial.println("");
  Serial.print("Analog Sticks:");
  //char analogValue[5];
  for(int i = 0; i < 4; i++) {
    Serial.print(joystickDigital[i]),Serial.print(",");
  }
  Serial.println("");
  //Serial.println(inputCommand);
}

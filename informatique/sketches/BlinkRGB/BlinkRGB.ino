/*
  Blink rgb with a state machine strategy
*/
#define LED_RED A1
#define LED_GREEN A2
#define LED_BLUE A3
#define PUSH_BUTTON 4
#define OFF 0
#define RED 1
#define GREEN 2
#define BLUE 3

boolean RGB_CHANGE = false;
int RGB_STATE = 0;
int last_btn = 0;
int btn = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  setup_serial();
  setup_btn();
  setup_rgb();
}

// the loop function runs over and over again forever
void loop() {
  // read sensors
  //read_btn();
  RGB_CHANGE = true;
  // apply state
  if(RGB_CHANGE){
    write_rgb();
  }
  // wait for a second
  delay(1000);
}

// the loop function runs over and over again forever
void read_btn() {
  btn = digitalRead(PUSH_BUTTON);
  Serial.print(last_btn);
  Serial.print("   ");
  Serial.println(btn);
  if(!last_btn && !btn){ // because mode pullup
    RGB_CHANGE = true;
    last_btn = 1;
  }
  else if(last_btn && btn){
    last_btn = 0;
  }
}

// the loop function runs over and over again forever
void write_rgb() {
  switch(RGB_STATE){
    case OFF :
      digitalWrite(LED_RED, HIGH);  // turn the LED on (HIGH is the voltage level)
      digitalWrite(LED_BLUE, LOW);  // turn the LED off by making the voltage LOW
      digitalWrite(LED_GREEN, LOW); // turn the LED off by making the voltage LOW
      Serial.println("RED");
      RGB_STATE = RED;
      break;

    case RED :
      digitalWrite(LED_RED, LOW);   // turn the LED off by making the voltage LOW
      digitalWrite(LED_BLUE, LOW);  // turn the LED off by making the voltage LOW
      digitalWrite(LED_GREEN, HIGH);// turn the LED on (HIGH is the voltage level)
      Serial.println("GREEN");
      RGB_STATE = GREEN;
      break;
      
    case GREEN :
      digitalWrite(LED_RED, LOW);   // turn the LED off by making the voltage LOW
      digitalWrite(LED_BLUE, HIGH); // turn the LED on (HIGH is the voltage level)
      digitalWrite(LED_GREEN, LOW); // turn the LED off by making the voltage LOW
      Serial.println("BLUE");
      RGB_STATE = BLUE;
      break;
      
    default :  
      digitalWrite(LED_RED, LOW);  // turn the LED off by making the voltage LOW
      digitalWrite(LED_BLUE, LOW); // turn the LED off by making the voltage LOW
      digitalWrite(LED_GREEN, LOW);// turn the LED on (HIGH is the voltage level)
      Serial.println("OFF");
      RGB_STATE = OFF;
      break;     
  }
  RGB_CHANGE = false;
}

void setup_serial() {
  // initialize serial
  Serial.begin(115200);
}

void setup_rgb() {
  // initialize pins as outputs
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
}

void setup_btn() {
  // initialize push btn
  pinMode(PUSH_BUTTON, INPUT_PULLUP);
}

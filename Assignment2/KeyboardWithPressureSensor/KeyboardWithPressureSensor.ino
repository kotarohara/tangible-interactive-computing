// We used code snippets from Arduino MouseKeyboard API page
// http://arduino.cc/en/Reference/MouseKeyboard

int pressurePin_FR = A0;
int pressurePin_BR = A1;
int pressurePin_BL = A2;
int pressurePin_FL = A3;

//int ledPin = 12;
int sensorValue_FR = 0;
int sensorValue_BR = 0;
int sensorValue_BL = 0;
int sensorValue_FL = 0;

int switch_pin = 2;
int val;

void setup()
{
  Serial.begin(9600);
  Keyboard.begin();
  pinMode(switch_pin, INPUT);  // set the switch pin (d2) as input
}

void loop()
{
  sensorValue_FR = analogRead(pressurePin_FR);
  sensorValue_BR = analogRead(pressurePin_BR);
  sensorValue_FL = analogRead(pressurePin_FL);
  sensorValue_BL = analogRead(pressurePin_BL);
  
  /*
  Serial.print("FR, BR, FL, BL : ");
  Serial.print(sensorValue_FR);
  Serial.print(", ");
  Serial.print(sensorValue_BR);
  Serial.print(", ");
  Serial.print(sensorValue_FL);
  Serial.print(", ");
  Serial.print(sensorValue_BL);
  Serial.println();
  */
  analog_signal_to_keyboard(sensorValue_FR, sensorValue_BR, sensorValue_FL, sensorValue_BL);
  delay(10);
}

void analog_signal_to_keyboard(int FR, int BR, int FL, int BL) {
  // Need a way to calibrate these things dynamically
  int FR_MAX = 1024;
  int BR_MAX = 1024;
  int FL_MAX = 1024;
  int BL_MAX = 1024;
  int FR_MIN = 500;
  int BR_MIN = 500;
  int FL_MIN = 500;
  int BL_MIN = 500;
  
  // Map sensor values to mapped values (0-127)
  int mapped_fr = map(FR, FR_MAX, FR_MIN, 0, 127);
  int mapped_br = map(BR, BR_MAX, BR_MIN, 0, 127);
  int mapped_fl = map(FL, FL_MAX, FL_MIN, 0, 127);
  int mapped_bl = map(BL, BL_MAX, BL_MIN, 0, 127);

  // Estimate up, down, right, and left mouse movement from mapped values.
  int up = (mapped_fr + mapped_fl) / 2;
  int down = (mapped_br + mapped_bl) / 2;
  int right = (mapped_fr + mapped_br) / 2;
  int left = (mapped_fl + mapped_bl) / 2;
  
  float keyboard_coefficient = 0.5;
  int keyboard_x = keyboard_coefficient * (right - left);
  int keyboard_y = keyboard_coefficient * (up - down);
  
  int keyboard_thresh = 70;
  // y-axis
  if (up > keyboard_thresh) {
    Keyboard.press(KEY_UP_ARROW);
  } else {
    Keyboard.release(KEY_UP_ARROW);
  }
  if (down > keyboard_thresh) {
     Keyboard.press(KEY_DOWN_ARROW);
  } else {
    Keyboard.release(KEY_DOWN_ARROW);
  }
  
  // x-axis
  if (right > keyboard_thresh) {
    Keyboard.press(KEY_RIGHT_ARROW);
  } else {
    Keyboard.release(KEY_RIGHT_ARROW);
  }
  
  if (left > keyboard_thresh) {
     Keyboard.press(KEY_LEFT_ARROW);
  } else {
    Keyboard.release(KEY_LEFT_ARROW);
  }
  
  // Switch on/off is mapped to press/release space key
  val = digitalRead(switch_pin);
  Serial.print("Swith: ");
  if (val == LOW) {
     Serial.print("Off, ");
     Keyboard.release(' ');                                                                                                                                                                                                                                                                                                                                                                                                                            
  } else { 
    Serial.print("On , ");
    Keyboard .press (' ');
  }
  
  Serial.print("(Up, Down, Right, Left) : (");
  Serial.print(up);
  Serial.print(", ");
  Serial.print(down);
  Serial.print(", ");
  Serial.print(right);
  Serial.print(", ");
  Serial.print(left);
  Serial.println(")");
  
}

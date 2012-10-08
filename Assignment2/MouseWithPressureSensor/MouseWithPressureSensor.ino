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

#define BUFFER_SIZE 8
#define DIFF_BUFFER_SIZE 64
int signal_buffer[BUFFER_SIZE];
int buffer_index = 0;
int diff_buffer_index = 0;
int signal_difference_buffer[DIFF_BUFFER_SIZE];

void setup()
{
  Serial.begin(9600);
  
  // Initialize
  for (int i = 0; i < BUFFER_SIZE; i++) {
    signal_buffer[i] = 0;
  }
  
  for (int i = 0; i < DIFF_BUFFER_SIZE; i++) {
    signal_difference_buffer[i] = 0;
  }
}

void loop()
{
  sensorValue_FR = analogRead(pressurePin_FR);
  sensorValue_BR = analogRead(pressurePin_BR);
  sensorValue_FL = analogRead(pressurePin_FL);
  sensorValue_BL = analogRead(pressurePin_BL);
  /*
  Serial.print(sensorValue_FR);
  Serial.print("\t");
  Serial.print(sensorValue_BR);
  Serial.print("\t");
  Serial.print(sensorValue_BL);
  Serial.print("\t");
  Serial.print(sensorValue_FL);
  
  Serial.println("");
  */
  analog_signal_to_mouse(sensorValue_FR, sensorValue_BR, sensorValue_FL, sensorValue_BL);
  delay(30); 
}

void analog_signal_to_mouse(int FR, int BR, int FL, int BL) {
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
  int average_mapped_values = (mapped_fr + mapped_br + mapped_fl + mapped_bl) / 4;
  
  /*
  // Find if there is a big peak, i.e. jump
  signal_buffer[buffer_index] = average_mapped_values;
  buffer_index += 1;
  buffer_index %= BUFFER_SIZE;
  
  int signal_square_difference = 0;
  int signal_square_difference_pos_sum = 0;
  int signal_square_difference_neg_sum = 0;
  for (int i = 1; i < BUFFER_SIZE; i += 1) {
    signal_square_difference = signal_buffer[i] - signal_buffer[i-1];
    if (signal_square_difference > 0) {
      signal_square_difference *= signal_square_difference;
      signal_square_difference_pos_sum += signal_square_difference;
    } else {
      signal_square_difference *= signal_square_difference;
      signal_square_difference_neg_sum += signal_square_difference;
    }
  }
  
  // If pressure is reliesed and applied in short time of period
  // Squrare sum of signal_difference_buffer becomes big,
  // but average becomes small. 
  Serial.println(signal_square_difference_pos_sum);
  
  signal_difference_buffer[diff_buffer_index++] = signal_square_difference_pos_sum - signal_square_difference_neg_sum;
  for (int i = 0; i < DIFF_BUFFER_SIZE; i++) {
     Serial.print(signal_difference_buffer[i]);
     Serial.print(" ");
  }
  Serial.println();
  */
  
  // Estimate up, down, right, and left mouse movement from mapped values.
  int down = (mapped_fr + mapped_fl) / 2;
  int up = (mapped_br + mapped_bl) / 2;
  int right = (mapped_fr + mapped_br) / 2;
  int left = (mapped_fl + mapped_bl) / 2;
  
  float mouse_coefficient = 0.5;
  int mouse_x = mouse_coefficient * (right - left);
  int mouse_y = mouse_coefficient * (up - down);
  Serial.print(mouse_x);
  Mouse.move(mouse_x, mouse_y, 0);
  
  Serial.print("(FR, BR, FL, BL, Up, Down, Right, Left) : (");
  Serial.print(mapped_fr);
  Serial.print(", ");
  Serial.print(mapped_br);
  Serial.print(", ");
  Serial.print(mapped_fl);
  Serial.print(", ");
  Serial.print(mapped_bl);
  Serial.print(", ");
  Serial.print(up);
  Serial.print(", ");
  Serial.print(down);
  Serial.print(", ");
  Serial.print(right);
  Serial.print(", ");
  Serial.print(left);
  Serial.println(")");
  
}

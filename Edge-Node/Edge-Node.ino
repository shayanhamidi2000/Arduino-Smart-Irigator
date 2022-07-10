#include <Wire.h>
#define SENSOR_ADDR 0x40
#define MOTOR_OUT 11
#define BLUETOOTH_DELIMETER_CHARACTER '-'
#define BLUETOOTH_END_CHARACTER '/'


int motor_pwm_value;
unsigned long timer_tick;
String decision_buffer = "";
void setup()
{  
  // Initialize I2C communication as MASTER  
  Wire.begin();
  pinMode(MOTOR_OUT, OUTPUT);
  // Initialize serial communication, set baud rate = 9600  
  Serial.begin(9600);
  Serial.setTimeout(0);
  timer_tick = 0;
  motor_pwm_value = 0;
}

boolean timer_ticked(){
  unsigned long now = millis();
  if (now - timer_tick >= 5000){
    timer_tick = now;
    return true;
  }
  return false;
}

float get_humidity_from_sensor() {
  unsigned int data[2];
  Wire.beginTransmission(SENSOR_ADDR);  
  // Send humidity measurement command, NO HOLD master  
  Wire.write(0xF5);  
  Wire.endTransmission();
  // Request 2 bytes of data  
  Wire.requestFrom(SENSOR_ADDR, 2);
  if(Wire.available() == 2){
    data[0] = Wire.read();    
    data[1] = Wire.read();
    // Convert the data    
    return (((data[0] * 256.0 + data[1]) * 125.0) / 65536.0) - 6;
  }
}

float get_temp_celcius(){
  unsigned int data[2];
  Wire.beginTransmission(SENSOR_ADDR);  
  // Send temperget_temp_celciusature measurement command, NO HOLD master  
  Wire.write(0xF3);  
  Wire.endTransmission();  
  // Request 2 bytes of data  
  Wire.requestFrom(SENSOR_ADDR, 2);
  // Read 2 bytes of data  
  // temp msb, temp lsb  
  if(Wire.available() == 2)  
  {    
    data[0] = Wire.read();    
    data[1] = Wire.read();
    // Convert the data    
    return (((data[0] * 256.0 + data[1]) * 175.72) / 65536.0) - 46.85;
  }
}

void send_sensor_data_to_other_node(float humidity, float temperature){
  String message = String(humidity) + BLUETOOTH_DELIMETER_CHARACTER + String(temperature) + BLUETOOTH_END_CHARACTER;
  Serial.write(&message[0]);
}

void update_motor_value(String new_value_str){
  String decision_str = decision_buffer.substring(0, decision_buffer.length() - 1);
  motor_pwm_value = atoi(&decision_str[0]);
}

void check_status_or_read_bluetooth_data(){
  String data = Serial.readString();
  if (data != ""){
    decision_buffer += data;
    if(data.indexOf(BLUETOOTH_END_CHARACTER) != -1){
      update_motor_value(decision_buffer);
      decision_buffer = "";
    }
  }
}

void loop()
{
  //keep motor engaged
  analogWrite(MOTOR_OUT, motor_pwm_value);
  if (timer_ticked()) // after 5 seconds
    send_sensor_data_to_other_node(get_humidity_from_sensor(), get_temp_celcius());
  check_status_or_read_bluetooth_data();
}

#include <LiquidCrystal.h>
#define RS_LCD 9 
#define EN_LCD 8 
#define D4_LCD 7 
#define D5_LCD 6  
#define D6_LCD 5 
#define D7_LCD 4
#define BLUETOOTH_DELIMETER_CHARACTER '-'
#define BLUETOOTH_END_CHARACTER '/'
#define BLUETOOTH_BAUD_RATE 9600
#define MAX_PWM_SPEED 256

LiquidCrystal lcd(RS_LCD, EN_LCD, D4_LCD, D5_LCD, D6_LCD, D7_LCD);

int last_humidity_value, last_cTemp_value;
String humidity_temp_data_buffer = "";

void extract_humidity_and_temperature(String humidity_temp_data){
  int delimeter_index = humidity_temp_data.indexOf(BLUETOOTH_DELIMETER_CHARACTER);
  String last_humidity_value_str = humidity_temp_data.substring(0, delimeter_index);
  String last_cTemp_value_str = humidity_temp_data.substring(delimeter_index + 1, humidity_temp_data.length() - 1);
  last_humidity_value = atoi(&last_humidity_value_str[0]);
  last_cTemp_value = atoi(&last_cTemp_value_str[0]);
}

int make_decision_with_help_of_sensor_data(){
  if (last_humidity_value > 50)
    return 0;
  if (last_humidity_value < 20)
    return 0.25*MAX_PWM_SPEED;
  if (last_cTemp_value < 25)
    return 0;
  else
    return 0.1*MAX_PWM_SPEED;
}

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(0);
  lcd.begin(20, 4);
}

void show_decision_on_lcd(int decision){
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Temperature: " + String(last_cTemp_value));
  lcd.setCursor(0, 2);
  lcd.print("humidity: " + String(last_humidity_value));
  lcd.setCursor(0, 3);
  lcd.print("decided speed: " + String(decision));
}

void send_decision_to_other_node(int decision){
  String decision_with_end_character = String(decision) + BLUETOOTH_END_CHARACTER;
  Serial.write(&decision_with_end_character[0]);
}

void process_sensor_data(){
  extract_humidity_and_temperature(humidity_temp_data_buffer);
  int decision = make_decision_with_help_of_sensor_data();
  send_decision_to_other_node(decision);
  show_decision_on_lcd(decision);
}

void check_status_or_read_bluetooth_data(){
  String data = Serial.readString();
  if (data != ""){
    humidity_temp_data_buffer += data;
    if(data.indexOf(BLUETOOTH_END_CHARACTER) != -1){
      process_sensor_data();
      humidity_temp_data_buffer = "";
    }
  }
}

void loop() {
  check_status_or_read_bluetooth_data();
}

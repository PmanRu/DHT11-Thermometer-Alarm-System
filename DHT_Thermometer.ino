#include <DHT.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

const int dht_sensor = A3;
DHT dht(dht_sensor, DHT11);

LiquidCrystal_I2C lcd(0x27,16,2); 

float tempf;
float humidity;

uint16_t indicator[2] = {2, 65535}; 

void disarm();
void sensorRead();
void blink();
void buzz();

void setup() {

  lcd.init();
  lcd.backlight();
  
  pinMode(8, OUTPUT);
  pinMode(12, INPUT);

  Serial.begin(9600);

  delay(500);
  Serial.println("DHT Sensor/Humidity test:\n");

  delay(1000);
  dht.begin();
  delay(5000);

}

void loop() {

  disarm();
  sensorRead();
  blink();
  buzz();

  delay(1000);

}

void blink() {

  if (tempf >= 80) {
    indicator[0]++;
  }

  else {
    indicator[0] = 2;
  }

  uint8_t ledIndicator = ((indicator[0] % 2) > 0) ? 1 : 0;
  digitalWrite(8, ledIndicator);

}

void buzz() {

  if ((indicator[0] % 2) > 0) {
    tone(7, indicator[1]);
  }

  else {
    noTone(7);
  }
}

void disarm() {

  if (digitalRead(12)==HIGH) {
    while (tempf >= 80) {
      digitalWrite(8, LOW);
      noTone(7);
      sensorRead();
      delay(1000);

    }
  }
}

void sensorRead() {
  
  tempf = dht.readTemperature(true);
  humidity = dht.readHumidity();

  lcd.setCursor(0,0);
  lcd.print("Humidity: ");
  lcd.setCursor(10,0);
  lcd.print(humidity);
  lcd.setCursor(15,0);
  lcd.print("%");

  lcd.setCursor(0,1);
  lcd.print("Temp. : ");
  lcd.setCursor(8,1);
  lcd.print(tempf);
  lcd.setCursor(13,1);
  lcd.print("F");

  Serial.print(humidity);
  Serial.print("% ");
  Serial.print(tempf);
  Serial.print("F ");
  Serial.println(indicator[0]);
  
}

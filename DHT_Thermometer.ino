#include <DHT.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

const int dht_sensor = A3;
DHT dht(dht_sensor, DHT11);

LiquidCrystal_I2C lcd(0x27,16,2); 

uint16_t indicator[2] = {2, 65535}; 

void disarm();
void sensorRead();
void blink();
void buzz();


void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  
  pinMode(8, OUTPUT);
  pinMode(12, INPUT);

  Serial.begin(9600);

  delay(500);
  Serial.println("DHT Sensor/Humidity test:\n");

  delay(1000);
  dht.begin();

  disarm();
}

void loop() {
  // put your main code here, to run repeatedly:
  disarm();
  sensorRead();
  blink();
  buzz();

  delay(1000);

}

void blink() {

  if (dht.readTemperature(true) >= 80) {
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
    while (dht.readTemperature(true) >= 80) {
      digitalWrite(8, LOW);
      noTone(7);
      sensorRead();
      delay(1000);

    }
  }
}

void sensorRead() {
  lcd.setCursor(0,0);
  lcd.print("Humidity: ");
  lcd.setCursor(10,0);
  lcd.print(dht.readHumidity());
  lcd.setCursor(15,0);
  lcd.print("%");

  lcd.setCursor(0,1);
  lcd.print("Temp. : ");
  lcd.setCursor(8,1);
  lcd.print(dht.readTemperature(true));
  lcd.setCursor(13,1);
  lcd.print("F");

  Serial.print(dht.readHumidity());
  Serial.print("% ");
  Serial.print(dht.readTemperature(true));
  Serial.print("F ");
  Serial.println(indicator[0]);
}

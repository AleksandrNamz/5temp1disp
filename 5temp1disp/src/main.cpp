#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>


// DS18BT20 пины gnd можно объеденить, vcc на общей шине, а пины data подключать в соответствии с пинами
#define SENSOR_PIN_1 2
#define SENSOR_PIN_2 3
#define SENSOR_PIN_3 4
#define SENSOR_PIN_4 5
#define SENSOR_PIN_5 6

#define LCD_ADDR 0x27

#define READ_INTERVAL 10000



OneWire wire1(SENSOR_PIN_1);
OneWire wire2(SENSOR_PIN_2);
OneWire wire3(SENSOR_PIN_3);
OneWire wire4(SENSOR_PIN_4);
OneWire wire5(SENSOR_PIN_5);


DallasTemperature sensor1(&wire1);
DallasTemperature sensor2(&wire2);
DallasTemperature sensor3(&wire3);
DallasTemperature sensor4(&wire4);
DallasTemperature sensor5(&wire5);

// Дисплей по умолчанию sda на а4, scl на а5
LiquidCrystal_I2C lcd(LCD_ADDR, 20, 4);

// Значения температур
float temp1 = 0;
float temp2 = 0;
float temp3 = 0;
float temp4 = 0;
float temp5 = 0;

// Флаги ошибок
bool error1 = false;
bool error2 = false;
bool error3 = false;
bool error4 = false;
bool error5 = false;

// Таймер
unsigned long lastReadTime = 0;



void readAllSensors() {
  float t;

  sensor1.requestTemperatures();
  delay(750);
  t = sensor1.getTempCByIndex(0);
  if (t == DEVICE_DISCONNECTED_C) {
    error1 = true;
  }
  else {
    error1 = false;
    temp1 = t;
  }

  sensor2.requestTemperatures();
  delay(750);
  t = sensor2.getTempCByIndex(0);
  if (t == DEVICE_DISCONNECTED_C) {
    error2 = true;
  }
  else {
    error2 = false;
    temp2 = t;
  }

  sensor3.requestTemperatures();
  delay(750);
  t = sensor3.getTempCByIndex(0);
  if (t == DEVICE_DISCONNECTED_C) {
    error3 = true;
  }
  else {
    error3 = false;
    temp3 = t;
  }

  sensor4.requestTemperatures();
  delay(750);
  t = sensor4.getTempCByIndex(0);
  if (t == DEVICE_DISCONNECTED_C) {
    error4 = true;
  }
  else {
    error4 = false;
    temp4 = t;
  }

  sensor5.requestTemperatures();
  delay(750);
  t = sensor5.getTempCByIndex(0);
  if (t == DEVICE_DISCONNECTED_C) {
    error5 = true;
  }
  else {
    error5 = false;
    temp5 = t;
  }
}

void printSensor(int col, int row, int n, float t, bool err) {
  char buf[12];
  char num[8];

  lcd.setCursor(col, row);

  if (err == true) {
    sprintf(buf, "%d:  ---", n);
    lcd.print(buf);
    return;
  }

  dtostrf(t, 0, 1, num);

  if (t >= 0) {
    sprintf(buf, "%d:+%s", n, num);
  }
  else {
    sprintf(buf, "%d:%s", n, num);
  }

  lcd.print(buf);
}

void updateDisplay() {
  lcd.clear();

  printSensor(0,  0, 1, temp1, error1);
  printSensor(0,  1, 2, temp2, error2);
  printSensor(0,  2, 3, temp3, error3);

  printSensor(10, 0, 4, temp4, error4);
  printSensor(10, 1, 5, temp5, error5);
}



void setup() {
  Serial.begin(9600);

  sensor1.begin();
  sensor1.setResolution(12);
  sensor2.begin();
  sensor2.setResolution(12);
  sensor3.begin();
  sensor3.setResolution(12);
  sensor4.begin();
  sensor4.setResolution(12);
  sensor5.begin();
  sensor5.setResolution(12);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  readAllSensors();
  updateDisplay();

  lastReadTime = millis();
}

void loop() {
  unsigned long now = millis();

  if (now - lastReadTime >= READ_INTERVAL) {
    lastReadTime = now;
    readAllSensors();
    updateDisplay();
  }
}



// TODO наверно можно обьеденить в одну шину, сделать паразитное питание, и разделять датчики по адресу (?)
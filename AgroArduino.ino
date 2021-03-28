#include <avr/sleep.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SENSOR_PIN A0        // Pin del sensor de humedad
#define BUTTON_PIN 2         // Pin del botón del modo ahorro de energía
#define PERIPHERAL_PWR_PIN 3 // Pin de control para los periféricos de 5 V
#define BUTTON_REFRESH 25    // Periodo de sampling del botón (ms)
#define SCREEN_REFRESH 100   // Periodo de refresco de la pantalla (ms)

byte smiley[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};

unsigned long timeNow = 0;

struct Button {
  byte          currState         = 0;
  byte          prevState         = 0;
  unsigned long prevMillis        = 0;
  byte          longPressCountMax = 50; // 80 * 25 = 2000 ms
  byte          longPressCount    = 0;
} button;

struct Sensor {
  float value          = 0;
  int   outputValue    = 0;
  int   oldOutputValue = 0;
} sensor;


// Dirección LCD en 0x27 para una pantalla de 16 caracteres y 2 líneas
LiquidCrystal_I2C lcd(0x27, 20, 4);

// ISR ocupa estar vacío para servir
void wakeUp() {}

void powerDown() {
  sleep_enable();
  attachInterrupt(0, wakeUp, HIGH);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  digitalWrite(LED_BUILTIN, LOW);
  lcd.clear();
  lcd.print("Suspendiendo ...");
  delay(1000); // Aquí no importa usar delay

  lcd.noBacklight();
  delay(1000);
  digitalWrite(PERIPHERAL_PWR_PIN, LOW);
  delay(1000);
  sleep_cpu();
  sleep_disable();
  detachInterrupt(0);
  digitalWrite(       LED_BUILTIN, HIGH);
  digitalWrite(PERIPHERAL_PWR_PIN, HIGH);

  delay(1000);
  lcd.init();
  lcd.backlight();
  lcd.print("Iniciando ...");

  lcd.setCursor(15, 1);
  lcd.write(0);
  delay(1000);
  lcd.clear();
  lcd.print("Humedad:");
}

void shortButtonPress() {
  // Lo que sucede si presiona menos de 2s
}

void longButtonPress() {
  // Lo que sucede si presiona 2s o más
  powerDown();
}

void buttonPress() {
  button.longPressCount = 0;
}

void buttonRelease() {
  if (button.longPressCount >= button.longPressCountMax) {
    longButtonPress();
  } else {
    shortButtonPress();
  }
}

void monitorButton() {
  // Manejo del botón
  // NO USAR DELAY, ACORTA EL CICLO DEL BOTON
  if (millis() > button.prevMillis + BUTTON_REFRESH) {
    button.prevMillis = millis();
    button.currState  = digitalRead(BUTTON_PIN);

    if ((button.prevState == LOW) && (button.currState == HIGH)) {
      buttonPress();
    } else if ((button.prevState == HIGH) && (button.currState == LOW)) {
      buttonRelease();
    } else if (button.currState == HIGH) {
      button.longPressCount++;
    }
    button.prevState = button.currState;
  }
}

void LCDRefresh() {
  lcd.setCursor(9, 0);

  if (millis() > timeNow + SCREEN_REFRESH) {
    timeNow = millis();

    for (uint8_t i = 0; i < 99; i++) {
      sensor.value = sensor.value + analogRead(SENSOR_PIN);
    }
    sensor.value       = sensor.value / 100.0;
    sensor.outputValue = map(sensor.value, 1023, 0, 0, 100);
    lcd.print(sensor.outputValue);
    lcd.print(   "%");

    if (sensor.outputValue != sensor.oldOutputValue) {
      lcd.setCursor(9, 0);
      lcd.print("               ");
    }
    sensor.oldOutputValue = sensor.outputValue;
  }
}

void setup() {
  // Pin Modes
  pinMode(        BUTTON_PIN, INPUT);
  pinMode(PERIPHERAL_PWR_PIN, OUTPUT);
  pinMode(       LED_BUILTIN, OUTPUT);

  digitalWrite(       LED_BUILTIN, HIGH);
  digitalWrite(PERIPHERAL_PWR_PIN, HIGH);


  // Inicializa el LCD
  lcd.init();

  // create a new character
  lcd.createChar(0, smiley);

  // Imprime mensaje al LCD
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Humedad:");
}

void loop() {
  // Monitoreo del botón
  monitorButton();

  // Refrescamiento de la pantalla LCD
  LCDRefresh();
}

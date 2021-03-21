#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

int cnter = 0;

// Dirección LCD en 0x27 para una pantalla de 16 caracteres y 2 líneas
LiquidCrystal_I2C lcd(0x27,20,4);  

void setup()
{
  // Inicializa el LCD
  lcd.init();                     
  lcd.init();
  // Imprime mensaje al LCD
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello");
  lcd.setCursor(0,1);
  lcd.print("World");
}


void loop()
{
  lcd.setCursor(7,1);
  lcd.print(cnter);
  cnter++;
  delay(1000);
}
/*  Sensor de Humedad de Suelo y Luz Solar
 *
 *  Toma medidas de sensores de humedad del suelo y de una celda fotorresistiva
 *  y las despliega en una pantalla LCD.
 *
 *  El sensor de humedad se encuentra en el pin A0 y el de la luz en el A1. Los
 *  sensores son alimentados con los pines digitales D8 y D9, mientras que la
 *  pantalla LCD es alimentado del pin 5V del Arduino
*/
#include <LiquidCrystal_I2C.h>

// Definiciones de los pines
#define SENSOR_HUMEDAD A0
#define SENSOR_LUZ A1
#define PWR_HUMEDAD 8
#define PWR_LUZ 9

// Valores limites de las mediciones del sensor de humedad
// VALOR_AIRE: valor medido con el sensor en el aire
// VALOR_AGUA: valor medido con el sensor en el agua
#define VALOR_AIRE 608
#define VALOR_AGUA 400

// Valores limites de las mediciones del sensor de luz solar
// VALOR_LUZ_PLENA: valor medido con el sensor a luz plena
// VALOR_OSCURO: valor medido con el sensor en la oscuridad
#define VALOR_LUZ_PLENA 713
#define VALOR_OSCURO 26

// Periodo de refresco de la pantalla (ms)
#define SCREEN_REFRESH 15

unsigned long timeNow = 0;

// SensorHumedad define tanto el valor de una medición de humedad entre 0 y 1023
// como el porcentaje real que representa
struct SensorHumedad
{
  int valor = 0;
  int porcentaje = 0;
} humedad;

// SensorLuz define tanto el valor de una medición de luz entre 0 y 1023
// como el porcentaje real que representa
struct SensorLuz
{
  int valor = 0;
  int porcentaje = 0;
} luz;

// Dirección LCD en 0x27 para una pantalla de 16 caracteres y 2 líneas
LiquidCrystal_I2C lcd(0x27, 16, 2);

// LCDRefresh se encarga de refrescar la pantalla con los valores medidos en
// cada periodo
void LCDRefresh()
{
  if (millis() > timeNow + SCREEN_REFRESH)
  {
    timeNow = millis();

    char msg[21];
    
    lcd.setCursor(0, 0);
    sprintf(msg, "Humedad: %-3d%%", humedad.porcentaje);
    lcd.print(msg);

    lcd.setCursor(0, 1);
    sprintf(msg, "Luz: %-3d%%", luz.porcentaje);
    lcd.print(msg);
  }
}

// LoadSensorData se encarga de leer de los puertos de medición analógicos y
// convertir el valor
void LoadSensorData()
{
  // Se lee del sensor de humedad
  humedad.valor = analogRead(SENSOR_HUMEDAD);

  // Se lee del sensor de luz solar
  luz.valor = analogRead(SENSOR_LUZ);

  // Se mapean sus representaciones a porcentajes
  humedad.porcentaje = map(humedad.valor, VALOR_AIRE, VALOR_AGUA, 0, 100);
  luz.porcentaje = map(luz.valor, VALOR_OSCURO, VALOR_LUZ_PLENA, 0, 100);

  // Se definen límites de los porcentajes
  if (humedad.porcentaje >= 100)
  {
    humedad.porcentaje = 100;
  }
  else if (humedad.porcentaje <= 0)
  {
    humedad.porcentaje = 0;
  }
  if (luz.porcentaje >= 100)
  {
    luz.porcentaje = 100;
  }
  else if (luz.porcentaje <= 0)
  {
    luz.porcentaje = 0;
  }

  // Esperamos un tiempo
  delay(250);
}

// Se cargan los modos de pines y se inicializa la pantalla LCD
void setup()
{
  // Los pines PWR se encargan de alimentar a los sensores,
  // esto para librerar los pines 5V y de 3.3V del Arduino
  pinMode(PWR_HUMEDAD, OUTPUT);
  pinMode(PWR_LUZ, OUTPUT);
  digitalWrite(PWR_HUMEDAD, HIGH);
  digitalWrite(PWR_LUZ, HIGH);

  // Inicializa el LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
}

// Ciclo principal
void loop()
{
  LoadSensorData();
  LCDRefresh();
}

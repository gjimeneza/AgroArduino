/*  Capacitive Soil Moisture Sensor Module Test

   Monitor soil moisture analog output 'A0' on ADC input A0
   Power the module off of digital output pin 9 only when taking a measurement
*/
#include <LiquidCrystal_I2C.h>

// Define pins used below
#define SENSOR_HUMEDAD A0
#define SENSOR_LUZ A1
#define PWR_HUMEDAD 8  
#define PWR_LUZ 9    

#define VALOR_AIRE 608
#define VALOR_AGUA 400

#define VALOR_LUZ_PLENA 713
#define VALOR_OSCURO 26

#define SCREEN_REFRESH 15   // Periodo de refresco de la pantalla (ms)

unsigned long timeNow = 0;

struct SensorHumedad {
  int   valor = 0;
  int   porcentaje = 0;
} humedad;

struct SensorLuz {
  int   valor = 0;
  int   porcentaje = 0;
} luz;

// Dirección LCD en 0x27 para una pantalla de 16 caracteres y 2 líneas
LiquidCrystal_I2C lcd(0x27, 20, 4);

void LCDRefresh() {
  if (millis() > timeNow + SCREEN_REFRESH) {
    timeNow = millis();

    char msg[21];
    int temperature = 8; 
    
    lcd.setCursor(0, 0);
    sprintf(msg, "Humedad: %-3d%%", humedad.porcentaje); 
    lcd.print(msg);

    lcd.setCursor(0, 1);
    sprintf(msg, "Luz: %-3d%%", luz.porcentaje); 
    lcd.print(msg);
  }

}

void LoadSensorData(){
  humedad.valor = analogRead(SENSOR_HUMEDAD);
  Serial.println("VALOR HUMEDAD SUELO: " + String(humedad.valor));
  delay(100);
  luz.valor = analogRead(SENSOR_LUZ);
  Serial.println("VALOR LUZ: " + String(luz.valor));
  
  humedad.porcentaje = map(humedad.valor, VALOR_AIRE, VALOR_AGUA, 0, 100);
  luz.porcentaje = map(luz.valor, VALOR_OSCURO, VALOR_LUZ_PLENA, 0, 100);
  
  
  if(humedad.porcentaje >= 100)
  {
    humedad.porcentaje = 100;
    Serial.println("100 %");
  }
  else if(humedad.porcentaje <=0)
  {
    humedad.porcentaje = 0;
    Serial.println("0 %");
  }
  else
  {
    Serial.print(humedad.porcentaje);
    Serial.println("%");
  }

  if(luz.porcentaje >= 100)
  {
    luz.porcentaje = 100;
  }
  else if(luz.porcentaje <=0)
  {
    luz.porcentaje = 0;
  }
  
  delay(250);
 }


//===============================================================================
//  Initialization
//===============================================================================
void setup()
{
  Serial.begin(9600);          // Set Serial Monitor window comm speed

  pinMode(PWR_HUMEDAD, OUTPUT);    // Set pin used to power sensor as output
  pinMode(PWR_LUZ, OUTPUT);    // Set pin used to power sensor as output
  digitalWrite(PWR_HUMEDAD, HIGH);  // Set to LOW to turn sensor off at start
  digitalWrite(PWR_LUZ, HIGH);  // Set to LOW to turn sensor off at start

  // Inicializa el LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
}

void loop() {
  LoadSensorData();
  LCDRefresh();
}

#include "arduino_secrets.h"

#include "Wire.h"
#include "LiquidCrystal_I2C.h"
LiquidCrystal_I2C LCD(0x27, 16, 2);

int mq3;
bool calculoRealizado = false;

void setup() {
  pinMode(A1, INPUT);
  analogWrite(A1, LOW);

  Serial.begin(9600);
  LCD.init();
  LCD.backlight();
}

void loop() {
  if (!calculoRealizado) {
    mq3 = analogRead(A1);

    Serial.print("Alcohol: ");
    Serial.println(mq3);

    LCD.setCursor(0, 0);
    LCD.print("Alcohol: ");
    LCD.print(mq3);

    // Ajusta el valor del umbral segÃºn sea necesario
    int umbral = 100;

    // Agrega la condiciÃ³n para verificar si la lectura es mayor que el umbral
    if (mq3 > umbral) {
      Serial.println("Advertencia: No puedes conducir.");
      LCD.setCursor(0, 1);
      LCD.print("No puedes conducir");
    } else {
      // Si MQ3 es 0, la concentraciÃ³n de alcohol es 0 mg/L
      if (mq3 == 0) {
        Serial.println("ConcentraciÃ³n de alcohol: 0 mg/L");
        LCD.setCursor(0, 1);
        LCD.print("No tomÃ³ nada");
      } else {
        // Realizar la regla de tres simple
        float concentracionAlcohol = (mq3 * 0.05) / 1.0;  // Regla de tres simple
        Serial.print("ConcentraciÃ³n de alcohol: ");
        Serial.print(concentracionAlcohol);
        Serial.println(" mg/L");

        LCD.setCursor(0, 1);
        LCD.print("Conc. de alcohol: ");
        LCD.print(concentracionAlcohol);
        LCD.print(" mg/L");

        // Ajusta los lÃ­mites segÃºn sea necesario
        float limiteConducir = 0.5;
        if (concentracionAlcohol > limiteConducir) {
          Serial.println("Puedes conducir.");
        } else {
          Serial.println("Riesgo para la vida: No conduzcas.");
        }
      }
    }

    calculoRealizado = true;
  }

  // MantÃ©n esta parte del cÃ³digo para evitar que se vuelva a calcular en cada iteraciÃ³n
  delay(1000);  // Ajusta segÃºn sea necesario
  LCD.clear();
  delay(500);  // Ajusta segÃºn sea necesario
}

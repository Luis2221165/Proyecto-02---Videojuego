#include <Arduino.h>

const int pinGPIO4 = 4;  // Define el pin GPIO4 para el comando 1202
const int pinGPIO5 = 5;  // Define el pin GPIO5 para el comando 777

void setup() {
    Serial.begin(115200);  // Inicializa la comunicación serial
    pinMode(pinGPIO4, OUTPUT);  // Configura el pin GPIO4 como salida
    pinMode(pinGPIO5, OUTPUT);  // Configura el pin GPIO5 como salida
    digitalWrite(pinGPIO4, HIGH);  // Asegúrate de que el pin GPIO4 esté inicialmente en estado alto
    digitalWrite(pinGPIO5, HIGH);   // Asegúrate de que el pin GPIO5 esté inicialmente en estado alto
}

void loop() {
    if (Serial.available()) {  // Verifica si hay datos disponibles en la comunicación serial
        String command = Serial.readStringUntil('\n'); // Lee el comando hasta el salto de línea
        command.trim();  // Elimina espacios en blanco alrededor del comando

        if (command.equals("1")) {  // Verifica si el comando es "1"
            digitalWrite(pinGPIO4, LOW); // Envía un 0 (GND) en GPIO4
            delay(500);  // Mantiene el pin en bajo por medio segundo
            pinMode(pinGPIO4, INPUT); // Cambia el pin GPIO4 a modo entrada (desconectado)
        }
        else if (command.equals("2")) { // Verifica si el comando es "2"
            digitalWrite(pinGPIO5, LOW); // Envía un 0 (GND) en GPIO5
            delay(500); // Mantiene el pin en bajo por medio segundo
            digitalWrite(pinGPIO5, HIGH); // Mantiene el pin GPIO5 en alto después
        }
    }
}


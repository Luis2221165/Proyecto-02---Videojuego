#include <esp_now.h>
#include <WiFi.h>
#include <Arduino.h>

const int pinGPIO4 = 4;  // Define el pin GPIO4 para el comando "1"
const int pinGPIO5 = 5;  // Define el pin GPIO5 para el comando "2"

// Función para recibir datos por ESP-NOW
void onDataRecv(const esp_now_recv_info* sender, const uint8_t* incomingData, int len) {
  // Imprimir el primer carácter recibido
  Serial.println((char)*incomingData);
}

void setup() {
  Serial.begin(115200);  // Inicializa la comunicación serial

  // Inicializa WiFi en modo estación
  WiFi.mode(WIFI_STA);

  // Inicializa ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error inicializando ESP-NOW");
    return;
  }

  // Registra la función de callback para recibir datos
  esp_now_register_recv_cb(onDataRecv);

  // Configura los pines GPIO4 y GPIO5 como salidas
  pinMode(pinGPIO4, OUTPUT);
  pinMode(pinGPIO5, OUTPUT);

  // Inicializa los pines en estado alto
  digitalWrite(pinGPIO4, HIGH);
  digitalWrite(pinGPIO5, HIGH);
}

void loop() {
  // Verifica si hay datos disponibles en la comunicación serial
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');  // Lee el comando hasta el salto de línea
    command.trim();  // Elimina espacios en blanco alrededor del comando

    // Comando para controlar el pin GPIO4 (equivalente a "1202")
    if (command.equals("1")) {
      digitalWrite(pinGPIO4, LOW);  // Envía un 0 (GND) en GPIO4
      delay(500);  // Mantiene el pin en bajo por medio segundo
      pinMode(pinGPIO4, INPUT);  // Cambia el pin GPIO4 a modo entrada (desconectado)
    }
    // Comando para controlar el pin GPIO5 (equivalente a "777")
    else if (command.equals("2")) {
      digitalWrite(pinGPIO5, LOW);  // Envía un 0 (GND) en GPIO5
      delay(500);  // Mantiene el pin en bajo por medio segundo
      digitalWrite(pinGPIO5, HIGH);  // Vuelve a poner el pin GPIO5 en alto
    }
  }
}

#include "WiFi.h"

void setup() {
  Serial.begin(115200);

  // Inicia el modo estación (WIFI_MODE_STA)
  WiFi.mode(WIFI_MODE_STA);

  // Agrega un pequeño retraso para permitir la configuración del WiFi
  delay(100);

  // Obtén y muestra la dirección MAC
  String macAddress = WiFi.macAddress();
  Serial.println("MAC Address: " + macAddress);
}

void loop() {
  // No es necesario hacer nada en loop
}


#include <esp_now.h>
#include <WiFi.h>

// Dirección MAC del maestro
const uint8_t maestroMAC[] = { 0x80, 0x7D, 0x3A, 0x0F, 0x7B, 0x91 };

// Pines del joystick
const int joyVertPin = 35; // Pin del eje vertical del joystick (VRy)
const int joyHorizPin = 34; // Pin del eje horizontal del joystick (VRx)
const int buttonA = 4; // Pin del botón A
const int buttonB = 2; // Pin del botón B
const int joyButtonPin = 15; // Pin del botón del joystick

// Pines del LED RGB
const int redPin = 14;
const int greenPin = 27;
const int bluePin = 12;

// Pin del LED de voltaje
const int voltageLedPin = 13; // Puedes elegir otro pin si lo necesitas

// Valores del umbral del joystick
const int thresholdUp = 3072; // Umbral para mover hacia arriba/derecha (75% del rango)
const int thresholdDown = 1024; // Umbral para mover hacia abajo/izquierda (25% del rango)

// Variables para almacenar el último estado
String lastVertState = "";
String lastHorizState = "";

// Función para controlar los colores del LED RGB
void setRGBColor(bool red, bool green, bool blue) {
    digitalWrite(redPin, red ? LOW : HIGH);   // LOW enciende el LED debido a cátodo común
    digitalWrite(greenPin, green ? LOW : HIGH);
    digitalWrite(bluePin, blue ? LOW : HIGH);
}

// Función para enviar datos
void sendData(const char* message) {
    esp_err_t result = esp_now_send(maestroMAC, (uint8_t *)message, strlen(message));
    
    // Depuración para ver el estado del envío
    if (result == ESP_OK) {
        Serial.println("Mensaje enviado con éxito: " + String(message));
    } else {
        Serial.println("Error al enviar: " + String(message) + ", código de error: " + String(result));
        setRGBColor(true, false, false); // Rojo en caso de error al enviar
    }
}

// Callback cuando se envía un mensaje
void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("Estado de la transmisión: ");
    
    if (status == ESP_NOW_SEND_SUCCESS) {
        Serial.println("Envío exitoso");
        setRGBColor(false, false, true); // Azul cuando la conexión es exitosa
    } else {
        Serial.println("Error de envío");
        setRGBColor(false, true, false); // Verde cuando hay problemas en la conexión
    }
}

void setup() {
    Serial.begin(115200);

    // Configuración de los pines del LED RGB
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);

    // Configuración del LED de voltaje
    pinMode(voltageLedPin, OUTPUT);
    digitalWrite(voltageLedPin, HIGH); // Encender el LED para indicar que hay voltaje

    // Inicializa WiFi en modo estación
    WiFi.mode(WIFI_STA);

    // Inicializa ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error inicializando ESP-NOW");
        setRGBColor(true, false, false); // Rojo si no se pudo inicializar ESP-NOW
        return;
    } else {
        setRGBColor(false, false, true); // Azul si la inicialización fue exitosa
    }

    // Agregar el maestro a la lista de pares
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, maestroMAC, 6);
    peerInfo.channel = 0;  // Canal de WiFi (0 para el canal actual)
    peerInfo.encrypt = false;  // Sin cifrado

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Error al agregar el maestro como par");
        setRGBColor(true, false, false); // Rojo si no se pudo agregar el maestro
        return;
    }

    // Configura los pines del joystick y botones
    pinMode(buttonA, INPUT_PULLUP);  // Botón A
    pinMode(buttonB, INPUT_PULLUP);  // Botón B
    pinMode(joyButtonPin, INPUT_PULLUP);  // Botón del joystick

    // Asigna la función de callback para cuando se envía un mensaje
    esp_now_register_send_cb(onSent);
}

void loop() {
    // Leer el joystick
    int joyVertValue = analogRead(joyVertPin);
    int joyHorizValue = analogRead(joyHorizPin);

    // Detectar movimiento en el joystick y enviar datos según el umbral
    // Prioridad al eje vertical, pero solo si está en posición neutral horizontalmente
    if (joyHorizValue >= thresholdDown && joyHorizValue <= thresholdUp) {
        if (joyVertValue > thresholdUp && lastVertState != "w") { 
            sendData("w");
            lastVertState = "w";
        } 
        else if (joyVertValue < thresholdDown && lastVertState != "s") { 
            sendData("s");
            lastVertState = "s";
        } 
        else if (joyVertValue >= thresholdDown && joyVertValue <= thresholdUp && lastVertState != "center") {
            lastVertState = "center";
        }
    }

    // Si el eje vertical está en posición neutral, revisar el eje horizontal
    if (joyVertValue >= thresholdDown && joyVertValue <= thresholdUp) {
        if (joyHorizValue > thresholdUp && lastHorizState != "a") {  // Cambié "d" por "a"
            sendData("a");
            lastHorizState = "a";
        } 
        else if (joyHorizValue < thresholdDown && lastHorizState != "d") {  // Cambié "a" por "d"
            sendData("d");
            lastHorizState = "d";
        } 
        else if (joyHorizValue >= thresholdDown && joyHorizValue <= thresholdUp && lastHorizState != "center") {
            lastHorizState = "center";
        }
    }

    // Verifica si el botón del joystick está presionado
    if (digitalRead(joyButtonPin) == LOW) {
        sendData("q");  // Envía el mensaje cuando el botón del joystick es presionado
        delay(500);    // Espera para evitar múltiples envíos
    }

    // Verifica si el botón A está presionado
    if (digitalRead(buttonA) == LOW) {
        sendData("t");  // Envía el mensaje A
        delay(500);    // Espera para evitar múltiples envíos
    }

    // Verifica si el botón B está presionado
    if (digitalRead(buttonB) == LOW) {
        sendData("e");  // Envía el mensaje B
        delay(500);    // Espera para evitar múltiples envíos
    }

    delay(100); // Pausa para evitar saturar la comunicación
}

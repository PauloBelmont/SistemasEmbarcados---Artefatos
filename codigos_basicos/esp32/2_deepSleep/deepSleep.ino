#include <WiFi.h>
#include "DHT.h"

#define LED_PIN 25
#define SENSOR_PIN 22
#define DHT_TYPE DHT22
#define PWM_CHANNEL 0   // Canal 0 do PWM
#define PWM_FREQ 5000   // Frequência 5 kHz
#define PWM_RES 8       // Resolução de 8 bits (0–255)

DHT dht(SENSOR_PIN, DHT_TYPE);

unsigned long lastReport = 0;
unsigned long ledOnTime = 0;
unsigned long cpuActiveTime = 0;

void setup() {
    Serial.begin(115200);

    // Configuração do PWM
    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RES);
    ledcAttachPin(LED_PIN, PWM_CHANNEL);

    // Desliga Wi-Fi e Bluetooth
    WiFi.mode(WIFI_OFF);
    btStop();

    dht.begin();
    Serial.println(">> DHT22 iniciado");

    // ===== Leitura do sensor =====
    unsigned long start = millis();

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("- NAN detectado");
    } else {
        Serial.print("Umidade: ");
        Serial.print(humidity);
        Serial.print(" | Temperatura: ");
        Serial.print(temperature);
        Serial.println(" ºC");
    }

    // ===== Controle do LED =====
    if (temperature > 25.0) {
        ledcWrite(PWM_CHANNEL, 128); // meio brilho (128/255)
        ledOnTime += 500;
    } else {
        ledcWrite(PWM_CHANNEL, 0);   // LED apagado
    }

    delay(100);

    // ===== Entra em deep sleep por 10s =====
    esp_sleep_enable_timer_wakeup(1000000 * 10);
    esp_deep_sleep_start();
}

void loop() {
    // vazio, porque o ESP reinicia após o deep sleep
}

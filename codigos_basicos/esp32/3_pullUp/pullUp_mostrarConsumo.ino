//dvfs.ino
//FreeRTOS -> task ->  OS -> setFreq

//Bibliotecas (<> = sistema, "" = usuário)
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/rtc_cntl.h"
#include "esp_private/esp_clk.h"

const int ledPin = 25;
const int btnPin = 4;

// Função para estimar consumo baseado na frequência
float estimar_consumo(uint32_t freq_cpu) {
  const float consumo_base = 35.0f;  // mA
  const float fator_consumo = 0.356f; // mA/MHz
  // F(x) = consumo_base + (freq_cpu - 80) * fator_consumo
  return consumo_base + ((float)freq_cpu - 80.0f) * fator_consumo;
}

// Explicação da função PULLUP
void pullup() {
  /*
    INPUT_PULLUP ativa um resistor interno que mantém o pino em nível alto (3.3V) 
    quando não está conectado a nada. Isso evita sinais flutuantes.
    No caso do botão, conectamos um lado ao pino e o outro ao GND.
    Assim:
      - Botão solto: leitura HIGH (1)
      - Botão pressionado: leitura LOW (0)
  */
  Serial.println("PULLUP ativo no pino do botão. Botão solto = HIGH, pressionado = LOW.");
}

// Função para mostrar consumo estimado
void mostrarConsumo() {
  uint32_t freq_hz = esp_clk_cpu_freq();    // Frequência da CPU em Hz
  uint32_t freq_mhz = freq_hz / 1000000;    // Converte para MHz
  float consumo = estimar_consumo(freq_mhz);
  Serial.print("Frequência CPU: ");
  Serial.print(freq_mhz);
  Serial.print(" MHz | Consumo estimado: ");
  Serial.print(consumo);
  Serial.println(" mA");
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(btnPin, INPUT_PULLUP); // Ativa resistor de pull-up interno
  pullup(); // Mostra explicação no Serial
}

// void loop() {
//   digitalWrite(ledPin, HIGH);
//   delay(500);
//   digitalWrite(ledPin, LOW);
//   delay(500);
//   mostrarConsumo();
//   pullup();
// }

void loop() {
  // Lê botão (LOW = pressionado, HIGH = solto)
  bool botaoPressionado = (digitalRead(btnPin) == LOW);

  if (botaoPressionado) {
    digitalWrite(ledPin, LOW); // Desliga LED
    Serial.println("Botão pressionado -> LED desligado");
    delay(2000);
  } else {
    digitalWrite(ledPin, HIGH);
    delay(2000);
  }
  mostrarConsumo();
}
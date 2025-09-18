#include <DHT.h>

#define LED_PIN 25
#define SENSOR_PIN 22
#define DHT_TYPE DHT22

//Instancia um sensor DHT, informando o pino e o tipo desse sensor
DHT dht(SENSOR_PIN, DHT_TYPE);

//Variáveis de controle
unsigned long lastReport= 0;
unsigned long ledOnTime = 0;
unsigned long cpuActiveTime = 0;

//Configuração inicial
void setup(){
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT); //Define o pino do led como output
  dht.begin(); //Inicia o sensor DHT22
  Serial.println(">> DHT22 iniciado!");
}

void loop(){
  //Marca o início do funcionamento do loop
  unsigned long start = millis();
  
  //Realiza leitura pelos sensores
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  //Verifica se não houve erro em nenhuma das leituras, e exibe o valor das mesmas no display Serial
  if (isnan(humidity) || isnan(temperature)){
    Serial.println("- NAN detectado");
  } else{
    Serial.print("Umidade: ");
    Serial.print(humidity);
    Serial.print("Temparatura: ");
    Serial.print(temperature);
    Serial.print("ºC");
  }
  //Acende o LED se ler uma temperatura acima de 25º
  if (temperature>25.0){
    digitalWrite(LED_PIN,HIGH);
    ledOnTime+=2000;
  }else{
    digitalWrite(LED_PIN,LOW);
  }
  delay(3000);
  //Incrementa de CPU ativo em cpuActiveTime (cpuActiveTime += millis() - start)
  cpuActiveTime +=millis() - start;
  //Reseta o contador de cpuActiveTime se o tempo de cpu ultrapassar 30 segundos
  if(millis() - lastReport >= 30000){
    lastReport = millis();
    cpuActiveTime =0;
    ledOnTime =0;
  }
}
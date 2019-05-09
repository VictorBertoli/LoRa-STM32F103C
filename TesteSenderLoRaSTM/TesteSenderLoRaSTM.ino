#include <SPI.h>
#include <LoRa.h>

#define BAND 433E6 //Frequência do radio - exemplo : 433E6, 868E6, 915E6
#define SCK PB3   //para o maple: 5   GPIO5  SCK
#define MISO PB4 //para o maple: 19 - GPIO19 MISO
#define MOSI PB5 //para o maple: 27 - GPIO27 MOSI
#define SS PA15   //para o maple: 18 - GPIO18 CS
#define RST PA12  //para o maple: 14 - GPIO14 RESET
#define DI00 PA11 //GPIO26 IRQ( Interrupt Request) -opcional-
#define INTERVAL 500 //Intervalo entre os envios

//Constante para informar ao Slave que queremos os dados
const String GETDATA = "getdata";

//Tempo do último envio
long lastSendTime = 0;

void setup(){
  Serial.begin(9600);
  while (!Serial);
  
  //Chama a configuração inicial do LoRa
  setupLoRa();
}

void loop(){
  //Se passou o tempo definido em INTERVAL desde o último envio
  if (millis() - lastSendTime > INTERVAL){
    //Marcamos o tempo que ocorreu o último envio
    lastSendTime = millis();
    //Envia o pacote para informar ao Slave que queremos receber os dados
    sender();
  }
}

//Configurações iniciais do LoRa
void setupLoRa(){ 
  //Inicializa a comunicação
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI00);
  //Inicializa o LoRa
  if (!LoRa.begin(BAND)){
   Serial.println("Starting LoRa failed!");
    while (1);
  }

  //Ativa o crc
  LoRa.enableCrc();
}

void sender(){
  Serial.println("send:");
  //Inicializa o pacote
  LoRa.beginPacket();
  //Envia o que está contido em "GETDATA"
  LoRa.print(GETDATA);
  //Finaliza e envia o pacote
  LoRa.endPacket();
}

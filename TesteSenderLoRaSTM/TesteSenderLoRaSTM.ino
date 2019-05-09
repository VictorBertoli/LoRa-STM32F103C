#include <SPI.h>
#include <LoRa.h>

//o código original era destinado ao STM32 Maple Mini, e originalmente, era para Duplex Callback

#define BAND 915E6 //Frequência do radio - exemplo : 433E6, 868E6, 915E6
#define SCK PB3   //para o maple: 5   GPIO5  SCK
#define MISO PB4 //para o maple: 19 - GPIO19 MISO
#define MOSI PB5 //para o maple: 27 - GPIO27 MOSI
#define SS PA15   //para o maple: 18 - GPIO18 CS
#define RST PA12  //para o maple: 14 - GPIO14 RESET
#define DI00 PA11 //GPIO26 IRQ( Interrupt Request) -opcional-
#define INTERVAL 500 //Intervalo entre os envios

const String GETDATA = "getdata";

//Tempo do último envio
long lastSendTime = 0;

void setup(){
  Serial.begin(9600);
  while (!Serial);
  
  //Configuração inicial do LoRa
  setupLoRa();
}

void loop(){
  //Se passou o tempo definido em INTERVAL desde o último envio
  if (millis() - lastSendTime > INTERVAL){
    //Marca o tempo do último envio
    lastSendTime = millis();
    //Envia os dados
    sender();
  }
}

//Função das configurações iniciais do LoRa
void setupLoRa(){ 
  //Inicializa a comunicação:
 
  //A linha a seguir está com o seguinte problema: No matching function for call to 'SPIClass::begin(<anonymous enum>, <anonymous enum>, <anonymous enum>, <anonymous enum>)
  SPI.begin(SCK, MISO, MOSI, SS);     //no programa original há estes parâmetros, porem na biblioteca SPI begin é void
  LoRa.setPins(SS, RST, DI00);
  
  //Inicializa o LoRa ()
  if (!LoRa.begin(BAND)){
   Serial.println("Starting LoRa failed!");     //caso a linha 43 seja comentada, o programa compila porém entra neste IF
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

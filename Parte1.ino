/*
 * TRABALHO FINAL DE ESTRUTURA DE DADOS
 * ARQUIVO REFERENTE A LEITURA E GRAVAÇÃO DE DADOS E A INSERÇÃO DOS MESMOS NA ESTRUTURA LISTA
 * GIOVANE AGUIAR DE ALMEIDA, PAULO HENRIQUE DE OLIVEIRA, RENAN GUIMARÃES
 */
#include "Arduino.h"
#include <SdFat.h>
#include <SPI.h>
#include <LiquidCrystal.h>
#include "lista_umidade.h"

/* Variaveis para leitura do arquivo texto */
SdFat sdCard;
SdFile Arquivo;

/* local do chip a ser usado */
const int chipSelect = 4;

/* Inicializando o lcd */
const int rs = 3, en = 5, d4 = A1, d5 = 2, d6 = 7, d7 = 6;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

const int pinoSensor = A0;

/* Funcao principal do Arduino */
void setup() {
    Serial.begin(9600);
    if(!sdCard.begin(chipSelect,SPI_HALF_SPEED))sdCard.initErrorHalt();
    lcd.begin(16,2); 
    pinMode(7, OUTPUT);
    pinMode(8, INPUT_PULLUP); 
    pinMode(9, INPUT_PULLUP);  
    pinMode(10, OUTPUT);
}
/* Variaveis que serão utilizadas durante a execução */
int media_das_umidades = 0;
int maior_umidade = 0;
int numero_de_loops = 0;
Lista* lista_umidade = new Lista();

void loop() {
    /*Primeira parte a ser executada, onde são realizadas as leituras de umidade
    a cada um segundo e o numero de loop e incrementado ate atingir um limite determindado */
    lcd.clear();
    lcd.setCursor(0,0);
    delay(2000);
    numero_de_loops++;
    Serial.print("Numero da execucao: ");
    Serial.println(numero_de_loops);
    delay(1000);
    
    /*Registra a umidade, imprime e insere na lista*/
    lcd.print("Umidade");
    int umidade = analogRead(pinoSensor);
    umidade = map(umidade, 1023,0 , 0, 100);
    lcd.setCursor(1,1);
    lcd.print(umidade);
    lcd.print("%");
    lista_umidade->Inserir_na_Lista(umidade);
    /*Se o sensor registrar uma umidade menor do que 40% acendemos o led */
    if(umidade<40){
        digitalWrite(7,HIGH);
        delay(1000);
        digitalWrite(7,LOW);
    }
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    if(numero_de_loops == 40) {
        /*Gravar os dados da lista no arquivo*/
        lista_umidade->Gravar_no_SD(Arquivo);
        media_das_umidades = lista_umidade->MediaDasUmidades();
        maior_umidade = lista_umidade->MaiorUmidade();
        /*Imprime a media e a maior umidade*/
        Serial.print("Media: ");
        Serial.print(media_das_umidades);
        Serial.println("%");
        lcd.print("Media");
        lcd.setCursor(1,1);
        lcd.print(media_das_umidades);
        lcd.print("%");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0,0);
        Serial.print("Maior: ");
        Serial.print(maior_umidade);
        Serial.println("%");
        lcd.print("Maior");
        lcd.setCursor(1,1);
        lcd.print(maior_umidade);
        lcd.print("%");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0,0);
        lista_umidade->~Lista();
        if(Arquivo.open("umidade.bin",O_RDWR)){
            int printar;
            while(Arquivo.read((char*)&printar, sizeof(int))){
                Serial.print(" / ");
                Serial.print(printar);
                Serial.print(" / ");
                delay(500);
            }
            Serial.println(' ');
            Arquivo.close(); 
            Serial.println("Dados gravados com sucesso!");
            lcd.print("Dados gravados");
        }
        /* loop para a retirada do cartao */ 
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Encerre o programa");
        //Rolagem para a esquerda
        for (int posicao = 0; posicao < 2; posicao++) {
            lcd.scrollDisplayLeft();
            delay(600);
        }
        for (int posicao = 0; posicao < 2; posicao++) {
            lcd.scrollDisplayRight();
            delay(600);
        }
        Serial.println("Programa encerrado, reinicializacao em 60 segundos");
        delay(60000);
        Serial.println("Programa reiniciado");
        lcd.print("Programa reiniciado");
        delay(6500);
        numero_de_loops = 0;
    }
}

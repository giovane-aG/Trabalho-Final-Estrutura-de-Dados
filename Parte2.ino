/*
 * TRABALHO FINAL DE ESTRUTURA DE DADOS
 * ARQUIVO REFERENTE A TRANSFERÊNCIA DOS DADOS DO ARQUIVO PARA A ARVORE,
 * EM SEGUIDA A BUSCA E REMOÇÃO DO MAIOR VALOR INSERIDO, FINALIZANDO COM A ORDENAÇÃO E GRAVANDO OS DADOS DE VOLTA NO ARQUIVO
 * GIOVANE AGUIAR DE ALMEIDA, PAULO HENRIQUE DE OLIVEIRA, RENAN GUIMARÃES
 */
#include "Arduino.h"
#include "SdFat.h"
#include <SPI.h>
#include <LiquidCrystal.h>
#include "ABB.h"

// Variaveis para leitura do arquivo texto
SdFat sdCard;
SdFile Arquivo;

const int chipSelect = 4; // local do chip a ser usado

//Inicializando o lcd
const int rs = 3, en = 5, d4 = A1, d5 = 2, d6 = 7, d7 = 6;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

          /* Funcao principal do Arduino */
void setup() {
    Serial.begin(9600);
    if(!sdCard.begin(chipSelect,SPI_HALF_SPEED))sdCard.initErrorHalt();
    lcd.begin(16,2); 
    pinMode(8, INPUT_PULLUP); 
    pinMode(9, INPUT_PULLUP);  
    pinMode(10, OUTPUT);
}
    ABB* arvore_umidade = new ABB();
    
          /* Funcao loop do Arduino */
void loop() {
    /*Aqui lemos o arquivo e o inserimos na arvore para realizar busca e remocao*/
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Lendo...");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,1);
    Serial.println("Lendo arquivo...");
    delay(1000);
        /* Inserção na arvore */
    if(Arquivo.open("umidade.bin",O_RDWR)){
        int aSerInserido;
        while(Arquivo.read((char*)&aSerInserido, sizeof(int))){
            arvore_umidade->insere(aSerInserido);
            Serial.print(aSerInserido);
            Serial.print(' ');
        }
        lcd.print("Dados inseridos");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0,1);
        Arquivo.close();
    }
        /* Busca e Remocao */
    if(Arquivo.open("umidade.bin",O_RDWR)) {
        Serial.println("Buscando no arquivo...");
        lcd.print("Buscando...");
        delay(1000);
        lcd.clear();
        lcd.setCursor(0,1);
        int procurado = arvore_umidade->Maior();
        /* variavel usada para receber os dados do arquivo e comparar com o procurado */
        int recebido;
        bool encontrou;
        while(Arquivo.read((char*)&recebido, sizeof(int)) and !encontrou) {
            if(recebido == procurado){
                Serial.print("O valor ");
                Serial.print(recebido);
                Serial.println(" foi encontrado");
                encontrou = true;
            }
        }
        if(!encontrou) {
            Serial.println("Valor nao encontrado no arquivo");
            lcd.print("Valor nao encontrado");
            for (int posicao = 0; posicao < 2; posicao++) {
                lcd.scrollDisplayLeft();
                delay(800);
            }
            for (int posicao = 0; posicao < 2; posicao++) {
                lcd.scrollDisplayRight();
                delay(800);
            }
            lcd.clear();
            lcd.setCursor(0,1);
        }else{
            Serial.print("Maior valor encontrado: ");
            Serial.println(arvore_umidade->Maior());
            arvore_umidade->Remove(procurado);
            Serial.println("Valor encontrado removido");
            lcd.print(procurado);
            lcd.setCursor(1,0);
            lcd.print(" Foi removido");
            delay(1500);
            lcd.clear();
            lcd.setCursor(0,1);
            /* Metodo de ordenação em memoria da abb */
            arvore_umidade->GravaEmOrdem();
        }
    }
    Arquivo.close();
    arvore_umidade->~ABB();
    Serial.println("Impressao do arquivo");
    if(Arquivo.open("umidade.bin",O_RDWR)){
        int printar;
        while(Arquivo.read((char*)&printar, sizeof(int))){
            Serial.print(" / ");
            Serial.print(printar);
            Serial.print(" / ");
            delay(400);
        }
        Serial.println(' ');
        Arquivo.close();
        Serial.println("Dados ordenados com sucesso!");
    }
    
    /* loop para a retirada do cartao */ 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Encerre o programa");
    
    /* Rolagem para a esquerda e direita */
    for (int posicao = 0; posicao < 2; posicao++) {
        lcd.scrollDisplayLeft();
        delay(1000);
    }
    for (int posicao = 0; posicao < 2; posicao++) {
        lcd.scrollDisplayRight();
        delay(1500);
    }
    delay(80000);
}

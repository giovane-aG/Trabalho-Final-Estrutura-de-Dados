/*
 * TRABALHO FINAL DE ESTRUTURA DE DADOS
 * ARQUIVO REFERENTE A ESTRUTURA LISTA ENCADEADA
 * GIOVANE AGUIAR DE ALMEIDA, PAULO HENRIQUE DE OLIVEIRA, RENAN GUIMARÃES
 */

#include "Arduino.h"
#include "SdFat.h"
#include <LiquidCrystal.h>

/* Estrutura Lista */

class Noh {
    friend class Lista;
  private:
    int mUmidade;
    Noh *prox;
  public:
    Noh(int h) {
      mUmidade = h;
      prox = NULL;
    }
};

class Lista {
  private:
    Noh *Inicio;
    Noh *Fim;
  public:
    Lista();
    ~Lista();
    void PercorreImprimindo();
    void Inserir_na_Lista(int h);
    int MediaDasUmidades();
    int MaiorUmidade();
    void Gravar_no_SD(SdFile Arquivo);
};

/* Construtor da Lista */
Lista::Lista() {
  Inicio = NULL;
  Fim = NULL;
}

/* Destrutor da Lista */
Lista::~Lista() {
  Noh* auxiliar = Inicio;
  while (auxiliar != NULL) {
    Inicio = Inicio->prox;
    delete auxiliar;
    auxiliar = Inicio;
  }
  Inicio = NULL;
  Fim = NULL;
}

void Lista::PercorreImprimindo(){
  Noh* percorredor = Inicio;
  while(percorredor != NULL) {
    Serial.print(percorredor->mUmidade);
    Serial.print(" / ");
    percorredor = percorredor->prox;
  }
  Serial.println(" ");
}

void Lista::Inserir_na_Lista(int h) {
  if (Inicio == NULL) {
    Noh* novo = new Noh(h);
    Inicio = novo;
    Serial.print(" Umidade Inserida:  ");
    Serial.print(Inicio->mUmidade);
    Serial.println("%");
    Fim = Inicio;
  }
  else {
    Noh* novo = new Noh(h);
    Fim->prox = novo;
    Fim = novo;
    Serial.print(" Umidade Inserida:  ");
    Serial.print(Fim->mUmidade);
     Serial.println("%");
  }
}

/* Funcao que retorna a media de todas as umidades gravadas ate entao */
int Lista::MediaDasUmidades() {
  Noh* percorredor = Inicio;
  int contador = 0;
  int soma = 0;
  while (percorredor != NULL) {
    soma += percorredor->mUmidade;
    contador++;
    percorredor = percorredor->prox;
  }
  return (soma / contador);
}

/* Funcao que retorna a maior umidade */
int Lista::MaiorUmidade() {
  Noh* percorredor = Inicio;
  float maior = 0;
  while (percorredor != NULL) {
    if (percorredor->mUmidade > maior)
      maior = percorredor->mUmidade;
    percorredor = percorredor->prox;
  }
  return maior;
}

/* Funcao que grava os ints da Lista no SD */
void Lista::Gravar_no_SD(SdFile Arquivo) {
  if (Arquivo.open("umidade.bin",O_RDWR | O_CREAT | O_TRUNC)) {
    Noh* auxiliar = Inicio;
    while(auxiliar != NULL) {
      Arquivo.write((char *)&(auxiliar->mUmidade),sizeof(int));
      auxiliar = auxiliar->prox;
    }
      Arquivo.close();
  }
}

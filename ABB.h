/*
 * TRABALHO FINAL DE ESTRUTURA DE DADOS
 * ARQUIVO REFERENTE A ESTRUTURA ARVORE DE BUSCA BINARIA
 * GIOVANE AGUIAR DE ALMEIDA, PAULO HENRIQUE DE OLIVEIRA, RENAN GUIMARÃES
 */
#include "Arduino.h"
#include "SdFat.h"
class Noh{
    friend class ABB;
    public:
    Noh(float _valor){
        pai = NULL;
        esquerda = NULL;
        direita = NULL;
        valor = _valor;
    }
    ~Noh(){
        delete esquerda;
        delete direita;
    }
    private:
    Noh* pai;
    Noh* esquerda;
    Noh* direita;
    float valor;
};

class ABB{
    public:
    ABB();
    ~ABB();
    void insere(float _valor);
    float Minimo();
    float Maior();
    void GravaEmOrdem();
    void Remove(int _valor);
    bool Busca(int _valor);
    Noh* NohBuscado(int _valor);
    void Transplanta(Noh* _noh1, Noh* _noh2);
    Noh* GetSucessor(Noh* _pai);
    private:
    Noh* raiz;
    Noh* MinimoAux(Noh* raizSub);
    Noh* MaximoAux(Noh* raizSUb);
    void GravaEmOrdemAux(Noh* atual,SdFile* Arquivo);
};

ABB::ABB(){
    raiz = NULL;
}

ABB::~ABB(){
    delete raiz;
}

void ABB::insere(float _valor){
    Noh* novoNoh = new Noh(_valor);
    bool repetido = false;
    if(raiz == NULL){
        raiz = novoNoh;
    } else {
        Noh* atual = raiz;
        Noh* anterior = NULL;
        while(atual != NULL){
            anterior = atual;
            if(atual->valor == _valor)
            {
              atual=NULL;
              repetido = true;
            }             
            else if(atual->valor > novoNoh->valor){
                atual = atual->esquerda;
            } else {
                atual = atual->direita;
            }
        }
        if(repetido==false)
        {
          novoNoh->pai = anterior;
          if(anterior->valor > novoNoh->valor){
              anterior->esquerda = novoNoh;
          } else {
              anterior->direita = novoNoh;
          }
       }
    }
}

Noh* ABB::MinimoAux(Noh* raizSub){
    while(raizSub->esquerda != NULL){
        raizSub = raizSub->esquerda;
    }

    return raizSub;
}

float ABB::Minimo(){
    if(raiz != NULL){
        Noh* minimo = MinimoAux(raiz);
        return minimo->valor;
    }
    return -1;
}

Noh* ABB::MaximoAux(Noh* raizSub)
{
  while(raizSub->direita != NULL)
  {
    raizSub = raizSub->direita;
  }
  return raizSub;
}

float ABB::Maior()
{
  if(raiz !=NULL)
  {
    Noh* maximo = MaximoAux(raiz);
    return maximo->valor;
  }
  return -1;
}


void ABB::Transplanta(Noh* _noh1, Noh* _noh2) {
    if (_noh1 == raiz) {
        raiz = _noh2;
    } else if (_noh1->pai->esquerda == _noh1) {
        _noh1->pai->esquerda = _noh2;
    } else {
        _noh1->pai->direita = _noh2;
    }

    if (_noh2 != NULL) {
        _noh2->pai = _noh1->pai;
    }
}

void ABB::Remove(int _valor){
    Noh* nohADeletar = NohBuscado(_valor);

    if (nohADeletar == NULL) {
        return -1;
    } else {
        int dadoRemovido = nohADeletar->valor;
        Noh* aSerOrganizado = NULL;

        if (nohADeletar->esquerda == NULL) {
            aSerOrganizado = nohADeletar->direita;
            Transplanta(nohADeletar, nohADeletar->direita);
        } else if (nohADeletar->direita == NULL) {
            aSerOrganizado = nohADeletar->esquerda;
            Transplanta(nohADeletar, nohADeletar->esquerda);
        } else { // noh tem dois filhos
            Noh* sucessor = GetSucessor(nohADeletar);
            aSerOrganizado = sucessor;

            if (sucessor->pai != nohADeletar) {
                Transplanta(sucessor, sucessor->direita);
                sucessor->direita = nohADeletar->direita;
                nohADeletar->direita->pai = sucessor;
            }
            Transplanta(nohADeletar, sucessor);
            sucessor->esquerda = nohADeletar->esquerda;
            nohADeletar->esquerda->pai = sucessor;
        }
        nohADeletar->esquerda = NULL;
        nohADeletar->direita = NULL;
        delete nohADeletar;
        return dadoRemovido;
    }
}

Noh* ABB::NohBuscado(int _valor) {
    Noh* percorredor = raiz;

    while (percorredor != NULL) {
        if (percorredor->valor == _valor) {
            return percorredor;
        } else if (_valor < percorredor->valor) {
            percorredor = percorredor->esquerda;
        } else {
            percorredor = percorredor->direita;
        }
    }
    return NULL;
}

Noh* ABB::GetSucessor(Noh* _pai) {
    if (_pai->direita == NULL){
        return NULL;
    } else {
        Noh* percorre = _pai->direita;
        while (percorre->esquerda != NULL){
            percorre = percorre->esquerda;
        }
        return percorre;
    }
}

bool ABB::Busca(int _valor) {

    if (NohBuscado(_valor) == NULL) {
        return false;
    }
    return true;
}

void ABB::GravaEmOrdem(){
    SdFile Arquivo;
    if(Arquivo.open("umidade.bin",O_WRONLY | O_CREAT | O_TRUNC))
    {
      GravaEmOrdemAux(raiz,&Arquivo);
      Arquivo.close();
    }    
}

void ABB::GravaEmOrdemAux(Noh* atual, SdFile* ArquivoX){
    int aux;
    if(atual != NULL)
    {
      GravaEmOrdemAux(atual->esquerda,ArquivoX);
      aux = atual->valor;
      ArquivoX->write((char*)&aux,sizeof(int));
      GravaEmOrdemAux(atual->direita,ArquivoX);
    }
}


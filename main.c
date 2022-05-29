#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
//#include "biblioteca.h"


/*
Documentação do jogo

Regras de utilização de nomenclatura da main.

Variáveis:
char palavra: é a variável que vai armazenar a palavra sorteada.
int rodada: indica quantas tentativas a pessoa ainda tem ao final da tentativa corrente

Arquivos:
lista.txt: É o arquivo com a lista de palavras para sorteio - UTILIZADO O PONTEIRO LISTA
listab.txt: É o arquivo com as palavras bloqueadas.


Funções:

sorteia_pal: Sorteia uma palavra da lista.
pega_pal: Ele scaneia o STDIN para pegar a palavra que o usuário digitou e armazenará no char palavra.
checa_pal: Checa se a palavra da lista não está na lista bloqueada.
testa_pal: Testa a correspondência da palavra sorteada com a digitada.
*/

void sorteia_pal();
void pega_pal();
void checa_pal();
void testa_pal();
void salva_pal();

char palavra[5], tentativa[5];
//Palavra sorteada é definida globalmente para permitir a manipulação por todas as funções sem necessitar de abstrações mais pesadas.

int main (){
    
    setlocale(LC_ALL, "Portuguese");

    FILE  *sorteadas;

    int rodada = 5, palavra_valida = 0;

    while (rodada >= 0){

        //printf("Bem vindo ao jogo das palavras, nesse jogo você deve acertar a palavra de 5 letras escolhida pelo programa");
        sorteia_pal();
        pega_pal();
        checa_pal(&palavra_valida);


        if (palavra_valida == 1){
            testa_pal();
            printf("A palavra é válida");
            return 0;
        }


        rodada--;
    }

    

}

/*
Aqui começa a manipulação das funções do arquivo
*/

void sorteia_pal(){

    int linha;
    // Vai sortear um número e ir direto para a palavra daquela linha usando fseek()

    FILE *lista;

    lista = fopen("lista.txt", "r");

    srand(time(NULL));

    linha = rand() % 14;


    for (int i = 0; i <= linha; i++){
        fscanf(lista, "%s", palavra);
    }

    // Teste para verificar se o sorteio ta funfando printf("\nA linha selecionada foi %d e a palavra foi %s", linha, palavra);

    fclose(lista);
}

void pega_pal(){
    printf("Digite a sua tentativa\n");

    scanf("%s", tentativa);
}

void checa_pal(int *validador){

FILE *listab;
char compara[5];

listab = fopen("listab.txt", "r");

do{
    fscanf(listab, "%s", compara);

    if (compara == palavra){
        printf("Você digitou uma palavra que não existe! Digite outra");
    }
    else
    *validador = 1;
    //Permite o jogo continuar.
    
}while(feof(listab)== 0);

fclose(listab);

}

void testa_pal(){

}
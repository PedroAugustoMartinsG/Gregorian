#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <ctype.h>


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
void salva_jogo();

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
            //printf("A palavra é válida");
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

    if (lista == NULL){
        printf("Arquivos essenciais do jogo não foram encontrados");
        
    }

    srand(time(NULL));

    linha = rand() % 14;


    for (int i = 0; i <= linha; i++){
        fscanf(lista, "%s", palavra);
    }

    // Teste para verificar se o sorteio ta funfando printf("\nA linha selecionada foi %d e a palavra foi %s", linha, palavra);

    fclose(lista);
}

void pega_pal(){

    int pega_digito;
    const char proibido[] = "012345789/*-+,!@#$&*()_?~}]{[";

    do{
        printf("Digite a sua tentativa\n");
        pega_digito = 0;

        scanf("%s", tentativa);


         
        if (strpbrk(proibido,tentativa) != 0){
            pega_digito = 1;
        }



    }while(strlen(tentativa) != 5 || pega_digito == 1);
    //Parte do código que testa se a tentativa tem 5 caracteres ou se tem número, seria interessante refatorar para deixar claro o erro. Função protótipo.

    


    
}

void checa_pal(int *validador){

FILE *listab;
char compara[5];

listab = fopen("listab.txt", "r");

 if (listab == NULL){
        printf("Arquivos essenciais do jogo não foram encontrados.");
    }

do{
    fscanf(listab, "%s", compara);

    printf("Comparando %s com %s", tentativa, compara);

    if (strcmp(tentativa, palavra)== 1){
        printf("\nVocê digitou uma palavra que não existe! Digite outra");
    }
    else
    *validador = 1;
    //Permite o jogo continuar.
    
}while(feof(listab)== 0);

fclose(listab);

}

void testa_pal(){

}
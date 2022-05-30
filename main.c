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
        salva_pal();
        printf("Tentativa passou pelas checagens.");
        return 0;

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
        //printf("palavra selecionada foi %s", palavra);
    }

    // Teste para verificar se o sorteio ta funfando printf("\nA linha selecionada foi %d e a palavra foi %s", linha, palavra);

    fclose(lista);
}

void pega_pal(){
    
    int pega_erro;
    const char proibido[] = "012345789/*-+,!@#$&*()_?~}]{[";

    do{
        printf("Digite a sua tentativa válida\n");
        pega_erro = 0;

        scanf("%s", tentativa);
        checa_pal(&pega_erro);

         
        if (strpbrk(proibido,tentativa) != 0 || strlen(tentativa) != 5){
            pega_erro = 1;
            printf("Detecada uma palavra inválida, tente novamente.\n");
        }

    }while(pega_erro == 1);
    //Parte do código que testa se a tentativa tem 5 caracteres ou se tem número, seria interessante refatorar para deixar claro o erro. Função protótipo.
    
}

void checa_pal(int *passa_erro){

FILE *listab;
char compara[5];

listab = fopen("listab.txt", "r");

 if (listab == NULL){
        printf("Arquivos essenciais do jogo não foram encontrados.");
    }

do{
    fscanf(listab, "%s", compara);

    //printf("Comparando %s com %s\n", tentativa, compara);

    if (strcmp(tentativa,compara)== 0){
        printf("Detecada uma palavra inválida, tente novamente.\n");
        *passa_erro = 1;
    }
    else
    *passa_erro = 0;
    //Permite o jogo continuar.
    
}while(feof(listab)== 0);

fclose(listab);

}

void testa_pal(){

}

void salva_pal(){
    FILE *arquivo;

    arquivo = fopen("salvos.txt", "a");

    fseek(arquivo, 0,SEEK_END);

    fprintf(arquivo,"%s", palavra);

    // Aqui precisamos determinar quantas palavras repetidas nós vamos salvar.

}
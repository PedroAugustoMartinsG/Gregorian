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
char palavra: é a variável que vai armazenar a palavra sorteada;
int rodada: indica quantas tentativas a pessoa ainda tem ao final da tentativa corrente.

Arquivos:
lista.txt: É o arquivo com a lista de palavras para sorteio - UTILIZADO O PONTEIRO LISTA;
listab.txt: É o arquivo com as palavras bloqueadas;
salvos.txt: É o arquivo com as palavras sorteadas;
backup.txt: É o arquivo com o histórico do jogo.


Funções:

sorteia_pal: Sorteia uma palavra da lista;
pega_pal: Ele scaneia o STDIN para pegar a palavra que o usuário digitou e armazenará no char palavra;
checa_pal: Checa se a tentativa do usuário é válida dentro dos parâmetros do jogo;
testa_pal: Testa a correspondência da palavra sorteada com a digitada, e armazena os acertos e erros no backup.txt;
salva_pal: Salva palavra sorteada em uma lista de palavras para futura checagem na sorteia_pal;
salva_jogo: Salva o estado do jogo em um arquivo externo;
reiniciar_jogo: Reinicia as variáveis e arquivos externos.
*/

void checa_pal();
void pega_pal();
void testa_pal();
void reiniciar_jogo();
void salvaChar_jogo();
void salvaInt_jogo();
void salva_pal();
void sorteia_pal();

char palavra[6], tentativa[6], letras[6]; // Tentativa precisa de um espaço a mais da quantidade de letras
int acertos;                              // muitas funções a usam, interessante ficar global. Acertos por rodada!

// Palavra sorteada e acertos são definidos globalmente para permitir a manipulação por todas as funções sem necessitar de abstrações mais pesadas.
// Palavra: palavra sorteada pelo jogo.
// Tentativa: tentativa do usuário de acertar a palavra.
// Letras: letras que o usuário acertou da palavra.
// Acertos: Quantidade de acertos.

int main()
{
    setlocale(LC_ALL, "Portuguese");
    int rodada = 0, status = 1;
    FILE *file = fopen("salvos.txt", "w"); // Para limpar o conteúdo de backup.txt
    fclose(file);
    while (status == 1)
    {
        reiniciar_jogo(&rodada); // Zerar variaveis rodada e acertos
        sorteia_pal();
        salva_pal();
        while (rodada < 5)
        {
            printf("Rodada: %d/5\n", rodada + 1);
            salvaInt_jogo(rodada);
            pega_pal();
            testa_pal();
            printf("Acertos: %d/5\n", acertos);
            if (acertos == 5 && strcmp(tentativa, palavra) == 0) // Tem que verificar se não são anagramas
            {
                // Podia aparecer uma pop up lindona dando parabéns por ter ganhado
                break;
            }
            rodada++;
        }
        // Fim do game
        printf("Digite 1 se deseja jogar novamente...");
        scanf("%i", &status);
    }
    return 0;
}

void reiniciar_jogo(int *v1) // Vai zerar as variáveis acertos, letras e rodada
{
    // Reiniciar variáveis
    acertos = 0;
    for (int i = 0; i < 5; i++) // Seria bom refatorar esta parta
    {
        letras[i] = ' ';
    }
    letras[5] = '\0';
    *v1 = 0; // Zerar a variavel rodada
}

void sorteia_pal() // Vai sortear uma palavra aleatória da lista
{
    int linha, flag = 0;
    char compara[6];
    FILE *lista;
    FILE *lista_salvos;
    if (lista == NULL)
    {
        printf("Arquivos essenciais do jogo não foram encontrados");
    }
    while (flag == 0)
    {
        lista = fopen("lista.txt", "r");
        lista_salvos = fopen("salvos.txt", "a+");
        flag = 1;
        srand(time(NULL));
        linha = rand() % 14;
        for (int i = 0; i <= linha; i++)
        {
            fscanf(lista, "%s", palavra);
            if (i == linha) // para aparecer somenta a resposta certa
                printf("\npalavra selecionada foi %s\n", palavra);
        }
        while (feof(lista_salvos) == 0)
        {
            fscanf(lista_salvos, "%s", compara);
            if (strcmp(palavra, compara) == 0)
            {
                printf("Detectada uma palavra repetida.\n");
                flag = 0;
                break;
            }
        }
        fclose(lista);
        fclose(lista_salvos);
    }
    // Teste para verificar se o sorteio ta funfando printf("\nA linha selecionada foi %d e a palavra foi %s", linha, palavra);
}

void salva_pal() // Vai salvar as palavras-respostas num arquivo
{
    // Salvando a palavra em um arquivo
    FILE *arq;
    FILE *arquivo;
    FILE *arq_backup;
    char teste[30][5];
    int quantidade = 0;
    arq = fopen("salvos.txt", "r");
    arquivo = fopen("salvos.txt", "a+");
    arq_backup = fopen("backup.txt", "w");
    // Apagando o arquivo quando apresenta 30 palavras
    if (arq)
    {
        for (int a = 0; a < 30; a++)
        {
            fscanf(arq, "%s", teste[a]);
            quantidade = quantidade + strlen(teste[a]);
        }
        fclose(arq);

        if (quantidade >= 150)
            arq = fopen("salvos.txt", "w");
        fclose(arq);
    }
    else
        printf("erro ao abrir o arquivo\n");
    // printf("\n\n\ quantidade %d\n",quantidade);
    if (arquivo)
    {
        fprintf(arquivo, "%s\n", palavra);
        fclose(arquivo);
    }
    else
        printf("erro 2 ao abrir o arquivo\n");
    // Colocando a palavra-resposta na primeira linha do backup
    if (arq_backup)
    {
        fprintf(arq_backup, "%s\n", palavra);
        fclose(arq_backup);
    }
    else
        printf("erro 3 ao abrir o arquivo\n");
}

void salvaInt_jogo(int termo) // Vai passar o int a ser salvo
{
    FILE *arquivo;
    arquivo = fopen("backup.txt", "a");
    if (arquivo == NULL)
    {
        printf("Erro na abertura do arquivo!");
    }
    fprintf(arquivo, "%i\n", termo);
    fclose(arquivo);
}

void salvaChar_jogo(char *termo) // Vai passar o vetor de char a ser salvo
{
    FILE *arquivo;
    arquivo = fopen("backup.txt", "a");
    if (arquivo == NULL)
    {
        printf("Erro na abertura do arquivo!");
    }
    for (int i = 0; i < 5; i++)
    {
        if (termo[i] != ' ') // Para nao imprimir espaços vazios
            fprintf(arquivo, "%c\n", termo[i]);
    }
    fclose(arquivo);
}

void pega_pal() // Vai pegar a tentativa do usuário e, verificar se ela está válida
{
    int pega_erro;
    const char proibido[] = "012345789/-+,!@#$&()_?~}]{[";
    do
    {
        printf("\nDigite a sua tentativa válida\n");
        pega_erro = 0;
        scanf("%s", tentativa);
        checa_pal(&pega_erro);
        if (strpbrk(proibido, tentativa) != 0 || strlen(tentativa) != 5 || pega_erro == 1) // Deve, antes de entrar na chega_pal, verificar se tentativa tem 5 letras
        {
            pega_erro = 1;
            printf("Detectada uma palavra inválida, tente novamente.\n");
        }
    } while (pega_erro == 1);
    // Parte do código que testa se a tentativa tem 5 caracteres ou se tem número, seria interessante refatorar para deixar claro o erro. Função protótipo.
}

void checa_pal(int *passa_erro) // Vai checar se o palpite está de acordo com as especificações da mecânica de jogo
{
    // Só funciona se, e somente se, o palpite tiver somente 5 letras
    FILE *listab;
    char compara[6];
    listab = fopen("listab.txt", "r");
    if (listab == NULL)
    {
        printf("Arquivos essenciais do jogo não foram encontrados.");
    }
    while (feof(listab) == 0)
    {
        fscanf(listab, "%s", compara);
        // printf("Comparando %s com %s\n", tentativa, compara);
        if (strcmp(tentativa, compara) == 0)
        {
            printf("Detectada uma palavra inválida, tente novamente.\n");
            *passa_erro = 1;
            break;
        }
        else
            *passa_erro = 0;
        // Permite o jogo continuar.
    }
    fclose(listab);
}

void testa_pal() // Vai verificar a quantidade de acertos do palpite
{
    char certos[6] = {' ', ' ', ' ', ' ', ' ', '\0'}; // Inicializar os possíveis acertos da rodada (importante para ter índices futuros)
    //char erros[6] = {' ', ' ', ' ', ' ', ' ', '\0'};  // Inicializar os possíveis erros da rodada (importante para ter índices futuros)
    int cont;
    // Fazer verificação letra por letra
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (tentativa[i] == palavra[j])
            {
                // Verificar se a letra já foi computada
                cont = 0;
                for (int k = 0; k < 5; k++)
                {
                    if (letras[k] == tentativa[i]) // Letra já computada
                    {
                        cont++;
                        break;
                    }
                }
                if (cont == 0)
                {
                    letras[i] = tentativa[i];
                    acertos++;
                    printf("\nVocê acertou a letra %c da palavra %s", tentativa[i], palavra); // Aqui podia colocar uma corzinha marota
                }
                certos[i] = tentativa[i];
            }
        }
    }
    // Pode ser interresante, no futuro, calcular as letras erradas
    printf("\nCertos: %s \n", certos);
    printf("Letras: %s \n", letras);
    // Vai salvar os erros da rodada no arquivo backup
    // Vai salvar os acertos da rodada no arquivo backup
    salvaInt_jogo(acertos); // Quantidade de acertos
    salvaChar_jogo(certos); // Letras acertadas
}

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
lista.txt: É o arquivo com a lista de palavras para sorteio ?? utilizado o ponteiro lista;
listab.txt: É o arquivo com as palavras bloqueadas;
salvos.txt: É o arquivo com as palavras sorteadas;
backup.txt: É o arquivo com o histórico do jogo.

Funções:
checa_pal: Checa se a tentativa do usuário é válida dentro dos parâmetros do jogo;
pega_pal: Escanea o STDIN para pegar a palavra que o usuário digitou e armazenará no char palavra;
reiniciar_jogo: Reinicia as variáveis e arquivos externos;
salvaChar_jogo: Salva o estado do jogo em um arquivo externo no tipo char;
salvaInt_jogo: Salva o estado do jogo em um arquivo no tipo int;
salva_pal: Salva palavra sorteada em uma lista de palavras para futura checagem na sorteia_pal;
sorteia_pal: Sorteia uma palavra da lista;
testa_pal: Testa a correspondência da palavra sorteada com a digitada, e armazena os acertos e erros no backup.txt.
*/

void checa_pal();
void pega_pal();
void reiniciar_jogo();
void salvaChar_jogo();
void salvaInt_jogo();
void salva_pal();
void sorteia_pal();
void testa_pal();

char palavra[6], tentativa[6], letras[6]; // Tentativa precisa de um espaço a mais da quantidade de letras
int acertos;                              // Muitas funções a usam, interessante ficar global. Acertos por rodada!

// Palavra sorteada e acertos são definidos globalmente para permitir a manipulação por todas as funções sem necessitar de abstrações mais pesadas.
// Palavra: palavra sorteada pelo jogo.
// Tentativa: tentativa do usuário de acertar a palavra.
// Letras: letras que o usuário acertou da palavra.
// Acertos: Quantidade de acertos.

int main()
{
    setlocale(LC_ALL, "Portuguese");
    int rodada = 0, status = 1;
    FILE *file = fopen("salvos.txt", "w"); // Limpar o conteúdo de salvos.txt
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
            printf("Acertos totais: %d\n", acertos);
            if (strcmp(tentativa, palavra) == 0) // Verificar se não são anagramas
            {
                // Podia aparecer uma pop up lindona dando parabéns por ter ganhado
                break;
            }
            rodada++;
        }
        // Fim da rotina
        printf("Digite 1 se deseja jogar novamente...");
        scanf("%i", &status);
    }
    return 0;
}

void reiniciar_jogo(int *v1) // Vai zerar as variáveis acertos, letras e rodada
{
    // Reiniciar variáveis
    acertos = 0;
    *v1 = 0;                    // Zerar a variavel rodada
    for (int i = 0; i < 5; i++) // Seria bom refatorar esta parte
    {
        letras[i] = ' ';
    }
    letras[5] = '\0';
}

void sorteia_pal() // Vai sortear uma palavra aleatória da lista
{
    int linha, flag = 0;
    char compara[6];
    FILE *lista;
    FILE *lista_salvos;
    while (flag == 0)
    {
        lista = fopen("lista.txt", "r");
        lista_salvos = fopen("salvos.txt", "a+");
        if (lista == NULL || lista_salvos == NULL)
        {
            printf("Arquivos essenciais do jogo não foram encontrados");
            exit(1);
        }
        flag = 1;
        srand(time(NULL));
        linha = rand() % 14;
        for (int i = 0; i <= linha; i++)
        {
            fscanf(lista, "%s", palavra);
        }
        while (feof(lista_salvos) == 0)
        {
            fscanf(lista_salvos, "%s", compara);
            if (strcmp(palavra, compara) == 0)
            {
                // printf("Detectada uma palavra repetida.\n");
                flag = 0;
                break;
            }
        }
        fclose(lista);
        fclose(lista_salvos);
    }
    printf("\nPalavra selecionada foi: %s\n", palavra);
    //  Teste para verificar se o sorteio ta funfando printf("\nA linha selecionada foi %d e a palavra foi %s", linha, palavra);
}

void salva_pal() // Vai salvar as palavras-respostas num arquivo
{
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
        for (int a = 0; feof(arq) == 0; a++)
        {
            fscanf(arq, "%s", teste[a]);
            quantidade = quantidade + strlen(teste[a]);
        }
        fclose(arq);
        if (quantidade >= 50) // 10 palavras
        {
            arq = fopen("salvos.txt", "w");
            fclose(arq);
        }
    }
    else
    {
        printf("Erro ao abrir o arquivo salvos.txt\n");
        exit(1);
    }
    // printf("\n\n\ quantidade %d\n",quantidade);
    if (arquivo)
    {
        fprintf(arquivo, "%s\n", palavra);
        fclose(arquivo);
    }
    else
    {
        printf("Erro ao abrir o arquivo salvos.txt\n");
        exit(1);
    }
    // Colocando a palavra-resposta na primeira linha do backup.txt
    if (arq_backup)
    {
        fprintf(arq_backup, "%s\n", palavra);
        fclose(arq_backup);
    }
    else
    {
        printf("Erro ao abrir o arquivo backup.txt\n");
        exit(1);
    }
}

void salvaInt_jogo(int termo) // Vai passar o int a ser salvo
{
    FILE *arquivo;
    arquivo = fopen("backup.txt", "a");
    if (arquivo == NULL)
    {
        printf("Erro na abertura do arquivo!");
        exit(1);
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
        exit(1);
    }
    for (int i = 0; i < 5; i++)
    {
        if (termo[i] != ' ') // Para não imprimir espaços vazios
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
        pega_erro = 0;
        printf("\nDigite a sua tentativa válida\n");
        scanf("%s", tentativa);
        checa_pal(&pega_erro);
        if (strpbrk(proibido, tentativa) != 0 || strlen(tentativa) != 5 || pega_erro == 1) // Condição de erros
        {
            pega_erro = 1;
            printf("Detectada uma palavra inválida, tente novamente.\n");
        }
    } while (pega_erro == 1);
    // Parte do código que testa se a tentativa tem 5 caracteres ou se tem número, seria interessante refatorar para deixar claro o erro. Função protótipo.
}

void checa_pal(int *passa_erro) // Vai checar se o palpite está de acordo com as especificações da mecânica de jogo
{
    FILE *listab;
    char compara[6], tentativaUpperCase[6];
    listab = fopen("listab.txt", "r");
    if (listab == NULL)
    {
        printf("Arquivos essenciais do jogo não foram encontrados.");
        exit(1);
    }
    // Converter em letras maiúsculas
    int i = 0;
    while (tentativa[i] != '\0')
    {
        tentativaUpperCase[i] = toupper(tentativa[i]);
        i++;
    }
    tentativaUpperCase[i] = '\0'; // Indicar o fim da string
    while (feof(listab) == 0)
    {
        fscanf(listab, "%s", compara);
        //printf("Comparando %s com %s\n", tentativaUpperCase, compara); // Comparar em letras maiúsculas
        if (strcmp(tentativaUpperCase, compara) == 0)
        {
            //printf("Detectada uma palavra inválida, tente novamente.\n");
            *passa_erro = 1;
            break;
        }
        else
            *passa_erro = 0;
        // Permite o jogo continuar.
    }
    fclose(listab);
}

void testa_pal() // Vai verificar a quantidade de acertos do palpites
{
    char certos[6] = {' ', ' ', ' ', ' ', ' ', '\0'}; // Inicializar os possíveis acertos da rodada (importante para ter índices futuros)
    // char erros[6] = {' ', ' ', ' ', ' ', ' ', '\0'};  // Inicializar os possíveis erros da rodada (importante para ter índices futuros)
    int cont, cont2, acertosRodada = 0;
    // Fazer verificação letra por letra
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (tentativa[i] == palavra[j])
            {
                // Verificar se a letra já foi computada
                cont = 0;
                cont2 = 0;
                for (int k = 0; k < 5; k++)
                {
                    if (letras[k] == tentativa[i]) // Letra já computada
                        cont++;
                    if (certos[k] == tentativa[i]) // Letra já computada
                        cont2++;
                }
                if (cont == 0)
                {
                    for (int k = 0; k < 5; k++) // Salvar em ordem de acerto
                    {
                        if (letras[k] == ' ')
                        {
                            letras[k] = tentativa[i];
                            break;
                        }
                    }
                    acertos++;
                    // printf("\nVocê acertou a letra %c da palavra %s!", tentativa[i], palavra); // Aqui podia colocar uma corzinha marota
                    printf("\nVocê acertou a letra %c", tentativa[i]);
                }
                if (cont2 == 0)
                {
                    acertosRodada++; // Se, na tentativa, houver letras repetidas, acertos de palavras repetidas só computará uma vez
                    certos[i] = tentativa[i];
                }
            }
        }
    }
    // Pode ser interresante, no futuro, calcular as letras erradas
    printf("\nLetras acertadas na rodada: %s \n", certos);
    printf("Letras acertadas no jogo: %s \n", letras);
    printf("Acertos rodada: %d\n", acertosRodada);
    // Vai salvar os acertos (e a sua quantidade) da rodada no arquivo backup.txt
    salvaInt_jogo(acertosRodada); // Quantidade de acertos da rodada
    salvaChar_jogo(certos);       // Letras acertadas
}

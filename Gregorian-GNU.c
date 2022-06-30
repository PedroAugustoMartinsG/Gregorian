#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <ctype.h>

/*
DOCUMENTAÇÃO DO JOGO

Variáveis globais:
Dicas: dez palavras semelhantes à palavra-resposta usadas, opcionalmente pelo usuário, como dicas;
Letras: letras que o usuário acertou da palavra;
Palavra: palavra sorteada pelo jogo;
Tentativa: tentativa do usuário de acertar a palavra;
Acertos: Quantidade de acertos de letras não repetidas.

Arquivos:
lista.txt: É o arquivo com a lista de palavras para sorteio ?? utilizado o ponteiro lista;
listab.txt: É o arquivo com as palavras bloqueadas (para manter o jogo family-friendly);
listac.txt: É arquivo com as palavras similares à palavra-resposta (para otimizar verificações);
salvos.txt: É o arquivo com todas as palavras sorteadas;
backup.txt: É o arquivo com o histórico do jogo;
backup2.txt: É o arquivo com parte do histórico do jogo.

Funções:
checa_pal: Checa se a tentativa do usuário é válida dentro dos parâmetros do jogo;
mostra_dicas: Mostra dez palavras similares à palavra-respota;
mostra_gameover: Mostra "GAME OVER" no console;
mostra_logo: Mostra o nome "Gregorian" no console;
mostra_menu: Mostra o menu principal com as opções: Novo Jogo, Carregar Jogo, Como Jogar e Sair;
mostra_parabens: Mostra "Palavra Correta" no console;
pega_pal: Escanea o STDIN para pegar a palavra que o usuário digitou e armazenará no char palavra;
reiniciar_jogo: Reinicia as variáveis e arquivos externos;
salvaChar_jogo: Salva o estado do jogo em um arquivo externo no tipo char;
salvaInt_jogo: Salva o estado do jogo em um arquivo externo no tipo int;
salva_dicas: Salva dez dicas na variável global dicas;
salva_pal: Salva todas as palavras sorteadas em uma lista de palavras para futura checagem;
salva_palSimi: Salva dez palavras similares à palavra-resposta;
sorteia_pal: Sorteia uma palavra da lista;
testa_pal: Testa a correspondência da palavra sorteada com a digitada, e armazena valores em arquivos externos.
*/

void checa_pal();
void mostra_dicas();
void mostra_gameover();
void mostra_logo();
void mostra_menu();
void mostra_parabens();
void pega_pal();
void recupera_jogo();
void reiniciar_jogo();
void salvaChar_jogo();
void salvaInt_jogo();
void salva_dicas();
void salva_pal();
void salva_palSimi();
void sorteia_pal();
void testa_pal();

char dicas[11][6], letras[6], palavra[6], tentativa[6]; // Toda variável do tipo char precisa de, pelo menos, um espaço a mais da quantidade de espaço
int acertos;											// Muitas funções a usam; interessante ficar global. Acertos por rodada!

int main() // Vai orientar na execução do programa
{
	system("clear");
	setlocale(LC_ALL, "Portuguese");
	int quest = 0, rodada = 0, status = 1;
	FILE *file = fopen("salvos.txt", "w"); // Limpar o conteúdo de salvos.txt
	fclose(file);
	mostra_menu(&quest, &rodada);
	while (status == 1)
	{
		if (quest != 2) // Se não há jogo a ser carregado
		{
			reiniciar_jogo(&rodada); // Zerar variáveis rodada e acertos
			sorteia_pal();
			salva_pal();
			salva_palSimi();
			system("clear");
			mostra_logo();
		}
		salva_dicas(&dicas);
		while (rodada < 5)
		{
			printf("\t\t--------------------------------Rodada: %d/5--------------------------------\n", rodada + 1);
			salvaInt_jogo(rodada, "backup.txt");
			pega_pal();
			testa_pal();
			if (strcmp(tentativa, palavra) == 0) // Verificar se não são anagramas
			{
				mostra_parabens();
				remove("backup2.txt");
				remove("backup3.txt");
				break;
			}
			rodada++;
			// Fim da rotina
		}
		if (rodada == 5)
		{
			mostra_gameover();
			remove("backup2.txt");
			remove("backup3.txt");
		}
		printf("\t\t\t\t\tDigite 1 se deseja jogar novamente...\n\t\t\t\t\t\t\t");
		scanf("%i", &status);
		quest = 0;
		rodada = 0;
	}
	return 0;
}

void mostra_menu(int *quest, int *rodada) // Vai mostrar o menu com as opções deo jogo
{
	int mmr = 1, d;
	FILE *file2;
	FILE *file3;
	while (mmr == 1)
	{
		{
			mostra_logo();
			printf("\n\n\t\t\t\t               (1)Novo Jogo\n");
			printf("\t\t\t\t               (2)Carregar Jogo\n");
			printf("\t\t\t\t               (3)Como Jogar?\n");
			printf("\t\t\t\t               (4)Sair \n");
			printf("\t\t\t\t\t\t");
		}
		scanf("%d", &d);
		switch (d)
		{
		case 1:
			mmr++;
			system("clear");
			break;
		case 2:
			file2 = fopen("backup2.txt", "r"); // Verificar se há jogo a ser carregado
			file3 = fopen("backup3.txt", "r"); // Verificar se há jogo a ser carregado
			if (file2 || file3)
			{
				reiniciar_jogo(rodada); // Zerar variáveis rodada e acertos
				recupera_jogo(rodada);
				mmr++;
				*quest = 2;
				fclose(file2);
				fclose(file3);
				system("clear");
				printf("\n\t\t\t\t\t Letras acertadas no jogo\n\t\t\t\t\t\t    ");
				for (int i = 0; i < 5; i++)
				{
					printf("%c  ", letras[i]);
				}
				printf("\n");
			}
			else
			{
				system("clear");
				printf("\n\t\t\t\t\t Não há jogo a ser carregado\n\t\t\t\t\t\t    ");
				printf("\n");
			}
			break;
		case 3:
			system("clear");
			mostra_logo();
			printf("\n\t\t           -A cada rodada você deve adivinhar a palavra da rodada\n");
			printf("\t\t          -Todas as palavras possuem 5 letras, e não são acentuadas\n");
			printf("\t\t  -Você tem 5 tentativas, a cada tentativa será mostrado as letras corretas\n");
			printf("\t\t    -Se estiver com problemas, pode digitar \"DICA\" para receber uma ajuda");
			printf("\n\t\t\t\t\t      Digite 1 para continuar\n\t\t\t\t\t\t\t");
			scanf("%d", &d);
			system("clear");
			break;
		case 4:
			exit(1);
			break;
		default:
			system("clear");
		}
	}
}

void mostra_logo() // Vai mostrar Gregorian no console
{
	printf("\t   .oooooo.                                                      o8o     \n");
	printf("\t  d8P'  `Y8b                                                     `\"'        \n");
	printf("\t 888           oooo d8b  .ooooo.   .oooooooo  .ooooo.  oooo d8b oooo   .oooo.   ooo. .oo.     \n");
	printf("\t 888           `888\"\"8P d88' `88b 888' `88b  d88' `88b `888\"\"8P `888  `P  )88b  `888P\"Y88b    \n");
	printf("\t 888     ooooo  888     888ooo888 888   888  888   888  888      888   .oP\"888   888   888    \n");
	printf("\t `88.    .88'   888     888    .o `88bod8P'  888   888  888      888  d8(  888   888   888    \n");
	printf("\t  `Y8bood8P'   d888b    `Y8bod8P' `8oooooo.  `Y8bod8P' d888b    o888o `Y888\"\"8o o888o o888o  \n");
	printf("\t                                  d\"     YD         \n");
	printf("\t                                  \"Y88888P'       \n");
}

void mostra_parabens() // Vai mostrar PALAVRA CORRETA no console
{
	system("clear");
	printf("\t ooooooooo.         .o.       ooooo              .o.       oooooo     oooo ooooooooo.         .o.       \n");
	printf("\t `888   `Y88.      .888.      `888'             .888.       `888.     .8'  `888   `Y88.      .888.      \n");
	printf("\t  888   .d88'     .8\"888.      888             .8\"888.       `888.   .8'    888   .d88'     .8\"888.     \n");
	printf("\t  888ooo88P'     .8' `888.     888            .8' `888.       `888. .8'     888ooo88P'     .8' `888.    \n");
	printf("\t  888           .88ooo8888.    888           .88ooo8888.       `888.8'      888`88b.      .88ooo8888.   \n");
	printf("\t  888          .8'     `888.   888       o  .8'     `888.       `888'       888  `88b.   .8'     `888.  \n");
	printf("\t o888o        o88o     o8888o o888ooooood8 o88o     o8888o       `8'       o888o  o888o o88o     o8888o \n");
	printf("\t\n\n\n");
	printf("\t     .oooooo.     .oooooo.   ooooooooo.   ooooooooo.   oooooooooooo ooooooooooooo       .o.       .o.     \n");
	printf("\t    d8P'  `Y8b   d8P'  `Y8b  `888   `Y88. `888   `Y88. `888'     `8 8'   888   `8      .888.      888     \n");
	printf("\t   888          888      888  888   .d88'  888   .d88'  888              888          .8\"888.     888     \n");
	printf("\t   888          888      888  888ooo88P'   888ooo88P'   888oooo8         888         .8' `888.    Y8P     \n");
	printf("\t   888          888      888  888`88b.     888`88b.     888    \"         888        .88ooo8888.   `8'     \n");
	printf("\t   `88b    ooo  `88b    d88'  888  `88b.   888  `88b.   888       o      888       .8'     `888.  .o.     \n");
	printf("\t    `Y8bood8P'   `Y8bood8P'  o888o  o888o o888o  o888o o888ooooood8     o888o     o88o     o8888o Y8P     \n\n");
}

void mostra_gameover() // Vai mostrar GAME OVER no console
{
	system("clear");
	printf("\t\t\t    .oooooo.          .o.       ooo        ooooo oooooooooooo   \n");
	printf("\t\t\t   d8P'  `Y8b        .888.      `88.       .888' `888'     `8   \n");
	printf("\t\t\t  888               .8\"888.      888b     d'888   888           \n");
	printf("\t\t\t  888              .8' `888.     8 Y88. .P  888   888oooo8      \n");
	printf("\t\t\t  888     ooooo   .88ooo8888.    8  `888'   888   888    \"      \n");
	printf("\t\t\t  `88.    .88'   .8'     `888.   8    Y     888   888       o   \n");
	printf("\t\t\t   `Y8bood8P'   o88o     o8888o o8o        o888o o888ooooood8   \n\n\n");
	printf("\t\t\t       .oooooo.   oooooo     oooo oooooooooooo ooooooooo.       \n");
	printf("\t\t\t      d8P'  `Y8b   `888.     .8'  `888'     `8 `888   `Y88.     \n");
	printf("\t\t\t     888      888   `888.   .8'    888          888   .d88'     \n");
	printf("\t\t\t     888      888    `888. .8'     888oooo8     888ooo88P'      \n");
	printf("\t\t\t     888      888     `888.8'      888    \"     888`88b.        \n");
	printf("\t\t\t     `88b    d88'      `888'       888       o  888  `88b.      \n");
	printf("\t\t\t      `Y8bood8P'        `8'       o888ooooood8 o888o  o888o     \n\n");
	printf("\n\t\t\t\t\t   A palavra correta era %s\n\t\t\t\t\t\t    ", palavra);
	printf("\n");
}

void reiniciar_jogo(int *v1) // Vai zerar as variáveis acertos, letras e rodada
{
	// Reiniciar variáveis
	acertos = 0;
	*v1 = 0; // Zerar a variavel rodada
	for (int i = 0; i < 5; i++)
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
		linha = rand() % 783; // 783 é a quantidade de linhas no arquivo lista.txt
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

void salva_palSimi() // Vai salvar as palavras que começam com a mesma letra da palavra-resposta no arquivo listac.txt
{
	int sort, i = 0;
	char comp[6], word;
	FILE *lista = fopen("lista.txt", "r");
	FILE *listac = fopen("listac.txt", "w");
	if (lista == NULL || listac == NULL)
	{
		printf("Arquivos essenciais do jogo não foram encontrados");
		exit(1);
	}
	// Excluir as letras que iniciam palavras de quantidade menor que 10
	word = palavra[0];
	if (word == 'K' || word == 'Q' || word == 'U')
		word++;
	// Pegar todas as palavras que iniciam com uma determinada letra
	fscanf(lista, "%s", comp);
	while (comp[0] != word) // Levar até a primeira palavra que tenha uma determinada letra
	{
		i++;
		fscanf(lista, "%s", comp);
	}
	while (comp[0] == word && feof(lista) == 0) // Da primeira palavra até a última palavra que tenha uma determinada letra
	{
		i++;
		fprintf(listac, "%s\n", comp);
		fscanf(lista, "%s", comp);
	}
	fclose(lista);
	fclose(listac);
}

void salva_dicas() // Vai salvar dez dicas na variável dicas
{
	int i = 0, sort;
	char word[6];
	FILE *lista = fopen("listac.txt", "r");
	srand(time(NULL));
	sort = rand() % 10; // 10 é a quantidade de dicas disponíveis
	while (i < 10)
	{
		fscanf(lista, "%s", word);
		if (strcmp(word, palavra) == 0)
			i--;
		else if (i == sort)
			strcpy(dicas[i], palavra);
		else
			strcpy(dicas[i], word);
		i++;
	}
	fclose(lista);
}

void salvaInt_jogo(int termo, char *arq) // Vai passar o int a ser salvo no arquivo
{
	FILE *arquivo;
	arquivo = fopen(arq, "a");
	if (arquivo == NULL)
	{
		printf("Erro na abertura do arquivo!");
		exit(1);
	}
	fprintf(arquivo, "%i\n", termo);
	fclose(arquivo);
}

void salvaChar_jogo(char *termo, char *arq) // Vai passar o vetor de char a ser salvo no arquivo
{
	FILE *arquivo;
	arquivo = fopen(arq, "a");
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
		printf("\t\t\t\t\t    Digite sua tentiva\n  ");
		printf("\t\t\t\tOu digite \"DICA\" para receber uma ajuda ;)\n\t\t\t\t\t\t  ");
		scanf("%s", tentativa);
		checa_pal(&pega_erro);
		if (pega_erro != -1)
		{
			if (strpbrk(proibido, tentativa) != 0 || strlen(tentativa) != 5 || pega_erro == 1) // Condição de erros
			{
				pega_erro = 1;
				printf("\t\t\t      Detectada uma palavra inválida, tente novamente.\n");
			}
		}
		else
			pega_erro = 1; // Voltar ao loop
	} while (pega_erro == 1);
}

void checa_pal(int *passa_erro) // Vai checar se o palpite está de acordo com as especificações da mecânica de jogo
{
	FILE *listab, *lista;
	char compara[6];
	listab = fopen("listab.txt", "r"); // Lista de palavra impróprias
	if (listab == NULL)
	{
		printf("Arquivos essenciais do jogo não foram encontrados.");
		exit(1);
	}
	// Converter em letras maiúsculas
	int i = 0;
	while (tentativa[i] != '\0')
	{
		tentativa[i] = toupper(tentativa[i]);
		i++;
	}
	tentativa[i] = '\0'; // Indicar o fim da string
	if (strcmp(tentativa, "DICA") == 0)
	{
		mostra_dicas();
		*passa_erro = -1;
	}
	else
	{
		while (feof(listab) == 0)
		{
			fscanf(listab, "%s", compara);
			if (strcmp(tentativa, compara) == 0)
			{
				*passa_erro = 1;
				break;
			}
			else
				*passa_erro = 0; // Permite o jogo continuar
		}
		fclose(listab);
		if (*passa_erro == 0)
		{
			lista = fopen("lista.txt", "r"); // Lista de palavras usuais com 5 letras
			if (lista == NULL)
			{
				printf("Arquivos essenciais do jogo não foram encontrados.");
				exit(1);
			}
			while (feof(lista) == 0)
			{
				fscanf(lista, "%s", compara);
				if (strcmp(tentativa, compara) == 0)
				{
					// Detectada uma palavra desconhecida (pelo sistema)
					*passa_erro = 0;
					break;
				}
				else
					*passa_erro = 1; // Permite o jogo continuar
			}
			fclose(listab);
		}
	}
}

void testa_pal() // Vai verificar a quantidade de acertos do palpites
{
	char certos[6] = {' ', ' ', ' ', ' ', ' ', '\0'}; // Inicializar os possíveis acertos da rodada
	int cont, cont2, acertosRodada = 0;
	printf("\t\t\t\t        Letras Acertadas da Rodada\n\t\t\t\t\t\t");
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
					if (letras[k] == tentativa[i]) // Letra já computada?
						cont++;
					if (certos[k] == tentativa[i]) // Letra já computada?
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
					printf("%c    ", tentativa[i]);
				}
				if (cont2 == 0)
				{
					acertosRodada++; // Se, na tentativa, houver letras repetidas, acertos de palavras repetidas só computará uma vez
					certos[i] = tentativa[i];
				}
			}
		}
	}
	printf("\n\t\t\t\t\t Letras acertadas no jogo\n\t\t\t\t\t\t    ");
	for (int i = 0; i < 5; i++)
	{
		printf("%c  ", letras[i]);
	}
	printf("\n");
	// Salvar (ou deletar) valores nos arquivos de backup
	salvaInt_jogo(acertosRodada, "backup.txt"); // Quantidade de acertos da rodada
	salvaChar_jogo(certos, "backup.txt");		// Letras acertadas
	remove("backup2.txt");
	remove("backup3.txt");
	salvaChar_jogo(letras, "backup2.txt"); // Letras acertadas
	salvaInt_jogo(acertos, "backup3.txt"); // Quantidade de acertos
}

void mostra_dicas() // Vai mostrar 10 dicas quando o usuário solicitar (a partir da segunda rodada)
{
	printf("\n\t\t\t    Dica: Possíveis respostas (uma delas é a correta):");
	for (int i = 0; i < 10; i++)
	{
		if (i % 2 == 0) // Matriz no formato 5x2
			printf("\n\t\t\t\t\t      ");
		printf("%s   ", dicas[i]);
	}
	printf("\n");
}

void recupera_jogo(int *rod) // Recupera a palavra-respota e o valor da última rodada do arquivo backup.txt
{
	// Recupera valores do backup.txt
	FILE *arq;
	arq = fopen("backup.txt", "r");
	if (arq)
	{
		fscanf(arq, "%s", palavra); // Palavra-resposta
		fseek(arq, -5, SEEK_END);
		fscanf(arq, "%d", rod); // Última rodada
		fclose(arq);
	}
	// Recupera valores do backup3.txt
	FILE *arq3;
	arq3 = fopen("backup3.txt", "r");
	if (arq3)
	{
		fscanf(arq3, "%d", &acertos);
		fclose(arq3);
	}
	// Recupera valores do backup2.txt
	FILE *arq2;
	arq2 = fopen("backup2.txt", "r");
	if (arq2)
	{
		for (int i = 0; feof(arq2) == 0 && i <= acertos; i++)
			fscanf(arq2, "%s", &letras[i]);
		fclose(arq2);
	}
}

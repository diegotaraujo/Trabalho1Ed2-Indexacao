/*--------------------------------------------------------------------------------------------------------------------------------------------
Universidade Federal de São Carlos, Campus Sorocaba
Bacharelado em Ciência da Computação
Estrutura de Dados 2
Prof. Tiago A. Almeida

TRABALHO 01 - INDEXAÇÃO
Setembro de 2015

Autores:
	Diego Tenorio de Araújo, 552143
	Gabriel Alves Bento, 587869
--------------------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INSERIR_PARTIDA 	1
#define REMOVER_PARTIDA 	2
#define BUSCAR_PARTIDA 		3
#define MODIFICAR_DURACAO 	4
#define LISTAR_PARTIDAS 	5
#define LIBERAR_ESPACO 		6
#define SAIR				7

#define ENTRADA_INVALIDA 		"Campo inválido! Informe novamente:\n"
#define PARTIDA_N_ENCONTRADA	"Partida não encontrada!\n"

//setar o tamanho das strings!<<<<<<<<<<<<<< um byte a mais que especificado no pdf para guardar o '\0' (6 bytes serão guardados como 5 no 
//arquivo quando excluirmos o '\0')

typedef struct partida {
	char equipe_azul[40];
	char equipe_vermelha[40];
	char equipe_vencedora[40];
	char apelido_mvp[40];
	char data[12]; 
	char duracao[6];
	char placar_azul[3];
	char placar_vermelha[3];
	char chave_primaria[9];
} Partida;

typedef struct {
	char primaria[9];
	long long int rrn;
} indexStruct;

//Prototipos das funções usadas
					//Estrutura de dados para armazenar os indices em RAM. ??????????????NÃO ENTENDI??????????
int verificarArquivoDados(FILE *); //Verificar se o arquivo de dados existe
int verificarIndicePrimario(FILE *); //Verificar se o indice primario existe
int verificarIndiceSecundario(FILE *, char *); //Verificar se os indices secundários existem
int criarIndicePrimario(FILE *); //Criar o indice primário
int criarIndiceSecundario(FILE *, char *); //Criar os indices secundários
int carregarIndices(); //Carregar os indices (DISCO PARA RAM)
Partida inserirPartida(); //Inserir registro
int buscarPartida(); //Buscar registro
int alterarPartida(); //Alterar registro
int removerPartida(); //Remover registro
void listarPartidas(); //Listar registros
void liberarEspaco(); //Liberar espaço
int atualizarIndices(); //Atualizar todos os indices
int ordenaIndicePrimario(indexStruct *, int , indexStruct);

int main () {
	/* Variaveis para arquivos, respectivamente: de dados, de indice primario, 
	de indice secundario para o nome da equipe vencedora e de indice secundario para apelido do MVP */
	FILE *data_file, *index_file, *winner_index_file, *mvp_index_file;
	int opcao;
	int existe;
	int tamanho, numero_sharps, qtdRRN=0, i, j, tamanhoPrimario=0;
	char c;
	Partida partida;
	indexStruct vetorPrimario[10000], novoIndice;

	/* Ao iniciar
		-Verificar se existe o arquivo de dados
			se existir: abrir o arquivo para escrita e leitura
			se não existir: criar o arquivo de dados no disco, abrindo para escrita e leitura
	*/

	existe = verificarArquivoDados(data_file);
	if(!existe)
		data_file = fopen("matches.dat", "w");
	else 
		data_file = fopen("matches.dat", "r+");
	
	/* -Verificar se existem os arquivos de indices
			Se existirem:
				Se estiverem consistentes: carregar os indices para RAM.
				Senão: refazer os indices e carrega-los para RAM.
			Se não existirem: criar os indices na RAM e grava-los no disco.
	*/		
	
	//Arquivo de índices primários
	existe = verificarIndicePrimario(index_file);
	if(!existe)
		index_file = fopen("iprimary.idx", "w");
	else {
		index_file = fopen("iprimary.idx", "r+");

		while(!EOF){
			i=0;
			
			for(j=0; j<9; j++)
				fscanf(index_file, "%c", vetorPrimario[i].primaria[j]); 
			
			fscanf(index_file, "%c",&c); //"desconsiderar @
			fscanf(index_file, "%d", vetorPrimario[i].rrn); //vetor[i].rrn = %dRRN
			fscanf(index_file, "%c",&c); //desconsiderar #
			i++;
			tamanhoPrimario++;
		}
	}

	do {
		scanf("%d", &opcao);
		getchar();

		switch(opcao) {
			case INSERIR_PARTIDA:

				partida = inserirPartida(); // unica funcao para leitura dos campos. Devolve uma struct partida
				
				/* Inserção da partida em data_file */
				fprintf(data_file, "%s@", partida.chave_primaria);
				fprintf(data_file, "%s@", partida.equipe_azul);
				fprintf(data_file, "%s@", partida.equipe_vermelha);
				fprintf(data_file, "%s@", partida.data);
				fprintf(data_file, "%s@", partida.duracao);
				fprintf(data_file, "%s@", partida.equipe_vencedora);
				fprintf(data_file, "%s@", partida.placar_azul);
				fprintf(data_file, "%s@", partida.placar_vermelha);
				fprintf(data_file, "%s@", partida.apelido_mvp);
				tamanho = 8 + strlen(partida.equipe_azul) + strlen(partida.equipe_vermelha) + 10 + 5 + strlen(partida.equipe_vencedora) + 2 + 2 + strlen(partida.apelido_mvp) + 9;
				numero_sharps = 192 - tamanho;
				while(numero_sharps  > 0){
					fprintf(data_file, "#", partida.duracao);
					numero_sharps--;
				}
				fflush(data_file);

				//Inserção ordenada em index_file (InsertionSort?)
					
				novoIndice.primaria = partida.chave_primaria;				
				novoIndice.rrn = rrn;
					
				if(!ordenaIndicePrimario(vetorPrimario, tamanhoPrimario, novoIndice))
					printf("Vetor de indices primarios cheio!\n");

				qtdRRN+=192;
				tamanhoPrimario++;

				//Inserção da partida em winner_index_file
				//Inserção da partida em mvp_index_file
				
			break;

			case BUSCAR_PARTIDA:
				//por código
				ler o codigo
				atribuir no novoIndice.primaria

				buscarPrimario no vetorPrimario
					return o rrn se achar
					return 0 se não encontrar				
				
				/*por nome da equipe vencedora
				por apelido do mvp*/
				ler o campo de mvp ou equipe vencedora
				atribuir no novo.IndiceSecundario // 2 vetores?
				
				buscarSecundário no vetorSecundario
					return 
				
			break;

			case REMOVER_PARTIDA:
			
				if(buscarPartida()) //buscarPrimario e buscarSecundario.
					//se encontrar devolve a posicao
					removerPartida();				
				else
					printf(PARTIDA_N_ENCONTRADA);
			break;

			case MODIFICAR_DURACAO:
			
				if(buscarPartida())
					//se encontrar devolve a posicao
					alterarPartida();
				
				else
					printf(PARTIDA_N_ENCONTRADA);
			break;

			case LISTAR_PARTIDAS:
				/*por código
				por nome da equipe vencedora
				por apelido do mvp*/
			break;

			case LIBERAR_ESPACO:
			break;

			case 7:
				return 0;
			break;

			default:
				printf(ENTRADA_INVALIDA);
			break;			
		}
	} while(opcao != 7);	//Segundo a descricao do problema, a opcao de finalizar eh a de numero 7


	return 0;
}

/*==================- IMPLEMENTAÇÃO DAS FUNÇÕES -==================*/

Partida inserirPartida(){
	
	Partida novo;
	int flag = 0;
	char equipeAzul[40], equipeVermelha[40], equipeVencedora[40], apelidoMvp[40], duracaoPartida[6], placarAzul[3], placarVermelha[3], chavePK[9];

	//Leitura das equipes
	scanf("%39[^\n]", novo.equipe_azul);
	getchar();

	scanf("%39[^\n]", novo.equipe_vermelha);
	getchar();
	
	//LE DATAS
	scanf("%[^\n]", novo.data);
	getchar();

	if(strlen(novo.data) == 11)
		printf("a");

	//Leitura da duracao
	flag = 0;
	do {
		scanf("%5[^\n]", novo.duracao);
		getchar();	//Tira \n do buffer
		flag = strlen(novo.duracao);
		if(flag != 5)
			printf("ROLA");
	} while (flag != 5);

	//Leitura da equipe vencedora
	do{
		flag = 0;
		scanf("%39[^\n]", novo.equipe_vencedora);
		getchar(); //Tira \n do buffer
		
		if(!strcmp(novo.equipe_azul, novo.equipe_vencedora) && !strcmp(novo.equipe_vermelha, novo.equipe_vencedora))
			flag = 1;
		if(flag)
			printf("PERU");

	}while(flag == 1);

	//Leitura dos placares
	flag = 0;
	do {
		scanf("%2[^\n]", novo.placar_azul);
		getchar();	//Tira \n do buffer
		flag = strlen(novo.placar_azul);
		if(flag != 2)
			printf("CARALHA DE ASA");
	} while(flag != 2);
	
	flag = 0;
	do {
		scanf("%2[^\n]", novo.placar_vermelha);
		getchar();	//Tira \n do buffer
		flag = strlen(novo.placar_vermelha);
		if(flag != 2)
			printf("PIROCA");
	} while (flag != 2);

	//Leitura do apelido
	scanf("%39[^\n]", novo.apelido_mvp);
	getchar();	//Tira \n do buffer
	
	novo.chave_primaria[0] = toupper(novo.equipe_azul[0]);
	novo.chave_primaria[1] = toupper(novo.equipe_vermelha[0]);
	novo.chave_primaria[2] = toupper(novo.apelido_mvp[0]);
	novo.chave_primaria[3] = toupper(novo.apelido_mvp[1]);

	novo.chave_primaria[4] = novo.data[0];
	novo.chave_primaria[5] = novo.data[1];
	novo.chave_primaria[6] = novo.data[3];
	novo.chave_primaria[7] = novo.data[4];

	novo.chave_primaria[8] = '\0';
	return novo;
}


/* Retorna 1 se o arquivo de dados existir */
int verificarArquivoDados(FILE *dataFile) {
	dataFile = fopen("matches.dat", "r");
	if(!dataFile)
		return 0;

	return 1;
}

int verificarIndicePrimario(FILE *primFile) {
	primFile = fopen("iprimary.idx", "r");
	if(!primFile)
		return 0;

	return 1;
}

int verificarIndiceSecundario(FILE *secFile, char *nome) {
	secFile = fopen(nome, "r");
	if(!secFile)
		return 0;

	return 1;
}

int criarIndicePrimario(FILE *primFile) {
	primFile = fopen("iprimary.idx", "w");
	if(!primFile) {
		perror("Ocorreu um erro ao abrir o arquivo de índices primário!\n");
		return 1;
	}

	return 0;
}

int criarIndiceSecundario(FILE *secFile, char *nome) {
	secFile = fopen(nome, "w");
	if(!secFile) {
		perror("Ocorreu um erro ao abrir o arquivo de índices secundário!\n");
		return 1;	
	}

	return 0;
}

int carregarIndices(char **indices) {

	return 0;
}

int buscarPartida() {
	return 0;
}

int alterarPartida() {
	return 0;
}

int removerPartida() {
	return 0;
}

void listarPartidas() {

}

void liberarEspaco() {

}

int atualizarIndices() {
	return 0;
}

int ordenaIndicePrimario(indexStruct *vetorPrimario, int tamanhoPrimario, indexStruct novoIndice){
	
	int i, j;	

	if(tamanhoPrimario == 10000)
		return 0;

	for(i=0; i < tamanhoPrimario; i++){
		if(strcmp(vetorPrimario[i].primaria,novoIndice.primaria) > 0){
			for(j=tamanhoPrimario-1; j>=i; j--){
				vetorPrimario[j+1] = vetorPrimario[j]; 
			}
			vetorPrimario[i] = novoIndice;	
			return 1;		
		}
	}
}

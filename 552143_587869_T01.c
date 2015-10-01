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
	char data[11]; 
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
int buscarPartida(indexStruct *, int , char *);

void le_equipe(char []);
void le_duracao(char []);
void le_vencedora(char [], char [], char []);
void le_placar(char []);
void le_apelido_mvp(char []);
void le_data(char[]);

int main () {
	/* Variaveis para arquivos, respectivamente: de dados, de indice primario, 
	de indice secundario para o nome da equipe vencedora e de indice secundario para apelido do MVP */
	FILE *data_file, *index_file, *winner_index_file, *mvp_index_file;
	int opcao;
	int existe;
	int tamanho, numero_sharps, qtdRRN=0, i, j, tamanhoPrimario=0, busca;
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
					
				strcmp(novoIndice.primaria,partida.chave_primaria);				
				novoIndice.rrn = qtdRRN;
					
				if(!ordenaIndicePrimario(vetorPrimario, tamanhoPrimario, novoIndice))
					printf("Vetor de indices primarios cheio!\n");

				qtdRRN+=192;
				tamanhoPrimario++;

				//Inserção da partida em winner_index_file
				//Inserção da partida em mvp_index_file
				
			break;

			case BUSCAR_PARTIDA:
								
				//por código
				//ler o codigo
				//atribuir no novoIndice.primaria

				//buscarPrimario no vetorPrimario
				busca =  buscarPartida(indexStruct *vetorPrimario, int tamanhoPrimario, char *chavePK);

				if(busca != -1)
					//busca tem o valor do rrn, agora é dar um seek direto no arquivo de dados!
									
				
				/*por nome da equipe vencedora
				por apelido do mvp*/
				//ler o campo de mvp ou equipe vencedora
				//atribuir no novo.IndiceSecundario // 2 vetores?
				
				//buscarSecundário no vetorSecundario
					//return 
				
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

				
				//AO ACABAR, ATRIBUIR O VALOR DO QTDRRN CORRETO! o ultimo RRN+192.
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
	char equipeAzul[40], equipeVermelha[40], equipeVencedora[40], apelidoMvp[40];
	char duracaoPartida[6], placarAzul[3], placarVermelha[3], chavePK[9];

	//Leitura das equipes que jogaram a partida
	le_equipe(novo.equipe_azul);
	le_equipe(novo.equipe_vermelha);
	
	//Leitura da data da partida
	le_data(novo.data);

	//Leitura da duracao da partida
	le_duracao(novo.duracao);

	//Leitura da equipe que venceu a partida
	le_vencedora(novo.equipe_vencedora, novo.equipe_azul, novo.equipe_vermelha);

	//Leitura dos placares da partida
	le_placar(novo.placar_azul);
	le_placar(novo.placar_vermelha);

	//Leitura do apelido do mvp da partida
	le_apelido_mvp(novo.apelido_mvp);

	/* Geracao da chave primária */
	novo.chave_primaria[0] = toupper(novo.equipe_azul[0]);
	novo.chave_primaria[1] = toupper(novo.equipe_vermelha[0]);
	novo.chave_primaria[2] = toupper(novo.apelido_mvp[0]);
	novo.chave_primaria[3] = toupper(novo.apelido_mvp[1]);

	novo.chave_primaria[4] = novo.data[0];
	novo.chave_primaria[5] = novo.data[1];
	novo.chave_primaria[6] = novo.data[3];
	novo.chave_primaria[7] = novo.data[4];

	novo.chave_primaria[8] = '\0';
	puts(novo.chave_primaria);

	return novo;
}


/* Retorna 1 se o arquivo de dados existir */
int verificarArquivoDados(FILE *dataFile) {
	dataFile = fopen("matches.dat", "r");
	if(dataFile == NULL)
		return 0;

	return 1;
}

/* Retorna 1 se o arquivo de indices primarios existir */
int verificarIndicePrimario(FILE *primFile) {
	primFile = fopen("iprimary.idx", "r");
	if(primFile == NULL)
		return 0;

	return 1;
}

/* Retorna 1 se o arquivo de indices secundario existir */
int verificarIndiceSecundario(FILE *secFile, char *nome) {
	secFile = fopen(nome, "r");
	if(secFile == NULL)
		return 0;

	return 1;
}

/* Retorna 0 se o arquivo de indices primarios foi criado com sucesso */
int criarIndicePrimario(FILE *primFile) {
	primFile = fopen("iprimary.idx", "w");
	if(primFile == NULL) {
		perror("Ocorreu um erro ao abrir o arquivo de índices primário!\n");
		return 1;
	}

	return 0;
}

/* Retorna 0 se o arquivo de indices secundarios foi criado com sucesso */
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

/*	Retorna 0 se estourar o numero de tamanhoPrimario */
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

	return 1;
}

int buscarPartida(indexStruct *vetorPrimario, int tamanhoPrimario, char *chavePK){
	
	int i, compara=0;

	for(i=0; i<tamanhoPrimario; i++){
		compara = strcmp(chavePK, vetorPrimario[i].primaria);
		if(compara == 0)
			return vetorPrimario[i].rrn;
		if(compara > 0)
			return -1;
	}

	return -1;
}

/* Le o nome de uma das equipes */
void le_equipe(char equipe_chamada[]) {
	char equipe[40];

	scanf("%39[^\n]", equipe);
	getchar();
	strcpy(equipe_chamada, equipe);
}

/* Le e verifica a duracao da partida */
void le_duracao(char duracao_chamada[]) {
	char duracao[6];
	int flag;

	do {
		flag = 0;
		scanf("%c%c", &duracao[0], &duracao[1]);
		duracao[2] = getchar();
		scanf("%c%c", &duracao[3], &duracao[4]);
		getchar(); //Tira \n do buffer
		//A condição a seguir obriga que a duração seja do formato "nn:nn" onde n é qualquer numero natural
		if(duracao[2] != ':' || (duracao[0] != '1' && duracao[0] != '2' && duracao[0] != '3' && duracao[0] != '4' && duracao[0] != '5' && duracao[0] != '6' && duracao[0] != '7' && duracao[0] != '8' && duracao[0] != '9'
								&& duracao[1] != '1' && duracao[1] != '2' && duracao[1] != '3' && duracao[1] != '4' && duracao[1] != '5' && duracao[1] != '6' && duracao[1] != '7' && duracao[1] != '8' && duracao[1] != '9'
								&& duracao[3] != '1' && duracao[3] != '2' && duracao[3] != '3' && duracao[3] != '4' && duracao[3] != '5' && duracao[3] != '6' && duracao[3] != '7' && duracao[3] != '8' && duracao[3] != '9'
								&& duracao[4] != '1' && duracao[4] != '2' && duracao[4] != '3' && duracao[4] != '4' && duracao[4] != '5' && duracao[4] != '6' && duracao[4] != '7' && duracao[4] != '8' && duracao[4] != '9')
							 || (duracao[0] == '0' && duracao[1] == '0' && duracao[3] == '0' && duracao[4] == '0')) {
			printf(ENTRADA_INVALIDA);
			flag = 1;
		}
	} while (flag);

	strcpy(duracao_chamada, duracao);	
}

/* Le e verifica o nome da equipe vencedora */
void le_vencedora(char vencedora_chamada[], char equipe_azul[], char equipe_vermelha[]) {
	int flag;
	char vencedora[40];

	do{
		flag = 0;
		scanf("%39[^\n]", vencedora);
		getchar(); //Tira \n do buffer
		
		if(strcmp(equipe_azul, vencedora) != 0 && strcmp(equipe_vermelha, vencedora) != 0)
			flag = 1;
		if(flag)
			printf(ENTRADA_INVALIDA);

	}while(flag == 1);

	strcpy(vencedora_chamada, vencedora);
}

/* Le e verifica o placar da partida */
void le_placar(char placar_chamada[]) {
	char placar[3];
	int tamanho_placar;

	do {
		scanf("%2[^\n]", placar);
		getchar();	//Tira \n do buffer
		tamanho_placar = strlen(placar);
		if(tamanho_placar != 2)
			printf(ENTRADA_INVALIDA);
	} while (tamanho_placar != 2);

	strcpy(placar_chamada, placar);
}

/* Le o nome do melhor jogador da partida */
void le_apelido_mvp(char apelido_chamada[]) {
	char apelido_mvp[40];

	scanf("%39[^\n]", apelido_mvp);
	getchar();	//Tira \n do buffer
	strcpy(apelido_chamada, apelido_mvp);
}

/* Le e verifica a data da partida */
void le_data(char data_chamada[]) {
	char dia0, dia1;
	char mes0, mes1;
	char ano0, ano1, ano2, ano3;
	char data[11];
	char c;
	int flag = 1, flag2 = 0;

	do {
		flag = 0;
		flag2 = 0;
		scanf("%c%c", &dia0, &dia1);
		c = getchar();
		if(c != '/') flag2 = 1; 
		scanf("%c%c", &mes0, &mes1);
		c = getchar();
		if(c != '/') flag2 = 1;
		scanf("%c%c%c%c", &ano0, &ano1, &ano2, &ano3);
		getchar();

		if(flag2 == 1) {
			printf(ENTRADA_INVALIDA);
			flag = 1;
		}
		else if(dia0 == '-' || dia1 == '-' || mes0 == '-' || mes1 == '-' || ano0 == '-' || ano1 == '-' || ano2 == '-'|| ano3 == '-') {
			printf(ENTRADA_INVALIDA);
			flag = 1;
		}
		else if(dia0 == '0' && dia1 == '0') {
			printf(ENTRADA_INVALIDA);
			flag = 1;
		}
		else if(dia0 == '3' && dia1 != '0' && dia1 != '1') {
			printf(ENTRADA_INVALIDA);
			flag = 1;
		}
		else if(mes0 == '0' && mes1 == '0') {
			printf(ENTRADA_INVALIDA);
			flag = 1;
		}
		else if(mes0 == '1' && mes1 != '0' && mes1 != '1' && mes1 != '2') {
			printf(ENTRADA_INVALIDA);
			flag = 1;
		}
		else if(ano0 != '2' || ano1 != '0' || ano2 != '1' || (ano3 != '1' && ano3 != '2' && ano3 != '3' && ano3 != '4' && ano3 != '5')) {
			printf(ENTRADA_INVALIDA);
			flag = 1;
		}
	} while(flag == 1);

	data[0] = dia0;
	data[1] = dia1;
	data[2] = '/';
	data[3] = mes0;
	data[4] = mes1;
	data[5] = '/';
	data[6] = ano0;
	data[7] = ano1;
	data[8] = ano2;
	data[9] = ano3;
	data[10] = '\0';

	strcpy(data_chamada, data);
}

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
#define REGISTRO_N_ENCONTRADO	"Registro não encontrado!\n"

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
	char vencedora[40];
	char mvp[40];
} indexStruct;

//Prototipos das funções usadas
int verificarArquivoDados(FILE *); //Verificar se o arquivo de dados existe
int verificarIndicePrimario(FILE *); //Verificar se o indice primario existe
int verificarIndiceSecundario(FILE *, char *); //Verificar se os indices secundários existem
int criarIndicePrimario(FILE *); //Criar o indice primário
int criarIndiceSecundario(FILE *, char *); //Criar os indices secundários
int carregarIndices(); //Carregar os indices (DISCO PARA RAM)
Partida inserirPartida(); //Inserir registro
int buscarPartida(indexStruct *vetorPrimario, int tamanhoPrimario, char *chavePK); //Buscar registro
int alterarPartida(FILE *, int, char *); //Alterar registro
int removerPartida(); //Remover registro
void listarPartidas(); //Listar registros
void liberarEspaco(); //Liberar espaço
int atualizarIndices(); //Atualizar todos os indices
int ordenaIndicePrimario(indexStruct *, int , indexStruct);

void le_equipe(char []);
void le_duracao(char []);
void le_vencedora(char [], char [], char []);
void le_placar(char []);
void le_apelido_mvp(char []);
void le_data(char[]);
void imprimirPartida(FILE *, int);

int main () {
	/* Variaveis para arquivos, respectivamente: de dados, de indice primario, 
	de indice secundario para o nome da equipe vencedora e de indice secundario para apelido do MVP */
	FILE *data_file, *index_file, *winner_index_file, *mvp_index_file;
	int opcao, opcao2;
	int existe, flag, indiceRemove;
	int tamanho, numero_sharps, qtdRRN=0, i, j, tamanhoPrimario=0, tamanhoSecundarioVencedor=0, tamanhoSecundarioMvp=0;
	char c, busca_chave_primaria[9], busca_nome_vencedora[40], busca_apelido_mvp[40], altera_duracao[6], controle, ajuda[40];
	Partida partida;
	indexStruct vetorPrimario[10000], vetorSecundarioVencedor[10000], vetorSecundarioMvp[10000],novoIndice;

	/* Ao iniciar
		-Verificar se existe o arquivo de dados
			se existir: abrir o arquivo para escrita e leitura
			se não existir: criar o arquivo de dados no disco, abrindo para escrita e leitura
	*/

	existe = verificarArquivoDados(data_file);
	if(!existe)
		data_file = fopen("matches.dat", "w");
	else 
		data_file = fopen("matches.dat", "a+");	
	
	existe = verificarIndicePrimario(index_file);
	if(!existe)
		index_file = fopen("iprimary.idx", "w");
	else{
		index_file = fopen("iprimary.idx", "r");
		
		fscanf(index_file, "%c\n", &controle);
		if(controle == 'S'){
			i=0;

			while(!feof(index_file)){
				fscanf(index_file, "%s %lld ", vetorPrimario[i].primaria, &vetorPrimario[i].rrn);
				tamanhoPrimario++;
				i++;
				qtdRRN+=192;
			}
		}
		else{
			i=0;

			
			while(fscanf(data_file, "%c", &vetorPrimario[i].primaria[0]) != EOF){
				//fscanf(data_file, "%c", &vetorPrimario[i].primaria[0]);
				fscanf(data_file, "%c", &vetorPrimario[i].primaria[1]);
				fscanf(data_file, "%c", &vetorPrimario[i].primaria[2]);
				fscanf(data_file, "%c", &vetorPrimario[i].primaria[3]);
				fscanf(data_file, "%c", &vetorPrimario[i].primaria[4]);
				fscanf(data_file, "%c", &vetorPrimario[i].primaria[5]);
				fscanf(data_file, "%c", &vetorPrimario[i].primaria[6]);
				fscanf(data_file, "%c", &vetorPrimario[i].primaria[7]);
				vetorPrimario[i].primaria[8] = '\0';
			
				vetorPrimario[i].rrn  = qtdRRN;
				tamanhoPrimario++;
				i++;
				qtdRRN+=192;

				fseek(data_file, 184, SEEK_CUR);
			}
			
			for(i=0; i< tamanhoPrimario; i++){
				printf("%s %lld\n", vetorPrimario[i].primaria, vetorPrimario[i].rrn);
			}
		}

		fclose(index_file);
	}

	// INDICE SECUNDARIO (EQUIPE VENCEDORA)

	existe = verificarIndiceSecundario(winner_index_file, "iwinner.idx");

	if(!existe){
		winner_index_file = fopen("iwinner.idx", "w");
	}
	else{
		winner_index_file = fopen("iwinner.idx", "r");

		fscanf(winner_index_file, "%c\n", &controle);

		if(controle == 'S'){
			i=0;
			while(fscanf(winner_index_file, "%[^#]#%[^#]#", &vetorSecundarioVencedor[i].primaria, &vetorSecundarioVencedor[i].vencedora) != EOF){
				//19 + 2 nomes de equipes
				tamanhoSecundarioVencedor++;
				i++;
			}
		}
		else{
			i=0;
			j=0;
			while(fscanf(winner_index_file, "%[^@]@", &vetorSecundarioVencedor[i].primaria) != EOF){
				j+=strlen(vetorSecundarioVencedor[i].primaria);
				fscanf(winner_index_file, "%[^@]@", &vetorSecundarioVencedor[i].vencedora); //desconsidera
				j+=strlen(vetorSecundarioVencedor[i].vencedora);
				fscanf(winner_index_file, "%[^@]@", &vetorSecundarioVencedor[i].vencedora); //desconsidera
				j+=strlen(vetorSecundarioVencedor[i].vencedora);
				fscanf(winner_index_file, "%[^@]@", &vetorSecundarioVencedor[i].vencedora); //desconsidera
				j+=strlen(vetorSecundarioVencedor[i].vencedora);
				fscanf(winner_index_file, "%[^@]@", &vetorSecundarioVencedor[i].vencedora); //desconsidera
				j+=strlen(vetorSecundarioVencedor[i].vencedora);
				fscanf(winner_index_file, "%[^@]@", &vetorSecundarioVencedor[i].vencedora);
				j+=strlen(vetorSecundarioVencedor[i].vencedora);

				fseek(winner_index_file, 6, SEEK_CUR);
				j+=6;

				fscanf(winner_index_file, "%[^@]@", &ajuda);
				j+= strlen(ajuda);

				fseek(winner_index_file, (192-j), SEEK_CUR);

				tamanhoSecundarioVencedor++;
				i++;
			}
		}
	}

	fclose(winner_index_file);

	// INDICE SECUNDARIO (MVP)

	existe = verificarIndiceSecundario(mvp_index_file, "imvp.idx");

	if(!existe){
		mvp_index_file = fopen("imvp.idx", "w");
	}
	else{
		mvp_index_file = fopen("imvp.idx", "r");

		fscanf(mvp_index_file, "%c\n", &controle);

		if(controle == 'S'){
			i=0;
			while(fscanf(mvp_index_file, "%[^#]#%[^#]#", &vetorSecundarioMvp[i].primaria, &vetorSecundarioMvp[i].mvp) != EOF){
				tamanhoSecundarioMvp++;
				i++;
			}
		}
		else{
			i=0;
			j=0;
			while(fscanf(mvp_index_file, "%[^@]@", &vetorSecundarioMvp[i].primaria) != EOF){
				j+=strlen(vetorSecundarioMvp[i].primaria);
				fscanf(mvp_index_file, "%[^@]@", &vetorSecundarioMvp[i].mvp); //desconsidera
				j+=strlen(vetorSecundarioMvp[i].mvp);
				fscanf(mvp_index_file, "%[^@]@", &vetorSecundarioMvp[i].mvp); //desconsidera
				j+=strlen(vetorSecundarioMvp[i].mvp);
				fscanf(mvp_index_file, "%[^@]@", &vetorSecundarioMvp[i].mvp); //desconsidera
				j+=strlen(vetorSecundarioMvp[i].mvp);
				fscanf(mvp_index_file, "%[^@]@", &vetorSecundarioMvp[i].mvp); //desconsidera
				j+=strlen(vetorSecundarioMvp[i].mvp);
				fscanf(mvp_index_file, "%[^@]@", &vetorSecundarioMvp[i].mvp); //desconsidera
				j+=strlen(vetorSecundarioMvp[i].mvp); 
				fscanf(mvp_index_file, "%[^@]@", &vetorSecundarioMvp[i].mvp); //desconsidera
				j+=strlen(vetorSecundarioMvp[i].mvp);
				fscanf(mvp_index_file, "%[^@]@", &vetorSecundarioMvp[i].mvp); //desconsidera
				j+=strlen(vetorSecundarioMvp[i].mvp);
				fscanf(mvp_index_file, "%[^@]@", &vetorSecundarioMvp[i].mvp); //desconsidera
				j+=strlen(vetorSecundarioMvp[i].mvp);

				fseek(mvp_index_file, (192-j), SEEK_CUR);

				tamanhoSecundarioMvp++;
				i++;
			}
		}
	}

	fclose(mvp_index_file);

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

				strcpy(novoIndice.primaria, partida.chave_primaria);				
				novoIndice.rrn = qtdRRN;

				flag = 0;

				for(i=0; i<tamanhoPrimario; i++){
					if(strcmp(vetorPrimario[i].primaria, novoIndice.primaria) == 0){
						flag = 1;
						break;
					}
				}

				if(!flag){	
					if(!ordenaIndicePrimario(vetorPrimario, tamanhoPrimario, novoIndice))
						printf("Vetor de indices primarios cheio!\n");

					qtdRRN+=192;
					tamanhoPrimario++;

					novoIndice.rrn = 0;
					strcpy(novoIndice.vencedora, partida.equipe_vencedora);
					if(!ordenaIndicePrimario(vetorSecundarioVencedor, tamanhoSecundarioVencedor, novoIndice))
						printf("Vetor de indices secundarios(vencedora) cheio!\n");
					tamanhoSecundarioVencedor++;

					strcpy(novoIndice.mvp,partida.apelido_mvp);
					if(!ordenaIndicePrimario(vetorSecundarioMvp, tamanhoSecundarioMvp, novoIndice))
						printf("Vetor de indices secundarios(mvp) cheio!\n");
					tamanhoSecundarioMvp++;

					winner_index_file = fopen("iwinner.idx", "w");
					rewind(winner_index_file);
					fprintf(winner_index_file, "N\n");								
					fclose(winner_index_file);

					mvp_index_file = fopen("imvp.idx", "w");
					rewind(mvp_index_file);
					fprintf(mvp_index_file, "N\n");								
					fclose(mvp_index_file);

					index_file = fopen("iprimary.idx", "w");
					rewind(index_file);
					fprintf(index_file, "N\n");								
					fclose(index_file);
				}
				else{
					printf("Chave primaria já inserida!\n");
				}

			break;

			case BUSCAR_PARTIDA:
				scanf("%d", &opcao2);
				getchar();

				switch(opcao2) {
					case 1:
						scanf("%8[^\n]", busca_chave_primaria);
						getchar();

						flag = 0;

						for(i=0; i<tamanhoPrimario; i++){
							if(strcmp(vetorPrimario[i].primaria, busca_chave_primaria) == 0){
								imprimirPartida(data_file, vetorPrimario[i].rrn);
								flag = 1;
								break;
							}
						}
						
						if(!flag){
							printf(REGISTRO_N_ENCONTRADO);
						}

					break;

					//por nome da equipe vencedora
					case 2:
						scanf("%39[^\n]", busca_nome_vencedora);
						getchar();

						flag = 0;

						for(i=0; i<tamanhoSecundarioVencedor; i++){
							if(strcmp(vetorSecundarioVencedor[i].vencedora, busca_nome_vencedora) == 0){
								for(j=0; j<tamanhoPrimario; j++){
									if(strcmp(vetorSecundarioVencedor[i].primaria, vetorPrimario[j].primaria) == 0){
										imprimirPartida(data_file, vetorPrimario[j].rrn);
										flag = 1;
										break;
									}
								}
							}
						}

						if(!flag){
							printf(REGISTRO_N_ENCONTRADO);
						}

					break;

					//por apelido do mvp
					case 3:
						scanf("%39[^\n]", busca_apelido_mvp);
						getchar();

						for(i=0; i<tamanhoSecundarioMvp; i++){
							if(strcmp(vetorSecundarioMvp[i].mvp, busca_apelido_mvp) == 0){
								for(j=0; j<tamanhoPrimario; j++){
									if(strcmp(vetorSecundarioMvp[i].primaria, vetorPrimario[j].primaria) == 0){
										imprimirPartida(data_file, vetorPrimario[j].rrn);
										flag = 1;
										break;
									}
								}
							}
						}

						if(!flag){
							printf(REGISTRO_N_ENCONTRADO);
						}
						
					break;
				}

			break;

			case REMOVER_PARTIDA:

				scanf("%8[^\n]", busca_chave_primaria);
				getchar();

				flag = 0;

					for(i=0; i<tamanhoPrimario; i++){
						if(strcmp(vetorPrimario[i].primaria, busca_chave_primaria) == 0){
							for(j=0; j<tamanhoSecundarioVencedor; j++){
								if(strcmp(vetorSecundarioVencedor[j].primaria, vetorPrimario[i].primaria)
									vetorSecundarioVencedor[j].primaria[7] = 'Z';
							}
							for(j=0; j<tamanhoSecundarioMvp; j++){
								if(strcmp(vetorSecundarioMvp[j].primaria, vetorPrimario[i].primaria)
									vetorSecundarioMvp[j].primaria[7] = 'Z';
							}
							vetorPrimario[i].primaria[7] = 'Z';
							flag = 1;
							break;
						}
					}
					
				if(!flag){
					printf(REGISTRO_N_ENCONTRADO);

					index_file = fopen("iprimary.idx", "w");
					winner_index_file = fopen("iwinner.idx", "w");
					mvp_index_file = fopen("imvp.idx", "w");


					rewind(index_file);
					rewind(winner_index_file);
					rewind(mvp_index_file);

					fprintf(index_file, "N\n");	
					fprintf(winner_index_file, "N\n");								
					fprintf(mvp_index_file, "N\n");			

					fclose(index_file);
					fclose(winner_index_file);
					fclose(mvp_index_file);
				}
				else{
					printf("Chave primaria já inserida!\n");
				}
			break;

			case MODIFICAR_DURACAO:
				
				scanf("%8[^\n]", busca_chave_primaria);
				getchar();


				scanf("%5[^\n]", altera_duracao);
				getchar();

				flag = 0;
	
				for(i=0; i<tamanhoPrimario; i++){
					if(strcmp(vetorPrimario[i].primaria, busca_chave_primaria) == 0){
						printf("%s\n", altera_duracao);				
						flag = alterarPartida(data_file, vetorPrimario[i].rrn, altera_duracao);
						break;
					}
				}
					
				if(!flag){
					printf(REGISTRO_N_ENCONTRADO);
				}

			break;

			case LISTAR_PARTIDAS:
				scanf("%d", &opcao2);
				getchar();

				switch(opcao2) {
					case 1:
						for(i=0; i<tamanhoPrimario; i++){
								if(vetorPrimario[i].primaria[7] != 'Z')
									imprimirPartida(data_file, vetorPrimario[i].rrn);
						}
					break;

					case 2:
						for(i=0; i<tamanhoSecundarioVencedor; i++){
							for(j=0; j<tamanhoPrimario; j++){
								if(vetorPrimario[j].primaria[7] != 'Z')
									if(strcmp(vetorSecundarioVencedor[i].primaria, vetorPrimario[j].primaria) == 0)
										imprimirPartida(data_file, vetorPrimario[i].rrn);
							}
						}
					break;

					case 3:
						for(i=0; i<tamanhoSecundarioMvp; i++){
							for(j=0; j<tamanhoPrimario; j++){
								if(vetorPrimario[j].primaria[7] != 'Z')
									if(strcmp(vetorSecundarioMvp[i].primaria, vetorPrimario[j].primaria) == 0)
										imprimirPartida(data_file, vetorPrimario[i].rrn);
							}
						}

					break;
				}
				
			break;

			case LIBERAR_ESPACO:

			break;

			case 7:

				//atualizar os indices no disco (?)										
				index_file = fopen("iprimary.idx", "w+");
				winner_index_file = fopen("iwinner.idx", "w+");
				mvp_index_file = fopen("imvp.idx", "w+");
				
				fprintf(index_file, "N\n");
				fprintf(winner_index_file, "N\n");
				fprintf(mvp_index_file, "N\n");								

				for(i=0; i< tamanhoSecundarioMvp; i++){
					fprintf(mvp_index_file, "%s#%s#", vetorSecundarioMvp[i].primaria, vetorSecundarioMvp[i].mvp);
				}

				for(i=0; i< tamanhoSecundarioVencedor; i++){
					fprintf(winner_index_file, "%s#%s#", vetorSecundarioVencedor[i].primaria, vetorSecundarioVencedor[i].vencedora);
				}

				for(i=0; i< tamanhoPrimario; i++){
					fprintf(index_file, "%s %lld ", vetorPrimario[i].primaria, vetorPrimario[i].rrn);
				}
				
				rewind(mvp_index_file);
				rewind(winner_index_file);
				rewind(index_file);
				fprintf(mvp_index_file, "S\n");
				fprintf(winner_index_file, "S\n");
				fprintf(index_file, "S\n");	
				fclose(mvp_index_file);
				fclose(winner_index_file);							
				fclose(index_file);				

				fclose(data_file);
				return 0;
			break;

			default:
				printf(ENTRADA_INVALIDA);
			break;			
		}
	} while(opcao != 7);

	return 0;
}

/*==================- IMPLEMENTAÇÃO DAS FUNÇÕES -==================*/

Partida inserirPartida(){
	
	Partida novo;
	char equipeAzul[40], equipeVermelha[40], equipeVencedora[40], apelidoMvp[40];
	char duracaoPartida[6], placarAzul[3], placarVermelha[3], chavePK[9];
	int flag;

	//Leitura das equipes que jogaram a partida
	le_equipe(novo.equipe_azul);
	le_equipe(novo.equipe_vermelha);
	
	//Leitura da data da partida
	le_data(novo.data);

	//Leitura da duracao da partida
	le_duracao(novo.duracao);
	// flag = 0;
	// do {
	// 	scanf("%5[^\n]", novo.duracao);
	// 	getchar();	//Tira \n do buffer
	// 	flag = strlen(novo.duracao);
	// 	if(flag != 5)
	// 		printf("ROLA");
	// } while (flag != 5);

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
	if(secFile == NULL) {
		perror("Ocorreu um erro ao abrir o arquivo de índices secundário!\n");
		return 1;	
	}

	return 0;
}

/* Retorna -1 se a partida não for encontrada; se for, retorna o rrn */
int buscarPartida(indexStruct *vetorPrimario, int tamanhoPrimario, char *chavePK){
	
	int i, compara=0;

	for(i = 0; i < tamanhoPrimario; i++) {
		compara = strcmp(chavePK, vetorPrimario[i].primaria);
		if(compara == 0)
			return vetorPrimario[i].rrn;
		if(compara > 0)
			return -1;
	}

	return -1;
}

int carregarIndices(char **indices) {
}

int alterarPartida(FILE *data_file, int rrn, char *altera_duracao) {
	//imprimir partida formatada, dados na ordem em que foram inseridos pelo usuario
	FILE *ponteiro;
	char caractere;
	int flag=0;

	ponteiro = data_file; 

	// posicionar o cursor na posicao do rrn
	fseek(ponteiro, rrn+37, SEEK_SET);

	fprintf(ponteiro, "%s", altera_duracao);
	
	return 1;
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

void imprimirPartida(FILE *data_file, int rrn) {
	//imprimir partida formatada, dados na ordem em que foram inseridos pelo usuario
	FILE *ponteiro;
	char caractere;
	int flag=0;

	ponteiro = data_file; 

	// posicionar o cursor na posicao do rrn
	fseek(ponteiro, rrn+9, SEEK_SET);

	while(flag<192){
		
		caractere = fgetc(ponteiro);

		if(caractere == '@')
			printf("\n");
		else if(caractere == '#'){
			printf("\n");	
			break;
		}
		else
			printf("%c", caractere);

		flag++;
	}
}

/*	Retorna 0 se estourar o numero de tamanhoPrimario */
int ordenaIndicePrimario(indexStruct *vetorPrimario, int tamanhoPrimario, indexStruct novoIndice){
	
	int i, j;	

	if(tamanhoPrimario == 10000){
		return 0;
	}

	if(tamanhoPrimario == 0){
		vetorPrimario[0] = novoIndice;
		return 1;
	}

	for(i=0; i < tamanhoPrimario; i++) {
		if(strcmp(vetorPrimario[i].primaria, novoIndice.primaria) > 0) {
			for(j=tamanhoPrimario-1; j>=i; j--) {
				vetorPrimario[j+1] = vetorPrimario[j]; 
			}
			vetorPrimario[i] = novoIndice;
			return 1;		
		}
	}

	vetorPrimario[i] = novoIndice;
	return 1;
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
		duracao[5] = '\0';
		//printf("%c%c:%c%c", duracao[0], duracao[1], duracao[3], duracao[4]);
		//A condição a seguir obriga que a duração seja do formato "nn:nn" onde n é qualquer numero natural
		if(duracao[2] != ':' ) {
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
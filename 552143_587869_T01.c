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

#define INSERIR_PARTIDA 	1
#define REMOVER_PARTIDA 	2
#define MODIFICAR_DURACAO 	3
#define BUSCAR_PARTIDA 		4
#define LISTAR_PARTIDAS 	5
#define LIBERAR_ESPACO 		6

#define ENTRADA_INVALIDA "Campo inválido! Informe novamente:"

//setar o tamanho das strings!<<<<<<<<<<<<<< um byte a mais que especificado no pdf para guardar o '\0' (6 bytes serão guardados como 5 no 
//arquivo quando excluirmos o '\0')

//Definição das estruturas usadas
struct data {
	int dia[3];
	int mes[3];
	int ano[5];
}
typedef data Data;

struct partida {
	char equipe_azul[40];
	char equipe_vermelha[40];
	char equipe_vencedora[40];
	char apelido_mvp[40];
	Data data_partida;
	char duracao[6];
	char placar_azul[3];
	char placar_vermelha[3];
	char chave_primaria[9];
};
typedef struct partida Partida;

//Prototipos das funções usadas
					//Estrutura de dados para armazenar os indices em RAM. ??????????????NÃO ENTENDI??????????
verificarArquivoDados(); //Verificar se o arquivo de dados existe
verificarIndicePrimario(); //Verificar se o indice primario existe
verificarIndiceSecundario(); //Verificar se os indices secundários existem
criarIndicePrimario(); //Criar o indice primário
criarIndiceSecundario(); //Criar os indices secundários
carregarIndices(); //Carregar os indices (DISCO PARA RAM)
Partida inserirPartida(); //Inserir registro
buscarPartida(); //Buscar registro
alterarPartida(); //Alterar registro
removerPartida(); //Remover registro
listarPartidas(); //Listar registros
liberarEspaço(); //Liberar espaço
atualizarIndices(); //Atualizar todos os indices

int main () {
	/*variaveis para arquivos, respectivamente: de dados, de indice primario, 
	de indice secundario para o nome da equipe vencedora e de indice secundario para apelido do MVP*/
	FILE *data_file, *index_file, *winner_index_file, *mvp_index_file;
	int opcao;
	
	
	/*
	Ao iniciar
		-Verificar se existe o arquivo de dados
			se existir: abrir o arquivo para escrita e leitura
			se não existir: criar o arquivo de dados no disco, abrindo para escrita e leitura

		-Verificar se existem os arquivos de indices
			Se existirem:
				Se estiverem consistentes: carregar os indices para RAM.
				Senão: refazer os indices e carrega-los para RAM.
			Se não existirem: criar os indices na RAM e grava-los no disco.
	*/		

	do {
		scanf("%d", &opcao);

		switch(opc) {
			case INSERIR_PARTIDA:

				Partida p;

				p = inserirPartida(); // unica funcao para leitura dos campos. Devolve uma struct partida
				
				/*novo.equipe_azul = le_nome_equipe();
				novo.equipe_vermelha = le_nome_equipe();
				data_partida = le_data();
				novo.duracao = le_duracao();
				novo.equipe_vencedora = le_vencedora(); // tratei, falta passar o novo como parametro da funcao
				novo.placar_azul = le_placar();
				novo.placar_vermelha = le_placar();
				apelido_mvp = le_apelido_mvp();
				//GERAR CHAVE PRIMARIA, FUNÇÃO JÁ DECLARADA
				*/
			break;

			case REMOVER_PARTIDA:
			
				if(buscarPartida())
					//se encontrar devolve a posicao
					removerPartida()
				
				else
					printf("Partida não encontrada!\n");
				
			break;

			case MODIFICAR_DURACAO:
			
				if(buscarPartida())
					//se encontrar devolve a posicao
					alterarPartida()
				
				else
					printf("Partida não encontrada!\n");
				
			break;

			case BUSCAR_PARTIDA:
				/*por código
				por nome da equipe vencedora
				por apelido do mvp*/
				
			break;

			case LISTAR_PARTIDAS:
				/*por código
				por nome da equipe vencedora
				por apelido do mvp*/

			break;

			case LIBERAR_ESPACO:
			break;

			default:
				printf(ENTRADA_INVALIDA);
			break;			
		}
	} while(opcao != 7)	//Segundo a descricao do problema, a opcao de finalizar eh a de numero 7


	return 0;
}

/*==================- IMPLEMENTAÇÃO DAS FUNÇÕES -==================*/

Partida inserirPartida(){
	
	Partida novo;
	
	int flag = 0;

	char equipeAzul[40], equipeVermelha[40], equipeVencedora[40], apelidoMvp[40], duracaoPartida[6], placarAzul[3], placarVermelha[3], chavePK[9];
	Data dataPartida;
	
	//Leitura das equipes
	scanf("%39[\n]", equipeAzul);
	getchar();
	
	scanf("%39[\n]", equipeVermelha);
	getchar();
	
	
	novo.equipe_azul = equipeAzul;
	novo.equipe_vermelha = equipeVermelha;
		
	//Leitura da data
	
	flag = 0;
	do {
		flag = 0;
		scanf("%d/%d/%d", &dataPartida.dia, &dataPartida.mes, &dataPartida.ano);
		if(dataPartida.dia < 1 || dataPartida.dia > 31 || dataPartida.mes < 1 || dataPartida.mes > 12 || dataPartida.ano < 2011 || dataPartida.ano > 2015) {
			flag = 1;
			printf(ENTRADA_INVALIDA);
		}
	} while(flag == 1);

	novo.data_partida = dataPartida;

	//Leitura da duracao
	
	flag = 0;
	do {
		scanf("%5[\n]", duracaoPartida);
		getchar();	//Tira \n do buffer
		flag = strlen(duracaoPartida);
		if(flag != 5)
			printf(ENTRADA_INVALIDA);
	} while (flag != 5);

	novo.duracao = duracaoPartida;

	//Leitura da equipe vencedora
	
	flag = 0;
	do{
		scanf("%39[\n]", equipeVencedora);
		getchar(); //Tira \n do buffer
		
		if(!strcmp(equipeAzul, equipeVencedora)
			flag = 1;
		else if(!strcmp(equipeVermelha, equipeVencedora)
			flag = 1;
		else
			flag = 0;

	}while(flag == 0);
	
	novo.equipe_vencedora = equipeVencedora;

	//Leitura dos placares
	
	flag = 0;

	do {
		scanf("%2[\n]", placarAzul);
		getchar();	//Tira \n do buffer
		flag = strlen(placarAzul);
		if(flag != 2)
			printf(ENTRADA_INVALIDA);
	} while (flag != 2);

	novo.placar_azul = placarAzul;
	
	flag = 0;

	do {
		scanf("%2[\n]", placarVermelha);
		getchar();	//Tira \n do buffer
		flag = strlen(placarVermelha);
		if(flag != 2)
			printf(ENTRADA_INVALIDA);
	} while (flag != 2);

	novo.placar_vermelha = placarVermelha;

	//Leitura do apelido

	scanf("%39[\n]", apelidoMvp);
	getchar();	//Tira \n do buffer
	
	novo.apelido_mvp = apelidoMvp;
	
	//Gerar a chave primaria

	/*
		composicao de letras maiusculas da primeira letra do nome da equipe azul, seguida da primeira letra do nome da equipe vermelha, seguida das duas primeiras letras do apelido do mvp, o dia e o mes da partida.

		Exemplo: FTFE0705
			Fnatic
			Team Mid-Solo
			Ferbien
			07/05
	*/

	chavePK[0] = toupper(equipeAzul[0]);
	chavePK[1] = toupper(equipeVermelha[0]);
	chavePK[2] = toupper(apelidoMvp[0]);
	chavePK[3] = toupper(apelidoMvp[2]);
	
	strcat(chavePK, dataPartida.dia);
	strcat(chavePK, dataPartida.mes);
	chavePK[8] = '\0';
	
	// Retorno da struct construida

	return novo;
}




















/*======================================================- LIMBO -====================================================== */


/* Lê nome da equipe azul */
char *le_nome_equipe() {
	char equipe[40];

	scanf("%39[\n]", equipe);
	getchar();	//Tira \n do buffer
	return equipe;
}

/* Lê e trata a data da partida */
RETORNO le_data() {
	Data data;
	int flag=0;

	do {
		flag = 0;
		scanf("%d/%d/%d", &data.dia, &data.mes, &data.ano);
		if(data.dia < 1 || data.dia > 31 || data.mes < 1 || data.mes > 12 || data.ano < 2011 || data.ano > 2015) {
			flag = 1;
			printf(ENTRADA_INVALIDA);
		}
	} while(flag == 1);
	
	return data;
}

/* Lê e trata o nome da equipe vencedora */
char *le_vencedora() {
	char vencedora[40];
	
	int flag = 0;	

	do{
		scanf("%39[\n]", vencedora);
		getchar(); //Tira \n do buffer
		
		if(!strcmp(novo.equipe_azul, vencedora)
			flag = 1;
		else if(!strcmp(novo.equipe_vermelha, vencedora)
			flag = 1;
		else
			flag = 0;

	}while(flag == 0);
	
	return vencedora;
}

/* Lê e trata o placar da equipe */
char *le_placar() {
	char placar[3];
	int tamanho_placar;

	do {
		scanf("%2[\n]", placar);
		getchar();	//Tira \n do buffer
		tamanho_placar = strlen(placar);
		if(tamanho_placar != 2)
			printf(ENTRADA_INVALIDA);
	} while (tamanho_placar != 2);

	return placar;
}

/*Lê e trata a duração da partida */
char *le_duracao() {
	int tamanho_duracao;
	char duracao[6];

	do {
		scanf("%5[\n]", duracao);
		getchar();	//Tira \n do buffer
		tamanho_duracao = strlen(duracao);
		if(tamanho_duracao != 5)
			printf(ENTRADA_INVALIDA);
	} while (tamanho_duracao != 5);

	return duracao;
}

/* Lẽ apelido do mvp */
char *le_apelido_mvp() {
	char apelido_mvp[40];

	scanf("%39[\n]", apelido_mvp);
	getchar();	//Tira \n do buffer
	return apelido_mvp;
}

/* Gera a chave primária */
char *gera_chave_primaria() {

	/*
		composicao de letras maiusculas da primeira letra do nome da equipe azul, seguida da primeira letra do nome da equipe vermelha, seguida das duas primeiras letras do apelido do mvp, o dia e o mes da partida.

		Exemplo: FTFE0705
			Fnatic
			Team Mid-Solo
			Ferbien
			07/05
	*/
	
}

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

int main () {
	/*variaveis para arquivos, respectivamente: de dados, de indice primario, 
	de indice secundario para o nome da equipe vencedora e de indice secundario para apelido do MVP*/
	FILE *data_file, *index_file, *winner_index_file, *mvp_index_file;
	int opcao;

	do {
		scanf("%d", &opcao);

		switch(opc) {
			case INSERIR_PARTIDA:
				Partida novo;

				novo.equipe_azul = le_nome_equipe();
				novo.equipe_vermelha = le_nome_equipe();
				data_partida = le_data();
				novo.duracao = le_duracao();
				novo.equipe_vencedora = le_vencedora(); //Tratar (na função) se o nome é da azul/vermelha ou não!
				novo.placar_azul = le_placar();
				novo.placar_vermelha = le_placar();
				apelido_mvp = le_apelido_mvp();
				//GERAR CHAVE PRIMARIA, FUNÇÃO JÁ DECLARADA
			break;

			case REMOVER_PARTIDA:
			break;

			case MODIFICAR_DURACAO:
			break;

			case BUSCAR_PARTIDA:
			break;

			case LISTAR_PARTIDAS:
			break;

			case LIBERAR_ESPACO:
			break;
		}
	} while(opcao != 7)	//Segundo a descricao do problema, a opcao de finalizar eh a de numero 7


	return 0;
}

/*==================- IMPLEMENTAÇÃO DAS FUNÇÕES -==================*/

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
	
	scanf("%39[\n]", vencedora);
	getchar(); //Tira \n do buffer
	
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
	
}
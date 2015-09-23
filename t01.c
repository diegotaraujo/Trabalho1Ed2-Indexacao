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

//setar o tamanho das strings!<<<<<<<<<<<<<< um byte a mais que especificado no pdf para guardar o '\0' (6 bytes serão guardados como 5 no 
//arquivo quando excluirmos o '\0')
struct Partida {
	char equipe_azul[40];
	char equipe_vermelha[40];
	char equipe_vencedora[40];
	char apelido_mvp[40];
	char data_partida[11];	//ler como int ou char????????
	char duracao[6];
	char placar_azul[3];
	char placar_vermelho[3];
	char chave_primaria[9];
};

int main () {
	/*variaveis para arquivos, respectivamente: de dados, de indice primario, 
	de indice secundario para o nome da equipe vencedora e de indice secundario para apelido do MVP*/
	FILE *data_file, *index_file, *winner_index_file, *mvp_index_file;
	int opcao;

	do {
		scanf("%d", &opcao);

		switch(opc) {
			case INSERIR_PARTIDA:
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
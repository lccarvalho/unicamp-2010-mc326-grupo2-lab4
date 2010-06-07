/******************************************************************************/
/* Grupo 2:                                                                   */
/*          Felipe Augusto da Silva    RA 096993                              */
/*          Lucas Barbi Rezende        RA 097042                              */
/*          Luiz Claudio Carvalho      RA 800578                              */
/*                                                                            */
/* MC236EF  1o semestre 2010                                           UNICAMP*/
/* Laboratório 04 - B-Tree
   
/******************************************************************************/

/* btree.h - Arquivo header para manipulação da B-Tree */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "envelope.h"

#define tamPrimCampoHd 20
#define tamUltCampoHd 40
#define NIL     -1
#define NOKEY   '@'
#define MAXORDEM 10
#define TAMCHAVE 6



/* Definição de tipo booleano */
typedef enum {false, true} Boolean;


/* Definição de um vetor de ponteiros para caractere. Uma vez inicializados,
   cada um dos ponteiros do vetor irá apontar para a posição inicial do
   conteúdo do campo[i] do registro correspondente. Caso seja necessário
   carregar mais de um registro do banco de dados na memória, cada um de seus 
   campos poderá ser acessado por: registro[m].campo[n]   */
typedef char** Record;


/* Definição de estrutura que armazena as características de um campo de um
   arquivo de dados. Estas características devem ser extraídas de um arquivo de
   configuração ou de uma seção específica do arquivo de dados */
typedef struct {
        char nome[tamPrimCampoHd+1];
        char tipo;
        int inicio;
        int tamanho;
        Boolean obrig;
        char msg[tamUltCampoHd+1];
        } Header;
        

/* Estrutura das chaves */
typedef struct {
        char vrChave[TAMCHAVE];     /* chave */
        int  RRNrecord;      /* RRN do registro no arquivo de dados */
        } CHAVE;


/* Estrutura de um nó da Btree */
typedef struct {
        int keycount;       /* número de chaves no nó */
        CHAVE  key[MAXORDEM-1];      /* chaves */
        int child[MAXORDEM];      /* RRNs dos descendentes */
        } BTPAGE;

#define PAGESIZE sizeof(BTPAGE)



/*** Protótipos das Funções ***************************************************/

void AbreArquivoDados(char* nome, FILE** arqDados, FILE** arqCfg);
/* Abre o arquivo de dados e seu respectivo arquivo de configuração (.cfg) */

void CarregaHeader(Header** h, int* numcampos, FILE* arqCfg);
/* Carrega o vetor head com os campos do banco de dados definido por arqCfg */

Record LeRegistro(FILE* arq, int n, Header* h);
/* Retorna um registro com 'n' campos lido em 'arq'. */

void EscreveRegistro(Record rec, FILE* arq, int numcampos, Header* h);
/* Grava, na posição corrente em arq, os dados de rec */

void LiberaRegistro(Record registro, int n);
/* Libera todas as strings apontadas por record e também os apontadores */ 

void btread(int rrn, BTPAGE *page_ptr);
/* Lê página de número 'rrn' do arquivo de índices */

void btwrite(int rrn, BTPAGE *page_ptr);
/* Escreve página de número 'rrn' no arquivo de índices */

int create_root(CHAVE key, int left, int right, int ordem);
/* Cria a raiz da Btree, inserindo a chave 'key' */

int getpage();
/* Pega o próximo bloco disponível para uma nova página */

int getroot();
/* Lê RRN da raiz */

Boolean insert(int rrn, CHAVE key, int *promo_r_child, CHAVE *promo_key,
                                          int ordem, Boolean *duplic);
/* Função para inserir 'key' na Btree. Usa chamadas recursivas até atingir uma
   folha e então insere. Se o nó estiver cheio, chama split() para dividí-lo.
   Se as promoções ocorrerem até a raiz, coloca a chave do meio em 'promo_key'
   e seu filho direito em 'promo_r_child'. Se já tiver uma chave igual no nó,
   seta duplic como true. */

void ins_in_page(char key, int r_child, BTPAGE *p_page);
/* Insere key e r_child em p_page */

void pageinit(BTPAGE *p_page);
/* Inicializa uma página, colocando -1 em todos as chaves e NIL nos
   apontadores para os descendentes */

void putroot(int root);
/* Coloca RRN da raiz no inicio do arquivo de índices */

Boolean search_node(CHAVE key, BTPAGE *p_page, int *pos);
/* Retorna true se key está no nó ou false caso contrário. Em qualquer caso
   coloca a posição correta da chave em pos */

void split(CHAVE key, int r_child, BTPAGE *p_oldpage, CHAVE *promo_key,
                              int *promo_r_child, BTPAGE *p_newpage, int ordem);
/* Divide o nó criando um novo nó e passando metade das chaves para o novo nó.
   Promove a chave do meio e o RRN do novo nó. */                       



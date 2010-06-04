/******************************************************************************/
/* Grupo 2:                                                                   */
/*          Felipe Augusto da Silva    RA 096993                              */
/*          Lucas Barbi Rezende        RA 097042                              */
/*          Luiz Claudio Carvalho      RA 800578                              */
/*                                                                            */
/* MC236EF  1o semestre 2010                                           UNICAMP*/
/* LaboratÛrio 04 - B-Tree
   
/******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include "btree.h"

#define QTE_ARGUMENTOS 6
/* Chamada do programa:
       Btree <ordem Btree> < arqdados> <arqindice> <Arqdespreza> <Arqdesc>
   onde:
       - <Ordem Btree> - Ordem da Btree. Valor entre 3 e 10, inclusive extremos;
Å      - <Arqdados> - Nome do arquivo de dados;
Å      - <Arqindice> - Nome do arquivo de Ìndices;
Å      - <Arqdespreza> - Nome do arquivo com as chaves desprezadas;
Å      - <Arqdesc> - Nome do arquivo com a descriÁo da estrutura da ·rvore que
                                           permitir· reconstrui-l· graficamente.
*/


int main(int argc, char* argv[]) {
       
       /* vari·veis ***********************************************************/
       
       FILE *ArqDados,      /* arquivo de dados */
            *ArqCfg,        /* arquivo com a configuraÁ„o do arquivo de dados */
            *ArqIndices,    /* arquivo de indices */
            *ArqDespreza,   /* arquivo de chaves desprezadas */
            *ArqDesc;       /* arquivo com descriÁ„o da estrutura da Btree */
       
       Header *head;        /* configuraÁ„o do arquivo de dados */
       
       Record reg;          /* registro */
       
       CHAVE key,           /* chave a ser inserida na Btree */
             promo_key;     /* chave promovida */
       
       int ordem,              /* ordem da ·rvore */
           root,               /* RRN da raiz da Btree */
           numcampos,          /* n˙mero de campos de um registro */
           RRN_RegCorrente,    /* RRN do registro no arquivo de dados */
           promo_rrn;          /* RRN promovido */
       
       Boolean fim = false,      /* flag para loop do menu */
               duplic = false,   /* registro duplicado */
               promoted;         /* conta se houve promoÁ„o de baixo */
               
       
       /***********************************************************************/
        
        
       /* verifica quantidade de argumentos */
       if(argc != QTE_ARGUMENTOS) {
            printf("Erro argumentos\n");
            system("pause");
            exit (0);
       }
       
       ordem = atoi(argv[1]);
       
       /* a ordem deve ser entre 3 e 10 */
       if(ordem < 3 || ordem > 10){
            printf("Ordem da arvore invalida\n");
            system("pause");
            exit (0);
       }

       /* abre arquivo de dados junto com seu arquivo de configuraÁ„o */
       AbreArquivoDados(argv[2], &ArqDados, &ArqCfg);
       
       /* configuraÁ„o do arquivo de dados */                         
       CarregaHeader(&head, &numcampos, arqCfg);
    
       fclose(arqCfg);
       
       /* menu do programa */
       do {
         
         system("cls");
         printf("Lab04 - B-Tree\n\n");
         printf("1. Construir a B-Tree\n");
         printf("2. Gerar um arquivo com a estrutura da B-Tree\n");
         printf("3. Encerrar\n");
         
         printf("\nOpcao: ");
         scanf("%d", &opcao);
         
         /* verifica se È uma opÁ„o v·lida */
         while(!isdigit(opcao) || opcao < 1 || opcao > 3){
             printf("\nOpcao invalida\n\nOpcao: ");
             scanf("%d", &opcao);
         }
         
         switch(opcao){
             
             case 1:
                  
                  ArqIndices = Fopen(argv[3], "w+");
                  ArqDespreza = Fopen(argv[4], "w");
                  
                  RRN_RegCorrente = 0;
                  
                  reg = LeRegistro(ArqDados, numcampos, head);
                  
                  /* informaÁıes da chave */
                  key.RRNrecord = RRN_RegCorrente;
                  strcpy(key.vrChave, reg[0]);
                  
                  LiberaRegistro(reg, numcampos);
                  
                  root = create_root(key, NIL, NIL, ordem); /* cria raiz da Btree */
                  
                  RRN_RegCorrente++;
                  
                  
                  while(!feof(ArqDados)) {
                       
                       reg = LeRegistroFixo(ArqDados, numcampos, head);
                       
                       /* informaÁıes da chave */
                       key.RRNrecord = RRN_RegCorrente;
                       strcpy(key.vrChave, reg[0]);
                       
                       /* insere chave */
                       promoted = insert(root, key, &promo_rrn, &promo_key, ordem, &duplic);
                       
                       /* se j· contÈm a chave na Btree, escreve o registro em ArqDespreza */
                       if(duplic)
                           EscreveRegistro(reg, ArqDespreza, numcampos, head);
                       
                       /* promoÁ„o atÈ o primeiro nÌvel, cria nova raiz */
                       if(promoted)
                           root = create_root(promo_key, root, promo_rrn, ordem);
                       
                       LiberaRegistro(reg, numcampos);
                       
                       RRN_RegCorrente++;
                       
                  } /* while */
                  
                  printf("\nArquivo %s criado\n", argv[3]);
                  system("pause");
                  
                  fclose(ArqIndices);
                  fclose(ArqDespreza);
             break;
             
             case 2:
                  ArqDesc = Fopen(argv[5], "w");
                  
                  //LUCAS
                  
                  printf("\nArquivo %s criado\n", argv[5]);
                  system("pause");
                  
                  fclose(ArqDesc);
             break;
             
             case 3:
                  fim = true;
             break;
       
       } while (!fim);


       fclose(ArqDados);
       free(head);
       
       return 0;

} /* main */


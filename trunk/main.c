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
       
       short root,             /* RRN da raiz da Btree */
             promo_rrn;        /* RRN promovido */
             
       int ordem,              /* ordem da ·rvore */
           numcampos,          /* n˙mero de campos de um registro */
           RRN_RegCorrente,    /* RRN do registro no arquivo de dados */
           
           opcao;              /* opcao escolhida do menu */
           
       char leOpcao[10];       /* string para ler a opcao */
       
       Boolean fim = false,      /* flag para loop do menu */
               duplic = false,   /* registro duplicado */
               promoted;         /* conta se houve promoÁ„o de baixo */
       BTPAGE novo;        
               
       
       /***********************************************************************/
        
       /* verifica quantidade de argumentos */
       if(argc != QTE_ARGUMENTOS) {
            printf("Linha de comando:\n\n");
            printf("lab4 <ordem Btree> < arqdados> <arqindice> <Arqdespreza> <Arqdesc>\n\n");
//            printf("- <Ordem Btree> - Ordem da Btree(3 - 10);\n");
//Å           printf("- <Arqdados>    - Nome do arquivo de dados;\n");
//Å           printf("- <Arqindice>   - Nome do arquivo de indices;\n");
//Å           printf("- <Arqdespreza> - Nome do arquivo com as chaves desprezadas;\n");
//Å           printf("- <Arqdesc>     - Nome do arquivo com a descricao da estrutura da arvore\n");
//            printf("                  que permitira reconstrui-la graficamente.\n");
            exit (0);
       }
       
       ordem = atoi(argv[1]);
       
       /* a ordem deve ser entre 3 e 10 */
       if(ordem < 3 || ordem > 10){
            printf("Ordem da arvore invalida\n");
            exit (0);
       }

       /* abre arquivo de dados junto com seu arquivo de configuraÁ„o */
       AbreArquivoDados(argv[2], &ArqDados, &ArqCfg);
       
       /* configuraÁ„o do arquivo de dados */                         
       CarregaHeader(&head, &numcampos, ArqCfg);
    
       fclose(ArqCfg);
       
       /* menu do programa */
       do {
         
         system("cls");
         printf("Lab04 - B-Tree\n\n");
         printf("1. Construir a B-Tree\n");
         printf("2. Gerar um arquivo com a estrutura da B-Tree\n");
         printf("3. Encerrar\n");
         
         printf("\nOpcao: ");
         scanf("%s", &leOpcao);
         opcao = atoi(leOpcao);
         
         /* verifica se È uma opÁ„o v·lida */
         while(opcao < 1 || opcao > 3){
             printf("\nOpcao invalida\n\nOpcao: ");
             scanf("%s", &leOpcao);
             opcao = atoi(leOpcao);
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
                 
                  root = create_root(key, NIL, NIL, ArqIndices); /* cria raiz da Btree */

                  RRN_RegCorrente++;
                  
                  
                  while((reg = LeRegistro(ArqDados, numcampos, head)) != NULL) {
                       
                       /* informaÁıes da chave */
                       key.RRNrecord = RRN_RegCorrente;
                       strcpy(key.vrChave, reg[0]);


printf("\n******************Inserindo agora: [%d] %s*************************\n", RRN_RegCorrente, key.vrChave);
system("pause");

                       /* insere chave */
                       promoted = insert(root, key, &promo_rrn, &promo_key,
                                                    ordem, &duplic, ArqIndices);

                       /* se j· contÈm a chave na Btree, escreve o registro em ArqDespreza */
                       if(duplic) {
                           EscreveRegistro(reg, ArqDespreza, numcampos, head);
                           duplic = false;
                       }
                      
                       /* promoÁ„o atÈ o primeiro nÌvel, cria nova raiz */
                       if(promoted) {
                           root = create_root(promo_key, root, promo_rrn,
                                                                    ArqIndices);
printf("\nNOVA RAIZ CRIADA\n");                                                                    
                       }
      
                       LiberaRegistro(reg, numcampos);
                       
                       RRN_RegCorrente++;

                  } /* while */
                  
                  fclose(ArqIndices);
                  fclose(ArqDespreza);
                  
                  printf("\nArquivo %s criado\n\n", argv[3]);
                  system("pause");

             break;
             
             case 2:
                  
                   
                   
                  ArqDesc = Fopen(argv[5], "w");  //abre os arquivos
                  ArqIndices = Fopen(argv[3], "r");

                  short raiz;
                  fread(&raiz, 1, 2, ArqIndices);  //pega a raiz

                  criar_saida(&novo, ArqIndices, ArqDesc, 1, raiz);   //cria o arquivo de saida

                  fclose(ArqDesc);             //fecha os arquivos
                  fclose(ArqIndices);
                  
             break;
             
             case 3:
                  fim = true;
             break;
         } /* switch */
       
       } while (!fim);


       fclose(ArqDados);
       free(head);
       
       return 0;

} /* main */


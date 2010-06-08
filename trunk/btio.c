/******************************************************************************/
/* Grupo 2:                                                                   */
/*          Felipe Augusto da Silva    RA 096993                              */
/*          Lucas Barbi Rezende        RA 097042                              */
/*          Luiz Claudio Carvalho      RA 800578                              */
/*                                                                            */
/* MC236EF  1o semestre 2010                                           UNICAMP*/
/* Laboratório 04 - B-Tree
   
/******************************************************************************/



#include "btree.h"


void AbreArquivoDados(char* nome, FILE** arqDados, FILE** arqCfg){
/* Abre o arquivo de dados e seu respectivo arquivo de configuração (.cfg) */ 

      *arqDados = Fopen(nome, "r");    /* Abre arquivo de dados */
   
      /* abre arquivo de configuração (mesmo nome com extensão .cfg) */
      nome[strlen(nome)-3] = '\0';
      *arqCfg = Fopen(strcat(nome, "cfg"), "r");

} /* AbreArquivoDados */


void TiraBrancosDoFinal(char* s){
/* Elimina todos os brancos em excesso no final de uma string. */
    
    int i = strlen(s) - 1;     //Último elemento da string
    while(i >= 0 && s[i] == ' ')
       i--;
    s[i+1] = '\0';
    
} /* TiraBrancosDoFinal */


void CarregaHeader(Header** h, int* numcampos, FILE* arqCfg){
/* Carrega o vetor head com os campos do banco de dados definido por arqCfg */

    int i, final;
    char c;
    
    fscanf(arqCfg, "%d", numcampos);     /* Le número de campos */
    fseek(arqCfg, 2, SEEK_CUR);
    *h = Malloc(sizeof(Header)*(*numcampos));     /* Aloca o vetor head */
    
    for(i = 0; i < *numcampos; i++) {
        
        /* Le nome */
        fread((*h+i)->nome, tamPrimCampoHd, 1, arqCfg);
        (*h+i)->nome[tamPrimCampoHd] = '\0';
        TiraBrancosDoFinal((*h+i)->nome);
    
        /* Le tipo */
        (*h+i)->tipo = fgetc(arqCfg);
        
        /* Le endereço de inicio e final de um campo e calcula seu tamanho */
        fscanf(arqCfg, "%d %d", &((*h+i)->inicio), &final);
        (*h+i)->tamanho = final - (*h+i)->inicio + 1;
        
        /* Le caractere de obrigatoriedade do campo */
        fseek(arqCfg, 1, SEEK_CUR);
        c = fgetc(arqCfg);
        if(c == 'S')
            (*h+i)->obrig = true;
        else
            (*h+i)->obrig = false;
            
        /* Le mensagem */    
        fseek(arqCfg, 1, SEEK_CUR);
        fread((*h+i)->msg, tamUltCampoHd, 1, arqCfg);
        (*h+i)->msg[tamUltCampoHd] = '\0';
        TiraBrancosDoFinal((*h+i)->msg);
    
        fseek(arqCfg, 2, SEEK_CUR);
    }
    
} /* CarregaHeader */


Record LeRegistro(FILE* arq, int n, Header* h) {
/* Retorna um registro com 'n' campos lido em 'arq'. */
   
   int i;
   Record registro;
   
   registro = Malloc(sizeof(char*)*n);
   
   for(i=0;i<n;i++){
                     
          registro[i] = (char*)Malloc(sizeof(char)*(h[i].tamanho+1));
          fread(registro[i], h[i].tamanho, 1, arq);
          registro[i][h[i].tamanho] = '\0';
   }
   
   return registro;
   
}/* LeRegistroFixo */


void EscreveRegistro(Record rec, FILE* arq, int numcampos, Header* h){
/* Grava, na posição corrente em arq, os dados de rec. */

   int i, tamreg;
   char *linha;
   
   /* tamanho de um registro do arquivo de entrada */
   tamreg = h[numcampos-1].inicio + h[numcampos-1].tamanho;
   
   linha = Malloc(sizeof(char)*(tamreg+1));
   
   linha[0] = '\0';
   
   /* junta todos os campos do registros em um unico bloco de memoria
      para dar apenas um fwrite */
   
   for(i = 0; i < numcampos; i++) {
         
         strcat(linha, rec[i]);
         
         if((i < numcampos-1) && (h[i+1].inicio > h[i].inicio + h[i].tamanho)) //campos com um '' no final
             strcat(linha, " ");
         
   }
   
   strcat(linha, "\n");
   
   fwrite(linha, tamreg, 1, arq);
   
   free(linha);

} /* ImprimeRegFixo */


void LiberaRegistro(Record registro, int n){
/* Libera todas as strings apontadas por record e também os apontadores */
     
     int i;
     for(i = 0; i < n; i++)
        free(registro[i]);
     free(registro);
     
} /* LiberaRegistro */


void putroot(int root, FILE* btfd) {
      fseek(btfd, 0L, SEEK_SET);
      fwrite(&root,1,sizeof(int),btfd);
      fflush(btfd);      
}        

int getpage(FILE* btfd) {
      long  addr;
      fseek(btfd, 0, SEEK_END);
      addr = ftell(btfd) - 2L;
      return ((int) addr / (int) PAGESIZE);
}

void btread(int rrn, BTPAGE *page_ptr, FILE* btfd) {
      long addr;
      addr = (long) rrn * (long) PAGESIZE + 2L;
      fseek(btfd, addr, SEEK_SET);
      fread(page_ptr, 1, PAGESIZE, btfd);
      fflush(stdin);      
}

void btwrite(int rrn, BTPAGE *page_ptr, FILE* btfd) {
      long addr;
      addr = (long) rrn * (long) PAGESIZE + 2L;
      fseek(btfd, addr, SEEK_SET);
      fwrite(page_ptr, 1, PAGESIZE, btfd);
      fflush(btfd);
      return;
}

/******************************************************************************/
/* Grupo 2:                                                                   */
/*          Felipe Augusto da Silva    RA 096993                              */
/*          Lucas Barbi Rezende        RA 097042                              */
/*          Luiz Claudio Carvalho      RA 800578                              */
/*                                                                            */
/* MC236EF  1o semestre 2010                                           UNICAMP*/
/* Laboratório 04 - B-Tree
   
/******************************************************************************/

/*insert.c...
             Contains insert() function to insert a key to a btree.
     Calls itself recursively until bottom of tree is reached.
     Then iserts key in node.
     If node is out of room,
        - calls split() to split the node
        - promotes middle key and rrn of new node
*/        

#include "btree.h"

/* insert()...
Arguments:
          rrn:                rrn of page to make insertion in
          *promo_r_child:     child promoted up from here to next level
          key:                key to be inserted here or lower
          *promo_key:         key promoted up from here to next level
*/
Boolean insert(short rrn, CHAVE key, short *promo_r_child, CHAVE* promo_key,
                                       int ordem, Boolean *duplic, FILE* btfd){

int i;
                                           
             BTPAGE page,           /* current page                           */
                    newpage;        /* new page created if split occurs       */
             int found, promoted;   /* boolean values                         */
             short pos,
                   p_b_rrn;         /* rrn promoted from below                */
             CHAVE  p_b_key;        /* key promoted from below                */
             
             if(rrn == NIL) {                                                 /* past bottom of tree... "promote" */
                    memmove(promo_key, &key, sizeof(CHAVE));                  /* original key so that it will be  */ 
                    *promo_r_child = NIL;                                     /* inserted at leaf level           */
                    return true;
             }
             
             
             
             btread(rrn, &page, btfd);
             found = search_node(key, &page, &pos);
             
             if(found) {
                 *duplic = true;
                 return false;
             }
             
           
             promoted = insert(page.child[pos], key, &p_b_rrn, &p_b_key,
                                                           ordem, duplic, btfd);
                                                           
             if(!promoted) {
                 return false;                                 /* no promotion */
             }

printf("\n***Em INSERT - vai fazer insercao em:\n");
for(i=0; i < page.keycount; i++) printf("[%d] %s ", i, page.key[i].vrChave);
printf("\nKeyCount: %d\n\n", i);
//system("pause");

             if(page.keycount < (ordem-1)) {
                 ins_in_page(p_b_key, p_b_rrn, &page);         /*OK to insert key */
                 btwrite(rrn, &page, btfd);                    /*and pointer in this page*/             
                 return false;                                 /* no promotion */
             }

             else {
                 split(p_b_key, p_b_rrn, &page, promo_key, promo_r_child, 
                                                         &newpage, ordem, btfd); 
                 btwrite(rrn, &page, btfd);
                 btwrite(*promo_r_child, &newpage, btfd);
                 return true;                                /* promotion */
             }       
} /* insert */


void criar_saida(BTPAGE *no ,FILE* indice, FILE* novo, int nivel, short vno){
   
   int i,j, filhos[10], chaves;
   
   btread(vno, no,indice);  //pega o no
   chaves = no->keycount;  //pega o numero de chaves
   
   if (nivel==1) chaves=1;
   
   for(j=0;j<nivel-1;j++) fprintf(novo, "     ");
 
//escreve os detalhes do no
   fprintf(novo, "No %d: Total de chaves: %d ", vno, no->keycount);
        
//passa por todas as chaves do no e escreve no arquivo        
   for(i=0;i<chaves;i++){         
          fprintf(novo, "(%d,%s),", no->child[i], no->key[i].vrChave);
          filhos[i]=no->child[i];
        
   }
   filhos[i]=no->child[i];
   fprintf(novo,"(%d,)", no->child[i]);
   fprintf(novo, "\n");

// se existir filhos faz a chamad recursiva, passando por todos os filhos
   if (filhos[0]>-1){
   for(i=0;i<chaves+1;i++){
           btread(filhos[i], no, indice);             
           criar_saida(no, indice, novo, nivel+1, filhos[i]);   
   }
   }                  



}                  

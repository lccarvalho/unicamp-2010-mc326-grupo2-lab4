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
Boolean insert(short rrn, CHAVE key, int *promo_r_child, CHAVE* promo_key,
                                       int ordem, Boolean *duplic, FILE* btfd){
                                           
//printf("Entrou em INSERT\n");                                           
                                           
//int insert(int rrn, char key, int *promo_r_child, char *promo_key) {
             BTPAGE page,           /* current page                           */
                    newpage;        /* new page created if split occurs       */
             int found, promoted;   /* boolean values                         */
             int pos,
                   p_b_rrn;         /* rrn promoted from below                */
             CHAVE  p_b_key;         /* key promoted from below                */
             
             if(rrn == NIL) {                               /* past bottom of tree... "promote" */
             
//printf("INSERT - Passou da folha e vai promover %d\n", key.RRNrecord);             
             
                    memmove(promo_key, &key, sizeof(CHAVE));/* original key so that it will be  */ 
                    *promo_r_child = NIL;                   /* inserted at leaf level           */
                    return true;
             }
             
             
             
             btread(rrn, &page, btfd);
             
//printf("Leu btpage - keycount=%d\n", page.keycount);
             
             found = search_node(key, &page, &pos);


//printf("Voltou de search_node: pos=%d\n", pos);
             
             
             if(found) {
                 *duplic = true;
                 
//printf("INSERT - Achou registro duplicado\n");                 
                 
                 return false;
             }
             
           
             promoted = insert(page.child[pos], key, &p_b_rrn, &p_b_key,
                                                           ordem, duplic, btfd);
                                                           
//printf("INSERT - voltou de insert; keycount=%d, ordem-1=%d\n", page.keycount, ordem-1);                                                            
//printf("                           promoted=%d\n", promoted); 
                                                           
             if(!promoted) {
             
//printf("INSERT - Not promoted\n");             
             
                 return false;                                 /* no promotion */
}
             if(page.keycount < (ordem-1)) {
                 ins_in_page(p_b_key, p_b_rrn, &page);         /*OK to insert key */
                 
//printf("INSERT - Inseriu in page\n");                 
                 
                 btwrite(rrn, &page, btfd);                    /*and pointer in this page*/             
                 return false;                                 /* no promotion */
             }
             else {
                 split(p_b_key, p_b_rrn, &page, promo_key, promo_r_child, 
                                                         &newpage, ordem, btfd); 
                                                         
printf("INSERT - Voltou de split\n");                                                         
                                                         
                 btwrite(rrn, &page, btfd);
                 btwrite(*promo_r_child, &newpage, btfd);
                 return true;                                /* promotion */
             }       
}

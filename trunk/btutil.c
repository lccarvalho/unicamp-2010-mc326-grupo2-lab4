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


short create_root(CHAVE key, short left, short right, FILE* btfd) {
           BTPAGE page;
           short rrn;
           rrn = getpage(btfd);
           pageinit(&page);
           page.key[0] = key;
           page.child[0] = left;
           page.child[1] = right;
           page.keycount = 1;
           btwrite(rrn, &page, btfd);
           putroot(rrn, btfd);
           return (rrn);
} /* create_root */



void pageinit(BTPAGE *p_page) {        /* p_page: pointer to a page  */
           int j;
           for(j = 0; j < MAXORDEM; j++) {
                 memset (p_page->key[j].vrChave, NOKEY, TAMCHAVE); 
                 p_page->child[j] = NIL;      
           }
           p_page->child[j] = NIL;      
           
} /* pageinit */



Boolean search_node(CHAVE key, BTPAGE *p_page, short *pos) {
                 /* pos: position where key is or should be inserted */
           
           int i;
           for(i=0; (i < p_page->keycount) && (strcmp(key.vrChave, p_page->key[i].vrChave) > 0); i++);

           *pos = i;
           
           if(*pos < p_page->keycount && strcmp(key.vrChave, p_page->key[*pos].vrChave)==0)
                   return true;        /* key in in the page */

           else return false;          /* key in not in the page */
} /* search_node */



void ins_in_page(CHAVE key, short r_child, BTPAGE *p_page) {
           int i;
           for(i = p_page->keycount; (strcmp(key.vrChave, p_page->key[i-1].vrChave) < 0) && i > 0; i--){
                 memmove(&p_page->key[i], &p_page->key[i-1], sizeof(CHAVE));
                 p_page->child[i+1] = p_page->child[i];
           }
           p_page->keycount++;
           memmove(&p_page->key[i], &key, sizeof(CHAVE));
           p_page->child[i+1] = r_child;
           
printf("\n***Em ins_in_page: Chave inserida=%s\n",key.vrChave);
for(i=0; i < p_page->keycount; i++) printf("[%d] %s ", i, &p_page->key[i].vrChave);
printf("\nKeyCount: %d\n\n", i);
//printf("Filhos:");
//for(i=0; i < p_page->keycount+2; i++) printf("[%d] %d ", i, &p_page->child[i]);
//system("pause");
           
} /* ins_in_page */

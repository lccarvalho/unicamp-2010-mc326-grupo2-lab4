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


int create_root(CHAVE key, int left, int right, FILE* btfd) {
           BTPAGE page;
           int rrn;
           rrn = getpage(btfd);
           pageinit(&page);
           page.key[0] = key;
           page.child[0] = left;
           page.child[1] = right;
           page.keycount = 1;
           btwrite(rrn, &page, btfd);
           putroot(rrn, btfd);
           return (rrn);
}

void pageinit(BTPAGE *p_page) {        /* p_page: pointer to a page  */
           int j;
           for(j = 0; j < MAXORDEM; j++) {
                 memset (p_page->key[j].vrChave, NOKEY, TAMCHAVE); 
                 p_page->child[j] = NIL;      
           }
           p_page->child[j] = NIL;      
}

Boolean search_node(CHAVE key, BTPAGE *p_page, int *pos) {
                 /* pos: position where key is or should be inserted */
           
           int i;
           for(i=0; i < p_page->keycount && (strcmp(key.vrChave, p_page->key[i].vrChave) > 0); i++);         //TROCAR POR STRCOMP/CMP
           *pos = i;
           
           if(*pos < p_page->keycount && strcmp(key.vrChave, p_page->key[*pos].vrChave)==0)              //TROCAR POR STRCOMP/CMP
                   return true;      /* key in in the page */
           else return false;          /* key in not in the page */
}

void ins_in_page(CHAVE key, int r_child, BTPAGE *p_page) {
           int i;
           for(i = p_page->keycount; (strcmp(key.vrChave, p_page->key[i-1].vrChave) < 0) && i > 0; i--){
                 memmove(&p_page->key[i], &p_page->key[i-1], sizeof(CHAVE));
                 memmove(&p_page->child[i+1], &p_page->child[i], sizeof(CHAVE));
           }
           p_page->keycount++;
           memmove(&p_page->key[i], &key, sizeof(CHAVE));
           p_page->child[i+1] = r_child;
}

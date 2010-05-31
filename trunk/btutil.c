/* btutil.c...
   Contains utility functions for btree program:
            
   create_root() -- get and initialize root node and insert one key
   pageinit()    -- put NOKEY in all "key" slots and NIL in "child" slots
   search_node() -- return YES if key in node, else NO. In either case, put
                    key's correct position in pos.
   ins_in_page() -- insert key and right child in page
*/
#include "bt.h"

create_root(char key, short left, short right) {
           BTPAGE page;
           short rrn;
           rrn = getpage();
printf("rrn in create_root: %d\n", rrn);
fflush(stdin);
           pageinit(&page);
           page.key[0] = key;
           page.child[0] = left;
           page.child[1] = right;
           page.keycount = 1;
           btwrite(rrn, &page);
           putroot(rrn);
           return (rrn);
}

pageinit(BTPAGE *p_page) {        /* p_page: pointer to a page  */
           int j;
           for(j = 0; j < MAXKEYS; j++) {
                 p_page->key[j] = NOKEY;
                 p_page->child[j] = NIL;      
           }
           p_page->child[j] = NIL;      
}

search_node(char key, BTPAGE *p_page, short *pos) {
                 /* pos: position where key is or should be inserted */
           
           int i;
           for(i=0; i < p_page->keycount && key > p_page->key[i]; i++);
           *pos = i;
           
           if(*pos < p_page->keycount && key == p_page->key[*pos])
                   return (YES);      /* key in in the page */
           else return (NO);          /* key in not in the page */
}

ins_in_page(char key, short r_child, BTPAGE *p_page) {
           int i;
           for(i = p_page->keycount; key < p_page->key[i-1] && i > 0; i--){
                 p_page->key[i] = p_page->key[i-1];
                 p_page->child[i+1] = p_page->child[i];
           }
           p_page->keycount++;
           p_page->key[i] = key;
           p_page->child[i+1] = r_child;
}

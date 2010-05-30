/*insert.c...
             Contains insert() function to insert a key to a btree.
     Calls itself recursively until bottom of tree is reached.
     Then iserts key in node.
     If node is out of room,
        - calls split() to split the node
        - promotes middle key and rrn of new node
*/        
#include "bt.h"

/* insert()...
Arguments:
          rrn:                rrn of page to make insertion in
          *promo_r_child:     child promoted up from here to next level
          key:                key to be inserted here or lower
          *promo_key:         key promoted up from here to next level
*/

insert(short rrn, char key, short *promo_r_child, char *promo_key) {
             BTPAGE page,           /* current page                           */
                    newpage;        /* new page created if split occurs       */
             int found, promoted;   /* boolean values                         */
             short pos,
                   p_b_rrn;         /* rrn promoted from below                */
             char  p_b_key;         /* key promoted from below                */
             
             if(rrn == NIL) {             /* past bottom of tree... "promote" */
                    *promo_key = key;     /* original key so that it will be  */
                    *promo_r_child = NIL; /* inserted at leaf level           */
                    return (YES);
             }
             btread(rrn, &page);
             found = search_node(key, &page, &pos);
             if(found) {
                 printf("Error: attempt to insert duplicate key:%c\n\007", key);
                 return (0);
             }
             promoted = insert(page.child[pos], key, &p_b_rrn, &p_b_key);
             if(!promoted)
                 return (NO);                                 /* no promotion */
             if(page.keycount < MAXKEYS) {
                 ins_in_page(p_b_key, p_b_rrn, &page);     /*OK to insert key */
                 btwrite(rrn, &page);               /*and pointer in this page*/             
                 return (NO);                                 /* no promotion */
             }
             else {
                 split(p_b_key, p_b_rrn, &page, promo_key, promo_r_child, 
                                                                     &newpage); 
                 btwrite(rrn, &page);
                 btwrite(*promo_r_child, &newpage);
                 return (YES);                                /* promotion */
             }       
}

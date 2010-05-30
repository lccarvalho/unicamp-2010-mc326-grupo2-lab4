/* btutil.c...
   Contains split() function for btree program:
            
          -- split node by creating new node and moving half of the keys
                    to new node. Promote middle key and rrn of new node.
                    
          Arguments:
              key:               key to be inserted
              promo_key:         key to be promoted up from here
              r_child:           child rrn to be inserted
              promo_r_child:     rrn to be promoted up from here
              p_oldpage          pointer to old page structure
              p_newpage          pointer to new page structure                       
*/
#include "bt.h"

split(char key, short r_child, BTPAGE *p_oldpage, char *promo_key,
                                 short *promo_r_child, BTPAGE *p_newpage) {

      int i;
      short mid;                  /* tells where split is to occur            */
      char workkeys[MAXKEYS+1];   /* temporarily holds keys, before split     */
      char workch[MAXKEYS+2];     /* temporarily holds children, before split */
      
      for(i=0; i < MAXKEYS; i++) {              /* move keys and children from*/
               workkeys[i] = p_oldpage->key[i]; /*old page into work arrays   */
               workch[i] = p_oldpage->child[i];
      }
      workch[i] = p_oldpage->child[i];
      for(i = MAXKEYS; key < workkeys[i-1] && i > 0; i--){ /* insert new key  */
               workkeys[i] = workkeys[i-1];
               workch[i+1] = workch[i];
      }
      workkeys[i] = key;
      workch[i+1] = r_child;
      
      *promo_r_child = getpage();           /* create new page for split      */
      pageinit(p_newpage);                  /* and promote rrn of new page    */
      
      for(i=0; i < MINKEYS; i++) {               /* move first half of keys   */
               p_oldpage->key[i] = workkeys[i];  /* and children to old page, */
               p_oldpage->child[i] = workch[i];  /* second half to new page   */
               p_newpage->key[i] = workkeys[i+1+MINKEYS];
               p_newpage->child[i] = workch[i+1+MINKEYS];
               p_oldpage->key[i+MINKEYS] = NOKEY;      /* mark second half of */
               p_oldpage->child[i+1+MINKEYS] = NIL;    /* old page as empty   */
      }
      p_oldpage->child[MINKEYS] = workch[MINKEYS];
      p_newpage->child[MINKEYS] = workch[i+1+MINKEYS];
      p_newpage->keycount = MAXKEYS - MINKEYS;
      p_oldpage->keycount = MINKEYS;      
      *promo_key = workkeys[MINKEYS];                  /* promote middle key   */
}

/* driver.c...
              Driver for btree tests
              
              Opens or creates b-tree file
              Gets next key and calls insert to insert key in tree
              If necessary creates a new root.
*/
#include <stdio.h>
#include <stdlib.h>
#include "bt.h"

main() {
       
       int promoted;              /* boolean: tells if a promotion from below */
       short root,                /* RRN of root page                         */
             promo_rrn;           /* RRN promoted from below                  */
       char promo_key,            /* key promoted from below                  */
                     key;         /* next key to insert in tree               */
                     
//       if(btopen()){               /* try to open btree.dat and get root       */
//printf("btopen retornou true\n");       
//             root = getroot();  
//printf("root in main: %d\n", root);                      
//}
//       else                       /* if btree.dat not there, create it        */
             root = create_tree();                     
printf("root in main: %d\n", root);        
       while((key = getchar()) != 'q') {
             promoted = insert(root, key, &promo_rrn, &promo_key);
             if(promoted)
                   root = create_root(promo_key, root, promo_rrn);
       }
       btclose();
}

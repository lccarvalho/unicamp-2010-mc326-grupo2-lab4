/* btio.c...
   Contains btree functions that directly involve file i/o:
            
   btopen()      -- open file "btree.dat" to hold the btree.
   btclose()     -- close "btree.dat".
   getroot()     -- get rrn of root node from first two bytes of btree.dat
   putroot()     -- put rrn of root node in first two bytes of btree.dat
   create_tree() -- create "btree.dat" and root node
   getpage()     -- get next available block in "btree.dat" for a new page
   btread()      -- read page number rrn from "btree.dat"
   btwrite()     -- write page number rrn to "btree.dat"
*/
#include <stdio.h>
#include "bt.h"
#include "fileio.h"

//int btfd;           /* global file descriptor for "btree.dat"                 */
FILE* btfd;

btopen() {
//      btfd = open("btree.dat", O_RDWR);
      btfd = fopen("btree.dat", "w+");  
//      return (btfd > 0);
      return (btfd != NULL);  
}

btclose() {
//      close(btfd);
fclose(btfd);
}

short getroot() {
      short root;
      long lseek();
      
      lseek(btfd, 0L, 0);
      if(read(btfd, &root, 2) == 0){ 
            printf("Error: Unable to get root.\007\n");
            exit(1);
      }
      return (root);
}

putroot(short root) {
      fseek(btfd, 0L, SEEK_SET);
      fwrite(&root,1,sizeof(short),btfd);
      fflush(btfd);      
}        

short create_tree() {
      char key;
      
//      btfd = creat("btree.dat", PMODE);
//      close(btfd);                     /* Have to close and reopen to insure  */
//      btopen();                        /* read/write access on many systems   */
btfd = fopen("btree.dat", "w+t");
      key = getchar();                 /* gGet first key                      */
      return(create_root(key, NIL, NIL));
}

short getpage() {
      long  addr;
      fseek(btfd, 0, SEEK_END);
      
      addr = ftell(btfd) - 2L;
printf("addr: %d  PAGESIZE: %d\n", addr, PAGESIZE);
      
      return ((short) addr / (short) PAGESIZE);
}

btread(short rrn, BTPAGE *page_ptr) {
      long addr;
      addr = (long) rrn * (long) PAGESIZE + 2L;
      fseek(btfd, addr, SEEK_SET);
      fread(page_ptr, 1, PAGESIZE, btfd);
}

btwrite(short rrn, BTPAGE *page_ptr) {
      long addr;
      addr = (long) rrn * (long) PAGESIZE + 2L;
      fseek(btfd, addr, SEEK_SET);
      fwrite(page_ptr, 1, PAGESIZE, btfd);
      fflush(btfd);
      return;
}


      
      
      

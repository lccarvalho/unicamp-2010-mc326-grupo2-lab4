/* bt.h...
          header file for btree programs
*/
#include <stdio.h>
#include <stdlib.h>

#define MAXKEYS 4
#define MINKEYS MAXKEYS/2
#define NIL     -1
#define NOKEY   '@'
#define NO      0
#define YES     1

//typedef enum (false, true) Boolean;

typedef struct {
        short keycount;             /* number of keys in page                 */
        char  key[MAXKEYS];         /* the actual keys                        */
        short child[MAXKEYS+1];     /* ptrs to RRNs of descendants            */
} BTPAGE;

#define PAGESIZE sizeof(BTPAGE)


extern short root;                 /* RRN of root page                        */
extern FILE* btfd;                 /* file descriptor of btree file           */
extern int infd;                   /* file descriptor of input file           */

/* prototypes */
void btclose(void);
int btopen(void);
void btread(short rrn, BTPAGE *page_ptr);
void btwrite(short rrn, BTPAGE *page_ptr);
short create_root(char key, short left, short right);
short create_tree();
short getpage();
short getroot();
int insert(short rrn, char key, short *promo_r_child, char *promo_key);
void ins_in_page(char key, short r_child, BTPAGE *p_page);
void pageinit(BTPAGE *p_page);
void putroot(short root);
int search_node(char key, BTPAGE *p_page, short *pos);
void split(char key, short r_child, BTPAGE *p_oldpage, char *promo_key,
                               short *promo_r_child, BTPAGE *p_newpage);

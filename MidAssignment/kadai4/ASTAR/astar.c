/* A star search */
#include <stdio.h>
#include <stdlib.h>

typedef struct BOARD {
    char cell[9];
    struct BOARD *next;
    struct BOARD *back;
    int depth,f;
} BOARD;

BOARD B0 = {
/*    2,3,5,7,1,6,4,8,0,NULL,NULL,0,0  */
/* problem 1 */
    1,5,2,4,0,3,7,8,6,NULL,NULL,0,0
/* problem 2 */
/*    1,2,0,4,5,3,7,8,6,NULL,NULL,0,0  */
};

BOARD BG = {
    1,2,3,4,5,6,7,8,0
};

BOARD *q0 = NULL, *q2 = &B0;

BOARD *getq() {
    BOARD *q; 
    if (q2==NULL) return NULL;
    q = q2;
    q2 = q2->next;
    return (q);
}

int nc = 0;
int ns = 0;

#define FN1 "Init.dat"
#define FN2 "Goal.dat"

void readdata(struct BOARD *b, char *buf);
void expand(struct BOARD *b);
void exchange(struct BOARD *b,int i,int j);
int equal(struct BOARD *b1, struct BOARD *b2);
void traceback(struct BOARD *b);
void printboard(struct BOARD *b);
void bornprint(struct BOARD *b);
void printQ(BOARD *b);
void putq(struct BOARD *b);
void f_value(BOARD *b);

int main(){
    BOARD *b;
    BOARD *q1;

    readdata(&B0,FN1);
    readdata(&BG,FN2);

    B0.depth = 1;

    printf("Initial state is \n");
    printboard(&B0);
    printf("Goal state is \n");
    printboard(&BG);
    printf("Execution begins..\n");

    
    f_value(&B0);

    while ((q1 = getq()) != NULL) {
	    expand(q1);
	    if (q0==NULL) q0 = q1;
	    else {
	        q1 -> next = q0;
	        q0 = q1;
	    }
    /*	printf("Q0 is ...\n");
	    printQ(q0);
	    printf("Q2 is ...\n");
	    printQ(q2);
    */
        }
}


void readdata(BOARD *b, char *buf){
    FILE *fn;
    int num[9],k;
  
    if ((fn = fopen(buf,"r")) == (FILE *)NULL) return;
    fscanf(fn,"%1d %1d %1d %1d %1d %1d %1d %1d %1d",
	   num,num+1,num+2,num+3,num+4,num+5,
	   num+6,num+7,num+8);

    for (k=0;k<9;k++) b->cell[k] = *(num+k);

    fclose(fn);
}


int diff(BOARD* b1,BOARD* b2){
    int ans=0,k;
    for (k=0;k<9;k++) if(b1->cell[k] != b2->cell[k]) ans++;
    return(ans);
}


void f_value(BOARD *b){
    b->f = b->depth + diff(b,&BG);
}


void exchange(BOARD *b, int i,int j){
    BOARD *m;
    int k;
    m=(struct BOARD*)malloc (sizeof(struct BOARD));
    if (m==NULL) {
	printf("memory overflow\n");
	exit(2);
    }
    for (k=0;k<9;k++) m-> cell[k] = b->cell[k];
    m-> cell[i] = b->cell[j];
    m-> cell[j] = 0;
    m-> back = b;
    m-> next = NULL;
    m-> depth = b -> depth + 1;
    if (equal(m,&BG)) { 
	    f_value(b);
	    bornprint(m);
	    printf("**** Answer found... ****\n");
	    traceback(m); 
	    printf("No. of children is %d\n",nc);
	    printf("Length of solution is %d\n",ns);
	    exit(0); 
    }
    putq(m);
}


void expand(BOARD *b){
    register int i;
    for (i=0; i<9; i++)
	if (b->cell[i]==0) break;
    if (i%3>0) exchange(b,i,i-1);
    if (i>2) exchange(b,i,i-3);
    if (i%3<2) exchange(b,i,i+1);
    if (i<6) exchange(b,i,i+3);
}


int equal(BOARD *b1,BOARD *b2){
    register int i;
    for (i=0; i<9; i++) 
	if (b1->cell[i]!=b2->cell[i]) return 0;
    return 1;
}

void traceback(BOARD *b){
    for (; b!=NULL; b=b->back) {
	ns++;
	printboard(b);
    }
}

void printboard(BOARD *b){
    int i;
    for (i=0;i<9;i++) {
	    if (b->cell[i]==0) putchar('X');
	    else printf("%1d",b->cell[i]);
	    if (i%3==2) {
	        putchar('\n');
	        if (i!=8) {
		    putchar('-');
		    putchar('+');
		    putchar('-');
		    putchar('+');
		    putchar('-');
		    putchar('\n');
	        }
	    }
	    else putchar('|');
    }
    putchar('\n');
}


void printQ(BOARD *b){
    if (b==NULL) return;
    printboard(b);
    printf(" %d ==> \n",b->f);
    printQ(b->next);
}


void bornprint(BOARD *b){
    nc++;
    printboard(b->back);
    printf(" %d ----> %d \n",b->back->f,b->f);
    printboard(b);
   printf("\n");
}

void putq(BOARD *b){
    BOARD *n,*oldn=NULL; 
    for (n=q0; n!=NULL; n=n->next)
	if (equal(b,n)) {
        free(b); 
        return; 
    }
    if (q2==NULL) {
        f_value(b);
        q2=b;
        bornprint(b);
    }
    else {
	    f_value(b);
	    for (n=q2; n->next!=NULL; n=n->next)
	        if (equal(b,n)) {
                free(b); 
                return; 
            }
	    for (n=q2; n!=NULL; oldn=n, n=n->next){
	        if (n->f > b->f) break;
        }
	    b -> next = n;
	    if (oldn != NULL) oldn->next = b;
	    else q2 = b;
	    bornprint(b);
    }
}


    
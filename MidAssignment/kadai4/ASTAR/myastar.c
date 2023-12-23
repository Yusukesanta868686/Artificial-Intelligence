/* A star search */
#include <stdio.h>
#include <stdlib.h>

#define NUM_COUPLES 3
#define NUM_PEOPLE (2 * NUM_COUPLES)
#define NUM_TRANSITION (NUM_PEOPLE * (NUM_PEOPLE - 1) / 2 + NUM_PEOPLE)

typedef struct BOARD {
    int cell[NUM_PEOPLE];
    struct BOARD *next;
    struct BOARD *back;
    int depth;
    int f;
    int now;
} BOARD ;

BOARD B0;
BOARD BG;

BOARD *q0 = NULL;
BOARD *q2 = &B0;

int nc = 0;
int ns = 0;

int depth_limit = 4;
int depth_increase = 6;

BOARD *getq() {
    BOARD *q; 
    if (q2==NULL) return NULL;
    q = q2;
    q2 = q2->next;
    return (q);
}


void expand(struct BOARD *b);
void exchange(struct BOARD *b,int i,int j);
int equal(struct BOARD *b1, struct BOARD *b2);
void init();
int check(BOARD* b);
void traceback(struct BOARD *b);
void putq(struct BOARD *b);
void f_value(BOARD *b);

int main(){
    BOARD *b;
    BOARD *q1;

    init();
    f_value(&B0);

    while ((q1 = getq()) != NULL) {
	    expand(q1);
	    if (q0==NULL) q0 = q1;
	    else {
	        q1 -> next = q0;
	        q0 = q1;
	    }
    }
}

void init(){
    for (int i = 0; i < NUM_PEOPLE; i++){
        B0.cell[i] = 0;
        BG.cell[i] = 1;
    }
    B0.next = NULL;
    BG.next = NULL;
    B0.back = NULL;
    BG.back = NULL;
    B0.depth = 0;
    BG.depth = 0;
    B0.now = 0;
    BG.now = 0;
}

int diff(BOARD* b1,BOARD* b2){
    int ans=0,k;
    for (k = 0; k < NUM_PEOPLE; k++) if(b1->cell[k] != b2->cell[k]) ans++;
    return(ans);
}


void f_value(BOARD *b){
    b->f = b->depth + 10 * diff(b,&BG);
}


void exchange(BOARD *b, int s, int t){
    BOARD *m = (BOARD*) malloc (sizeof(BOARD));
    if (m == NULL){
        printf("memory overflow\n");
        exit(2);
    }
    
    for (int i = 0; i < NUM_PEOPLE; i++){
        m->cell[i] = b->cell[i];
    }

    if (s >= 0) m->cell[s] = 1 - (m->cell[s]);
    if (t >= 0) m->cell[t] = 1 - (m->cell[t]);
    
    m->back = b;
    m->next = NULL;
    m->depth = b->depth + 1;
    m->now = m->cell[s];
    if (equal(m, &BG)){
        f_value(b);
        nc++;
        printf("**** Answer found! ****\n");
        printf("------trace------\n");
        traceback(m);
        printf("-----------------\n");
        printf("Number of children is %d\n", nc);
        printf("Length of solution is %d\n", ns - 1);
        exit(0);
    }
    if (check(m)) putq(m);
}


void expand(BOARD *b){
    for (int i = 0; i < NUM_PEOPLE; i++){
        for (int j = i; j < NUM_PEOPLE; j++){
            if (i == j) {
                if (b->depth % 2 == 0 && b->cell[i] == 0){
                    exchange(b, i, -1);
                } else if (b->depth % 2 == 1 && b->cell[i] == 1){
                    exchange(b, i, -1);
                }
            } else if (b->depth % 2 == 0){
                if (b->cell[i] == 0 && b->cell[j] == 0){
                    exchange(b, i, j);
                }
            } else if (b->depth % 2 == 1){
                if (b->cell[i] == 1 && b->cell[j] == 1){
                    exchange(b, i, j);
                }
            }
        }
    }
}

int check(BOARD *b){
    int together;
    int nanpa;

    if (b == NULL) return 0;
    
    for (int i = 0; i < NUM_COUPLES; i++){
        together = 0;
        nanpa = 0;
        for (int j = NUM_COUPLES; j < NUM_PEOPLE; j++){
            if (j == i + NUM_COUPLES) {
                if (b->cell[i] == b->cell[j]) together = 1;
            }

            else{
                if (b->cell[i] == b->cell[j]) nanpa = 1;
            }
        }

        if (together == 0 && nanpa == 1) return 0;
    }

    return 1;
}

int equal(BOARD *b1,BOARD *b2){
    register int i;
    for (i=0; i< NUM_PEOPLE; i++) 
	if (b1->cell[i]!=b2->cell[i]) return 0;
    return 1;
}

void traceback(struct BOARD *b){
    for (; b!= NULL; b = b->back) {
        ns++;
        for (int i = 0; i < NUM_PEOPLE; i++){
            printf("%d", b->cell[i]);
        }
        printf("\n");
    }
}



void putq(BOARD *b){
    BOARD *n,*oldn=NULL; 
    for (n=q0; n!=NULL; n=n->next)
	if (equal(b,n) && b->now == n->now) {
        free(b); 
        return; 
    }
    if (q2==NULL) {
        f_value(b);
        q2=b;
        nc++;
    }
    else {
	    f_value(b);
	    for (n=q2; n->next!=NULL; n=n->next)
	        if (equal(b,n) && b->now == n->now) {
                free(b); 
                return; 
            }
	    for (n=q2; n!=NULL; oldn=n, n=n->next){
	        if (n->f > b->f) break;
        }
	    b -> next = n;
	    if (oldn != NULL) oldn->next = b;
	    else q2 = b;
	    nc++;
    }
}


    
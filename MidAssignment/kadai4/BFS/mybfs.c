#include <stdio.h>
#include <stdlib.h>

#define NUM_COUPLES 3
#define NUM_PEOPLE (2 * NUM_COUPLES)

typedef struct BOARD {
    int cell[NUM_PEOPLE];
    struct BOARD *next;
    struct BOARD *back;
    int depth;
    int now;
} BOARD ;

BOARD B0;
BOARD BG;

BOARD *q0 = NULL;
BOARD *q1 = &B0;
BOARD *q2 = &B0;

int nc = 0;
int ns = 0;

int depth_limit = 4;
int depth_increase = 6;

int check(BOARD*);
int equal(BOARD*, BOARD*);
void exchange(BOARD*, int, int);
void expand(BOARD*);
void putq(BOARD*);
void init();

int main(){
    struct BOARD *b;
    init();
    while (q1 != NULL) {
	    for (b=q1; b!=NULL; b=b->next) expand(b);
	    q0=q1;
        q1=q2;
        q2=NULL;
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



void traceback(struct BOARD *b){
    for (; b!= NULL; b = b->back) {
        ns++;
        for (int i = 0; i < NUM_PEOPLE; i++){
            printf("%d", b->cell[i]);
        }
        printf("\n");
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

int equal (BOARD *b1, BOARD *b2){
    for (int i = 0; i < NUM_PEOPLE; i++){
        if (b1->cell[i] != b2->cell[i]) return 0;
    }
    return 1;
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

void putq(BOARD *b){
    BOARD *n;
    for (n = q0; n != NULL; n = n->next){
        if (equal(b, n) && b->now == n->now){
            free(b);
            return;
        } 
    }
    if (q2 == NULL){
        q2 = b;
        nc++;
    } else {
        for (n = q2; n->next != NULL; n = n->next){
            if (equal(b, n) && b->now == n->now){
                free(b);
                return;
            }
        }
        n->next = b;
        nc++;
    }
}
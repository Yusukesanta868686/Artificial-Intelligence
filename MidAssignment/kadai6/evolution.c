#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FIELD_SIZE 32
#define NUM_FAMILY 100
#define NUM_GENERATION 20000
#define NUM_POINTS 23

typedef struct Point{
    int x;
    int y;
    double score;
} Point;

typedef struct Family{
    double sum_score;
    Point points[NUM_POINTS];
} Family;

int mask[FIELD_SIZE][FIELD_SIZE];

Family* init(){
    Family *family = (Family*)malloc(sizeof(Family) * NUM_FAMILY);
    for(int i = 0; i < NUM_FAMILY; i++){
        for (int j = 0; j < NUM_POINTS; j++){
            family[i].points[j].x = rand() % FIELD_SIZE;
            family[i].points[j].y = rand() % FIELD_SIZE;
            family[i].points[j].score = 0;
        }   
        family[i].sum_score = 0;
    }
    return family;
    free(family);
}

double calc_potential_energy(Point point1, Point point2){
    return 1 / (pow(pow((point1.x - point2.x), 2) + pow((point1.y - point2.y), 2), 0.5)+ 0.001);
}

int cmp_Asc_Score(const void *family1, const void *family2){
    if (((Family *)family1)->sum_score > ((Family *)family2)->sum_score) return 1;
    else if (((Family *)family1)->sum_score < ((Family*)family2)->sum_score) return -1;
    else return 0;
}

int cmp_Asc_Score_for_point(const void *point1, const void *point2){
    if (((Point *)point1)->score > ((Point *)point2)->score) return 1;
    else if (((Point *)point1)->score < ((Point *)point2)->score) return -1;
    else return 0;
}

void printboard(Family family, int gen){
    for (int j = 0; j < FIELD_SIZE; j++){
        for (int k = 0; k < FIELD_SIZE; k++){
            mask[j][k] = 0;
        }
    }
    for (int j = 0; j < NUM_POINTS; j++){
        mask[family.points[j].y][family.points[j].x] = 1;
    }
    printf("generation: %d\n", gen);
    for (int j = 0; j < FIELD_SIZE + 2; j++){
        for (int k = 0; k < FIELD_SIZE + 2; k++){
            if (j == 0 || j == FIELD_SIZE + 1){
                if (k == 0 || k == FIELD_SIZE + 1) printf("+");
                else printf("--");
            } else{
                if (k == 0 || k == FIELD_SIZE + 1) printf("|");
                else if (mask[j - 1][k - 1] == 1) printf("o ");
                else printf("  ");
            }
        }
        printf("\n");
    }
}

int main(){
    //構造体の定義
    Family *family = (Family*)malloc(sizeof(Family) * NUM_FAMILY);
    Family *nextfamily = (Family*)malloc(sizeof(Family) * NUM_FAMILY);
    
    //構造体の初期化
    family = init();
    nextfamily = init();

    //変数の定義
    double score, ran_value;

    //世代数だけ進化計算を行う
    for (int i = 0; i < NUM_GENERATION; i++){
        for (int j = 0; j < NUM_FAMILY; j++){
            for (int k = 0; k < NUM_POINTS - 1; k++){
                for (int l = k + 1; l < NUM_POINTS; l++){
                    score = calc_potential_energy(family[j].points[k], family[j].points[l]);
                    family[j].points[k].score += score;
                    family[j].points[l].score += score;
                    family[j].sum_score += score; 
                }
                
            }
        }
        //家族のスコアの昇順にソート
        qsort(family, NUM_FAMILY, sizeof(Family), cmp_Asc_Score);
        
        for (int j = 0; j < NUM_FAMILY; j++){
            qsort(&family[j].points, NUM_POINTS, sizeof(Point), cmp_Asc_Score_for_point);
        }

        if (i % 100 == 0) printboard(family[0], i);
        
        //次の世代への引き継ぎ
        for (int j = 0; j < NUM_FAMILY / 5; j++){
            memcpy(&nextfamily[j], &family[j], sizeof(Family));
            nextfamily[j].sum_score = 0;
        }

        for (int j = NUM_FAMILY / 5 ; j < NUM_FAMILY; j++){
            int f1 = rand() % (NUM_FAMILY / 3);
            int f2 = (f1 + 37) % (NUM_FAMILY / 3);
            int ko, ko2;
            for(int k = 0; k < NUM_POINTS; k++){
                if (k < (int)(NUM_POINTS * 0.4)){
                    int a = rand() % NUM_POINTS;
                    nextfamily[j].points[k].x = family[f1].points[k].x;
                    nextfamily[j].points[k].y = family[f1].points[k].y;
                    ko = k;
                }
                else if (k < (int)(NUM_POINTS * 0.8)){
                    int a = rand() % NUM_POINTS;
                    nextfamily[j].points[k].x = family[f2].points[k - ko - 1].x;
                    nextfamily[j].points[k].y = family[f2].points[k - ko - 1].y;
                    ko2 = k;
                }
                else {
                    ran_value = (double)rand() / RAND_MAX;
                    if (ran_value > 0.8){
                        nextfamily[j].points[k].x = rand() % FIELD_SIZE;
                        nextfamily[j].points[k].y = rand() % FIELD_SIZE;
                    } else {
                        int a = rand() % NUM_POINTS;
                        nextfamily[j].points[k].x = ((family[f1].points[k - ko2 - 1].x + family[f2].points[k - ko2 - 1].x) / 2) ;
                        nextfamily[j].points[k].y = ((family[f1].points[k - ko2 - 1].y + family[f2].points[k - ko2 - 1].y) / 2) ;
                    }
                }
            }
            nextfamily[j].sum_score = 0;
        }

        memcpy(family, nextfamily, sizeof(Family) * NUM_FAMILY);
        
    }
    free(family);
    free(nextfamily);
    return 0;
}


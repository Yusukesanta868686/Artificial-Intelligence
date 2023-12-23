#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FIELD_SIZE 32
#define NUM_INDIVIDUAL 200
#define NUM_GENERATION 10000
#define NUM_POINTS 23

typedef struct Point{
    int x;
    int y;
} Point;

typedef struct Individual{
    double sum_score;
    Point points[NUM_POINTS];
} Individual;

int mask[FIELD_SIZE][FIELD_SIZE];

Individual* init(){
    Individual *group = (Individual*)malloc(sizeof(Individual) * NUM_INDIVIDUAL);
    for(int i = 0; i < NUM_INDIVIDUAL; i++){
        for (int j = 0; j < NUM_POINTS; j++){
            group[i].points[j].x = rand() % FIELD_SIZE;
            group[i].points[j].y = rand() % FIELD_SIZE;
        }   
        group[i].sum_score = 0;
    }
    return group;
    free(group);
}

double calc_potential_energy(Point point1, Point point2){
    return 1 / (pow(pow((point1.x - point2.x), 2) + pow((point1.y - point2.y), 2), 0.5)+ 0.001);
}

int cmp_Asc_Score(const void *group1, const void *group2){
    if (((Individual *)group1)->sum_score > ((Individual *)group2)->sum_score) return 1;
    else if (((Individual *)group1)->sum_score < ((Individual *)group2)->sum_score) return -1;
    else return 0;
}


void printboard(Individual group, int gen){
    for (int j = 0; j < FIELD_SIZE; j++){
        for (int k = 0; k < FIELD_SIZE; k++){
            mask[j][k] = 0;
        }
    }
    for (int j = 0; j < NUM_POINTS; j++){
        mask[group.points[j].y][group.points[j].x] = 1;
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
    Individual *group = (Individual*)malloc(sizeof(Individual) * NUM_INDIVIDUAL);
    Individual *nextgroup = (Individual*)malloc(sizeof(Individual) * NUM_INDIVIDUAL);
    
    //構造体の初期化
    group = init();
    nextgroup = init();

    //変数の定義
    double score, ran_value;

    //世代数だけ進化計算を行う
    for (int i = 0; i < NUM_GENERATION; i++){
        for (int j = 0; j < NUM_INDIVIDUAL; j++){
            for (int k = 0; k < NUM_POINTS - 1; k++){
                for (int l = k + 1; l < NUM_POINTS; l++){
                    score = calc_potential_energy(group[j].points[k], group[j].points[l]);
                    group[j].sum_score += score; 
                }
                
            }
        }

        //家族のスコアの昇順にソート
        qsort(group, NUM_INDIVIDUAL, sizeof(Individual), cmp_Asc_Score);

        if (i % 100 == 0) printboard(group[0], i); 
    
        //次の世代への引き継ぎ
        for (int j = 0; j < NUM_INDIVIDUAL / 5; j++){
            memcpy(&nextgroup[j], &group[j], sizeof(Individual));
            nextgroup[j].sum_score = 0;
        }

        for (int j = NUM_INDIVIDUAL / 5 ; j < NUM_INDIVIDUAL; j++){
            int father = rand() % (NUM_INDIVIDUAL / 5);
            int mother = rand() % (NUM_INDIVIDUAL / 5);
            for(int k = 0; k < NUM_POINTS; k++){
                double a = (double)rand() / RAND_MAX;
                
                if (a < 0.4){
                    nextgroup[j].points[k].x = group[father].points[k].x;
                    nextgroup[j].points[k].y = group[father].points[k].y;
                }
                else if (a < 0.8){
                    nextgroup[j].points[k].x = group[mother].points[k].x;
                    nextgroup[j].points[k].y = group[mother].points[k].y;
                }
                else {
                    nextgroup[j].points[k].x = rand() % FIELD_SIZE;
                    nextgroup[j].points[k].y = rand() % FIELD_SIZE;
                }
            }
            nextgroup[j].sum_score = 0;
        }

        //構造体を次の世代にコピー
        memcpy(group, nextgroup, sizeof(Individual) * NUM_INDIVIDUAL);
        
    }
    free(group);
    free(nextgroup);
    return 0;
}


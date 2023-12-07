#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FIELD_SIZE 40
#define NUM_INDIVIDUAL 200
#define NUM_GENERATION 10000
#define NUM_POINTS 23

typedef struct Point{
    int x;
    int y;
    double score;
} Point;

typedef struct Group{
    double sum_score;
    Point points[NUM_POINTS];
} Group;

int mask[FIELD_SIZE][FIELD_SIZE];

Group* init(){
    Group *group = (Group*)malloc(sizeof(Group) * NUM_INDIVIDUAL);
    for(int i = 0; i < NUM_INDIVIDUAL; i++){
        for (int j = 0; j < NUM_POINTS; j++){
            group[i].points[j].x = rand() % FIELD_SIZE;
            group[i].points[j].y = rand() % FIELD_SIZE;
            group[i].points[j].score = 0;
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
    if (((Group *)group1)->sum_score > ((Group *)group2)->sum_score) return 1;
    else if (((Group *)group1)->sum_score < ((Group *)group2)->sum_score) return -1;
    else return 0;
}

int cmp_Asc_Score_for_point(const void *point1, const void *point2){
    if (((Point *)point1)->score > ((Point *)point2)->score) return 1;
    else if (((Point *)point1)->score < ((Point *)point2)->score) return -1;
    else return 0;
}

void printboard(Group group, int gen){
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
    Group *group = (Group*)malloc(sizeof(Group) * NUM_INDIVIDUAL);
    Group *nextgroup = (Group*)malloc(sizeof(Group) * NUM_INDIVIDUAL);
    
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
                    group[j].points[k].score += score;
                    group[j].points[l].score += score;
                    group[j].sum_score += score; 
                }
                
            }
        }
        //家族のスコアの昇順にソート
        qsort(group, NUM_INDIVIDUAL, sizeof(Group), cmp_Asc_Score);

        //printf("generation: %d\n", i);
        //for (int j = 0; j < NUM_INDIVIDUAL; j++) printf("%f\n", group[j].sum_score);
        if (i % 100 == 0) printboard(group[0], i); 

        for (int j = 0; j < NUM_INDIVIDUAL; j++){
            qsort(&group[j].points, NUM_POINTS, sizeof(Point), cmp_Asc_Score_for_point);
        }

    
        //次の世代への引き継ぎ
        for (int j = 0; j < NUM_INDIVIDUAL / 5; j++){
            memcpy(&nextgroup[j], &group[j], sizeof(Group));
            nextgroup[j].sum_score = 0;
            for (int k = 0; k < NUM_POINTS; k++) nextgroup[j].points[k].score = 0;
        }

        for (int j = NUM_INDIVIDUAL / 5 ; j < NUM_INDIVIDUAL; j++){
            int father = rand() % (NUM_INDIVIDUAL / 5);
            int mother = rand() % (NUM_INDIVIDUAL / 5);
            for(int k = 0; k < NUM_POINTS; k++){
                int a = rand() % NUM_POINTS;
                if (k < (int)(NUM_POINTS * 0.4)){
                    nextgroup[j].points[k].x = group[father].points[k].x;
                    nextgroup[j].points[k].y = group[father].points[k].y;
                }
                else if (k < (int)(NUM_POINTS * 0.8)){
                    nextgroup[j].points[k].x = group[mother].points[k].x;
                    nextgroup[j].points[k].y = group[mother].points[k].y;
                }
                else {
                    nextgroup[j].points[k].x = rand() % FIELD_SIZE;
                    nextgroup[j].points[k].y = rand() % FIELD_SIZE;
                }
            }

            for (int k = 0; k < NUM_POINTS; k++) nextgroup[j].points[k].score = 0;
            nextgroup[j].sum_score = 0;
        }

        //構造体を次の世代にコピー
        memcpy(group, nextgroup, sizeof(Group) * NUM_INDIVIDUAL);
        
    }
    free(group);
    free(nextgroup);
    return 0;
}


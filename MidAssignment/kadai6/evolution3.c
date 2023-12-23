#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FIELD_SIZE 20
#define NUM_INDIVIDUAL 200
#define NUM_GENERATION 10000
#define NUM_POINTS 23
#define NUM_CHARGES 6
#define MASK_SIZE (int)((1 + (double)1 / pow(2, 0.5)) * FIELD_SIZE * 2)
typedef struct Point{
    int x;
    int y;
    int z;
} Point;

typedef struct Individual{
    double sum_score;
    Point points[NUM_POINTS];
} Individual;

double charges[8][3];

Individual* init(){
    Individual *group = (Individual*)malloc(sizeof(Individual) * NUM_INDIVIDUAL);
    for(int i = 0; i < NUM_INDIVIDUAL; i++){
        for (int j = 0; j < NUM_POINTS; j++){
            group[i].points[j].x = rand() % FIELD_SIZE;
            group[i].points[j].y = rand() % FIELD_SIZE;
            group[i].points[j].z = rand() % (FIELD_SIZE * 2);
        }   
        group[i].sum_score = 0;
    }
    return group;
    free(group);
}

void init_charge(double charges[][3]){
    double array[8][3] = {{round((10 * pow(3, 0.5) / 9) * 100) / 100, round(((double)50 / 3) * 100) / 100 + 20, round((40 * pow(6, 0.5) / 9 ) * 100) / 100 + 20},
    {round((10 * pow(3, 0.5) / 9) * 100) / 100, round((-(double)50 / 3) * 100) / 100 + 20, round((40 * pow(6, 0.5) / 9) * 100) / 100 +  20},
    {round((160 * pow(3, 0.5) / 9) * 100) / 100, 20, round((40 * pow(6, 0.5) / 9) * 100) / 100 + 20},
    {round((10 * pow(3, 0.5) / 9) * 100) / 100, round(((double)50 / 3) * 100) / 100 + 20, round((-40 * pow(6, 0.5) / 9) * 100) / 100 + 20},
    {round((10 * pow(3, 0.5) / 9) * 100) / 100, round((-(double)50 / 3) * 100) / 100 + 20, round((-40 * pow(6, 0.5) / 9) * 100) / 100 + 20},
    {round((160 * pow(3, 0.5) / 9) * 100) / 100, 20, round((-40 * pow(6, 0.5) / 9 ) * 100) / 100 + 20},
    {round((20 * pow(3, 0.5) / 3) * 100) / 100, 20, round((5 * pow(6, 0.5) / 3) * 100) / 100 + 20},
    {round((20 * pow(3, 0.5) / 3) * 100) / 100, 20, -round((5 * pow(6, 0.5) / 3) * 100) / 100 + 20}};

    memcpy(charges, array, sizeof(array));
}
double calc_potential_energy(Point point1, Point point2){
    return 1 / (pow(pow(point1.x - point2.x, 2) + pow(point1.y - point2.y, 2) + pow(point1.z - point2.z, 2), 0.5)+ 0.001);
}

double calc_coulomb_energy(Point point){
    double energy1 = 1 / pow((pow(point.x - charges[0][0], 2) + pow(point.y - charges[0][1], 2) + pow(point.z - charges[0][2], 2)), 0.5);
    double energy2 = 1 / pow((pow(point.x - charges[1][0], 2) + pow(point.y - charges[1][1], 2) + pow(point.z - charges[1][2], 2)), 0.5);    
    double energy3 = 1 / pow((pow(point.x - charges[2][0], 2) + pow(point.y - charges[2][1], 2) + pow(point.z - charges[2][2], 2)), 0.5);
    double energy4 = 1 / pow((pow(point.x - charges[3][0], 2) + pow(point.y - charges[3][1], 2) + pow(point.z - charges[3][2], 2)), 0.5);
    double energy5 = 1 / pow((pow(point.x - charges[4][0], 2) + pow(point.y - charges[4][1], 2) + pow(point.z - charges[4][2], 2)), 0.5);
    double energy6 = 1 / pow((pow(point.x - charges[5][0], 2) + pow(point.y - charges[5][1], 2) + pow(point.z - charges[5][2], 2)), 0.5);
    double energy7 = 1 / pow((pow(point.x - charges[6][0], 2) + pow(point.y - charges[6][1], 2) + pow(point.z - charges[6][2], 2)), 0.5);
    double energy8 = 1 / pow((pow(point.x - charges[7][0], 2) + pow(point.y - charges[7][1], 2) + pow(point.z - charges[7][2], 2)), 0.5);
    if (energy1 + energy2 + energy3 + energy7/16 > energy4 + energy5 + energy6 + energy8/16) return (energy1 + energy2 + energy3 + energy7/16);  
    else return (energy4 + energy5 + energy6 + energy8/16);
}

double calc_g(Point *point){
    double g_x = 0;
    double g_y = 0;
    double g_z = 0;
    for (int i = 0; i < NUM_POINTS; i++){
        g_x += (double)point[i].x / NUM_POINTS;
        g_y += (double)point[i].y / NUM_POINTS;
        g_z += (double)point[i].z / NUM_POINTS;
    }
    return pow(g_x - 20 * pow(3, 0.5) / 3, 2) + pow(g_y - 20, 2) + pow(g_z - 20, 2);
}

int cmp_Asc_Score(const void *group1, const void *group2){
    if (((Individual *)group1)->sum_score > ((Individual *)group2)->sum_score) return 1;
    else if (((Individual *)group1)->sum_score < ((Individual *)group2)->sum_score) return -1;
    else return 0;
}


void printboard(Individual group, int gen){
    int mask[MASK_SIZE][MASK_SIZE];
    int z, x;
    for (int j = 0; j < MASK_SIZE; j++){
        for (int k = 0; k < MASK_SIZE; k++){
            mask[j][k] = 0;
        }
    }
    for (int j = 0; j < NUM_POINTS; j++){
        z = (int)(group.points[j].z + (double)group.points[j].y / pow(2, 0.5));
        x = (int)(group.points[j].x + (double)group.points[j].y / pow(2, 0.5));
        printf("%d, %d, %d\n", group.points[j].x, group.points[j].y, group.points[j].z);
        mask[z][x] = 1;
    }
    printf("generation: %d\n", gen);
    for (int j = 0; j < MASK_SIZE + 2; j++){
        for (int k = 0; k < MASK_SIZE + 2; k++){
            if (j == 0 || j == MASK_SIZE + 1){
                if (k == 0 || k == MASK_SIZE + 1) printf("+");
                else printf("--");
            } else{
                if (k == 0 || k == MASK_SIZE + 1) printf("|");
                else if (mask[MASK_SIZE - j][k - 1] == 1) printf("o ");
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
    init_charge(charges);
    //変数の定義
    double score, ran_value;
    
    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 3; j++){
            printf("%f,", charges[i][j]);
        }
        printf("\n");
    }
    //世代数だけ進化計算を行う
    for (int i = 0; i < NUM_GENERATION; i++){
        for (int j = 0; j < NUM_INDIVIDUAL; j++){
            for (int k = 0; k < NUM_POINTS - 1; k++){
                double score2 = 0;
                for (int l = k + 1; l < NUM_POINTS; l++){
                    score = calc_potential_energy(group[j].points[k], group[j].points[l]);
                    group[j].sum_score += score; 
                    score2 += score;
                }
                group[j].sum_score += calc_coulomb_energy(group[j].points[k]);   
            }
            group[j].sum_score += calc_coulomb_energy(group[j].points[NUM_POINTS - 1]);
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
                    nextgroup[j].points[k].z = group[father].points[k].z;
                }
                else if (a < 0.8){
                    nextgroup[j].points[k].x = group[mother].points[k].x;
                    nextgroup[j].points[k].y = group[mother].points[k].y;
                    nextgroup[j].points[k].z = group[mother].points[k].z;
                }
                else {
                    nextgroup[j].points[k].x = rand() % FIELD_SIZE;
                    nextgroup[j].points[k].y = rand() % FIELD_SIZE;
                    nextgroup[j].points[k].z = rand() % (FIELD_SIZE * 2);
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
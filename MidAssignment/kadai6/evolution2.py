import random
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Circle

FIELD_SIZE = 256
NUM_GENERATION= 100000

def set_points(points):
    for i in range(23):
        points[i, 0] = random.randint(-256, 256)
        points[i, 1] = random.randint(-256, 256)
    return points    

def eval_origin(point):
    return (point[0] ** 2 + point[1] ** 2) * 100

def eval_first_circle(point, n):
    score1 = np.abs((point[0] ** 2 + point[1] ** 2) - 2500)
    
    return score1 / 25

def eval_second_circle(point, n):
    score1 = np.abs((point[0] ** 2 + point[1] ** 2) - 22500)
   
    return score1 / 225

def eval_third_circle(point, n):
    score1 = np.abs((point[0] ** 2 + point[1] ** 2) - 62500)

    return score1 /625

def calc_g(points):
    x0 = 0 
    y0 = 0
    x1 = 0
    y1 = 0
    x2 = 0
    y2 = 0

    for i in range(1, 3):
        x0 += points[i, 0]
        y0 += points[i, 1]

    for i in range(3, 7):
        x1 += points[i, 0]
        y1 += points[i, 1]

    for i in range(7, 23):
        x2 += points[i, 0]
        y2 += points[i, 1]

    return x0/2, y0/2, x1/4, y1/4, x2/16, y2/16  

def calc_var(points):
    return (np.var(points[1:3, 0]), np.var(points[1:3, 1]), np.var(points[3:7, 0]), 
        np.var(points[3:7, 1]), np.var(points[7:23, 0]), np.var(points[7:23, 1])) 

def calc_arg(points):
    arg1 = np.array([np.arctan2(points[i, 0], points[i, 1]) for i in range(1, 3)])
    arg2 = np.array([np.arctan2(points[i, 0], points[i, 1]) for i in range(3, 7)])
    arg3 = np.array([np.arctan2(points[i, 0], points[i, 1]) for i in range(7, 23)])

    return arg1, arg2, arg3

def calc_nearest_distance(points, arg, n):
    arg_order = np.argsort(arg)
    result = np.zeros(arg.shape[0])
    for i in range(arg.shape[0]):
        result[arg_order[i]] = ((points[arg_order[i], 0] - points[arg_order[(i + 1) % n], 0]) ** 2 + (points[arg_order[i], 1] - points[arg_order[(i + 1) % n], 1]) ** 2)

    return result
def calc_arg_avg(points):
    one_arg, two_arg, three_arg = 0, 0, 0
    for i in range(1, 3):
        one_arg += np.arctan2(points[i, 0], points[i, 1])
    for i in range(3, 7):
        two_arg += np.arctan2(points[i, 0], points[i, 1])
    for i in range(7, 23):
        three_arg += np.arctan2(points[i, 0], points[i, 1])

    return one_arg/2, two_arg/4, three_arg/16


def main():
    #初めの状態をセット
    points = np.zeros((23, 2))
    points = set_points(points)
    best = np.zeros((23, 2))
    min_eval_value = np.inf
    #探索
    x0_g, x1_g, x2_g, y0_g, y1_g, y2_g = 0, 0, 0, 0, 0, 0
    one_arg, two_arg, three_arg = 0, 0, 0
    x0_v, x1_v, x2_v, y0_v, y1_v, y2_v = 0, 0, 0, 0, 0, 0
    arg1, arg2, arg3 = np.zeros(2), np.zeros(4), np.zeros(16)
    arg_score1, arg_score2, arg_score3 = np.zeros(2), np.zeros(4), np.zeros(16)
    for i in range(NUM_GENERATION):
        eval_values = np.zeros(23)
        #ノルムの小さい順にソート
        norms = np.linalg.norm(points, axis = 1)
        sorted_points = points[np.argsort(norms)]
        
        x0_g, y0_g, x1_g, y1_g, x2_g, y2_g = calc_g(sorted_points)
        one_arg, two_arg, three_arg = calc_arg_avg(sorted_points)
        x0_v, y0_v, x1_v, y1_v, x2_v, y2_v = calc_var(sorted_points) 
        arg1, arg2, arg3 = calc_arg(sorted_points)
        arg_score1 = calc_nearest_distance(sorted_points[1:3], arg1, 2)
        arg_score2 = calc_nearest_distance(sorted_points[3:7], arg2, 4)
        arg_score3 = calc_nearest_distance(sorted_points[7:23], arg3, 16)

        #ノルムが一番小さい点の評価
        eval_values[0] = eval_origin(sorted_points[0])

        #次の2点の評価
        for j in range(1, 3):
            eval_values[j] = eval_first_circle(sorted_points[j], j) 

        #次の4点の評価
        for j in range(3, 7):
            eval_values[j] = eval_second_circle(sorted_points[j], j)

        #残りの16点の評価
        for j in range(7, 23):
            eval_values[j] = eval_third_circle(sorted_points[j], j)

        result = sorted_points[np.argsort(eval_values)]
        np.set_printoptions(precision=4, suppress=True)
 

        if (np.sum(eval_values) < min_eval_value):
            best = result
            min_eval_value = np.sum(eval_values)
            
            print("generation: {}".format(i))
            print(eval_values)
            print(best)
            print(np.argsort(eval_values))
            print(sorted_points)
            print(x0_g, x1_g, x2_g, y0_g, y1_g, y2_g)
            
        count2 , count4= 0, 0
        for j in range(23):
            theta = np.arctan2(sorted_points[j, 0], sorted_points[j, 1])
            if (j != 0):
                if (0 < j < 3):
                    points[j, 0] = sorted_points[j, 0] - int((4 * sorted_points[j, 0] * (sorted_points[j, 0] ** 2 + sorted_points[j, 1] ** 2 - 2500) / 25600 + x0_g / 2 - 2 * (sorted_points[j, 0] - x0_g) / 10) )
                    points[j, 1] = sorted_points[j, 1] - int((4 * sorted_points[j, 1] * (sorted_points[j, 0] ** 2 + sorted_points[j, 1] ** 2 - 2500) / 25600 + y0_g / 2 - 2 * (sorted_points[j, 1] - y0_g) / 10) )
                        
                    #points[j, 0], points[j, 1] = points[j, 0] * np.cos(2 * (theta - one_arg) / 2000) + points[j, 1] * np.sin(2 * (theta - one_arg) / 2), points[j, 0] * np.sin(2 * (theta - one_arg) / 2) - points[j, 1] * np.cos(2 * (theta - one_arg) / 2000)
                    #points[j + 10, 0] -= 4 * result[j, 0] * (result[j, 0] ** 2 + result[j, 1] ** 2 - 2500) 
                    #points[j + 10, 1] -= 4 * result[j, 1] * (result[j, 0] ** 2 + result[j, 1] ** 2 - 2500) 
                
                elif (2 < j < 7):
                    points[j, 0] = sorted_points[j, 0] - int((4 * sorted_points[j, 0] * (sorted_points[j, 0] ** 2 + sorted_points[j, 1] ** 2 - 22500) / 230400 + x1_g / 2 - 2 * (sorted_points[j, 0] - x1_g) / 25))
                    points[j, 1] = sorted_points[j, 1] - int((4 * sorted_points[j, 1] * (sorted_points[j, 0] ** 2 + sorted_points[j, 1] ** 2 - 22500) / 230400 + y1_g / 2 - 2 * (sorted_points[j, 1] - y1_g) / 25))
                    #points[j, 0], points[j, 1] = points[j, 0] * np.cos(2 * (theta - two_arg) / 4000) + points[j, 1] * np.sin(2 * (theta - two_arg) / 4), points[j, 0] * np.sin(2 * (theta - two_arg) / 4) - points[j, 1] * np.cos(2 * (theta - two_arg) / 4000)
                    #points[j + 10, 0] -= 4 * result[j, 0] * (result[j, 0] ** 2 + result[j, 1] ** 2 - 22500) 
                    #points[j + 10, 1] -= 4 * result[j, 1] * (result[j, 0] ** 2 + result[j, 1] ** 2 - 22500) 
                
                else:
                    points[j, 0] = sorted_points[j, 0] - int((4 * sorted_points[j, 0] * (sorted_points[j, 0] ** 2 + sorted_points[j, 1] ** 2 - 62500) / 540000 + x2_g / 2 - 2 * (sorted_points[j, 0] - x2_g) / 100))
                    points[j, 1] = sorted_points[j, 1] - int((4 * sorted_points[j, 1] * (sorted_points[j, 0] ** 2 + sorted_points[j, 1] ** 2 - 62500) / 540000 + y2_g / 2 - 2 * (sorted_points[j, 1] - y2_g) /100) )
                    #points[j, 0], points[j, 1] = points[j, 0] * np.cos(2 * (theta - three_arg) / 16000) + points[j, 1] * np.sin(2 * (theta - three_arg) / 16), points[j, 0] * np.sin(2 * (theta - three_arg) / 16) + points[j, 1] - np.cos(2 * (theta - three_arg) / 16000)
                    #points[j + 10, 0] -= 4 * result[j, 0] * (result[j, 0] ** 2 + result[j, 1] ** 2 - 62500) 
                    #points[j + 10, 1] -= 4 * result[j, 1] * (result[j, 0] ** 2 + result[j, 1] ** 2 - 62500) 
            else:
                points[j, 0] = sorted_points[j, 0] - 2 * sorted_points[j, 0] / 1000
                points[j, 1] = sorted_points[j, 1] - 2 * sorted_points[j, 1] / 1000
                #points[j + 10, 0] = random.randint(-256, 256)
                #points[j + 10, 1] = random.randint(-256, 256)
        
        points = np.clip(points, -256, 256)
    '''
        a = random.random()
        if (a > 0.8):
            for j in range(20, 23):
                points[j, 0] = random.randint(-256, 256)
                points[j, 1] = random.randint(-256, 256)
        else:
            for j in range(20, 23):
                points[j, 0] = (result[j - 18, 0] + result[j - 17, 0]) / 2
                points[j, 1] = (result[j - 18, 1] + result[j - 17, 1]) / 2

    '''
    plt.scatter(best[:, 0], best[:, 1])
    plt.title("Result")
    plt.xlim(-256, 256)
    plt.ylim(-256, 256)
    circle = Circle((0, 0), 50, color='green', fill=False)
    plt.gca().add_patch(circle)
    circle = Circle((0, 0), 150, color='green', fill=False)
    plt.gca().add_patch(circle)
    circle = Circle((0, 0), 250, color='green', fill=False)
    plt.gca().add_patch(circle)
    plt.axis('equal')
    plt.axhline(0, color='black', linewidth=0.5)
    plt.axvline(0, color='black', linewidth=0.5)
    plt.savefig("result.png")

if __name__ == "__main__":
    main()
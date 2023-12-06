import random
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Circle

FIELD_SIZE = 256
NUM_GENERATION= 100000

cos16 = np.array([250 * np.cos(i * np.pi / 8) for i in range(16)])
sin16 = np.array([250 * np.sin(i * np.pi / 8) for i in range(16)])
cos4 = np.array([150, 0, -150, 0])
sin4 = np.array([0, 150, 0, -150])
cos2 = np.array([50, -50])
sin2 = np.array([0, 0])

def set_points(points):
    for i in range(23):
        points[i, 0] = random.randint(-256, 256)
        points[i, 1] = random.randint(-256, 256)
        

def eval_origin(point):
    return (point[0] ** 2 + point[1] ** 2) * 100

def eval_first_circle(point, n):
    score1 = np.abs((point[0] ** 2 + point[1] ** 2) - 2500)

    score2 = np.inf
    for i in range(2):
        if (point[0] - cos2[i]) ** 2 + (point[1] - sin2[i]) ** 2 < score2:
            score2 = (point[0] - cos2[i]) ** 2 + (point[1] - sin2[i]) ** 2 
    
    #score2 = (point[0] - cos2[n - 1]) ** 2 + (point[1] - sin2[n - 1]) ** 2
    return (score1 * score2) / 15

def eval_second_circle(point, n):
    score1 = np.abs((point[0] ** 2 + point[1] ** 2) - 22500)
    
    score2 = np.inf
    for i in range(4):
        if (point[0] - cos4[i]) ** 2 + (point[1] - sin4[i]) ** 2 < score2:
            score2 = (point[0] - cos4[i]) ** 2 + (point[1] - sin4[i]) ** 2 
    
    #score2 = (point[0] - cos4[n - 3]) ** 2 + (point[1] - sin4[n - 3]) ** 2
    return (score1 * score2) / 135

def eval_third_circle(point, n):
    score1 = np.abs((point[0] ** 2 + point[1] ** 2) - 62500)
    
    score2= np.inf
    for i in range(16):
        if (point[0] - cos16[i]) ** 2 + (point[1] - sin16[i]) ** 2 < score2:
            score2 = (point[0] - cos16[i]) ** 2 + (point[1] - sin16[i]) ** 2 
    
    #score2 = (point[0] - cos16[n - 7]) ** 2 + (point[1] - sin16[n - 7]) ** 2

    return (score1 * score2) /375

def calc_g(points):
    x0 = 0 
    y0 = 0
    x1 = 0
    y1 = 0
    x2 = 0
    y2 = 0

    for i in range(1, 3):
        x0 += points[i][0]
        y0 += points[i][1]

    for i in range(3, 7):
        x1 += points[i][0]
        y1 += points[i][1]

    for i in range(7, 23):
        x2 += points[i][0]
        y2 += points[i][1]

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

def main():
    #初めの状態をセット
    points = np.zeros((23, 2))
    set_points(points)
    best = np.zeros((23, 2))
    min_eval_value = np.inf

    #探索
    x0_g, x1_g, x2_g, y0_g, y1_g, y2_g = 0, 0, 0, 0, 0, 0
    x0_v, x1_v, x2_v, y0_v, y1_v, y2_v = 0, 0, 0, 0, 0, 0
    arg1, arg2, arg3 = np.zeros(2), np.zeros(4), np.zeros(16)
    arg_score1, arg_score2, arg_score3 = np.zeros(2), np.zeros(4), np.zeros(16)
    for i in range(NUM_GENERATION):
        eval_values = np.zeros(23)
        #ノルムの小さい順にソート
        norms = np.linalg.norm(points, axis = 1)
        sorted_points = points[np.argsort(norms)]

        x0_g, y0_g, x1_g, y1_g, x2_g, y1_g = calc_g(sorted_points)
        x0_v, y0_v, x1_v, y1_v, x2_v, y2_v = calc_var(sorted_points) 
        arg1, arg2, arg3 = calc_arg(sorted_points)
        arg_score1 = calc_nearest_distance(sorted_points[1:3], arg1, 2)
        arg_score2 = calc_nearest_distance(sorted_points[3:7], arg2, 4)
        arg_score3 = calc_nearest_distance(sorted_points[7:23], arg3, 16)

        #ノルムが一番小さい点の評価
        eval_values[0] = eval_origin(sorted_points[0])

        #次の2点の評価
        for j in range(1, 3):
            eval_values[j] = eval_first_circle(sorted_points[j], j) + 25 / ((sorted_points[j, 0] -  x0_g) ** 2 + (sorted_points[j, 1]  - y0_g) ** 2 + 0.001) + 2500 / (x0_v ** 2 + y0_v ** 2 + 0.001) + 1000000 / (np.abs(arg_score1[j - 1]) + 0.001)

        #次の4点の評価
        for j in range(3, 7):
            eval_values[j] = eval_second_circle(sorted_points[j], j) + 225 / ((sorted_points[j, 0] -  x1_g) ** 2 + (sorted_points[j, 1] - y1_g) ** 2 + 0.001) + 22500 / (x1_v ** 2 + y1_v ** 2 + 0.001) + 1000000 / (np.abs(arg_score2[j - 3]) + 0.001)

        #残りの16点の評価
        for j in range(7, 23):
            eval_values[j] = eval_third_circle(sorted_points[j], j) + 625 / ((sorted_points[j, 0] - x2_g) ** 2 + (sorted_points[j, 1] - y2_g) ** 2 + 0.001) + 62500 / (x2_v ** 2 + y2_v ** 2 + 0.001) + 1000000 / (np.abs(arg_score3[j - 7]) + 0.001)
        
        result = sorted_points[np.argsort(eval_values)]
        

        if (np.sum(eval_values) < min_eval_value):
            best = result
            min_eval_value = np.sum(eval_values)
            print("generation: {}".format(i))
            print(eval_values)
            print(best)
            print(np.argsort(eval_values))
        count2 , count4= 0, 0
        for j in range(10):
            #points[j, 0] = result[j, 0] + 5 * random.choice([-1, 0, 1])
            #points[j, 1] = result[j, 1] + 5 * random.choice([-1, 0, 1])
            if (np.argsort(eval_values)[j] != 0):
                parts = 2 if (np.argsort(eval_values)[j] < 3) else 4 if (np.argsort(eval_values[j]) < 7) else 16
                points[j, 0] = result[j, 0] * np.cos(random.randint(0, parts) * np.pi / (parts / 2)) - result[j, 1] * np.sin(random.randint(0, parts) * np.pi / (parts / 2))
                points[j, 1] = result[j, 0] * np.sin(random.randint(0, parts) * np.pi / (parts / 2)) + result[j, 1] * np.cos(random.randint(0, parts) * np.pi / (parts / 2))     
                points[j + 10, 0] = result[j, 0] * np.cos(random.randint(0, parts) * np.pi / (parts / 2)) - result[j, 1] * np.sin(random.randint(0, parts) * np.pi / (parts / 2))
                points[j + 10, 1] = result[j, 0] * np.sin(random.randint(0, parts) * np.pi / (parts / 2)) + result[j, 1] * np.cos(random.randint(0, parts) * np.pi / (parts / 2))
                if (np.argsort(eval_values)[j] < 3):         
                    rate = 1 if count2 == 0 else 5 
                    points[j] *= rate
                    points[j + 10] *= rate 
                    if (count2):
                        points[j, 0], points[j, 1] = points[j, 0] * np.cos(random.randint(0, 15) * np.pi / 8) - points[j, 1] * np.sin(random.randint(0, 15) * np.pi / 8), points[j, 0] * np.sin(random.randint(0, 15) * np.pi / 8) + points[j, 1] * np.cos(random.randint(0, 15) * np.pi / 8)
                        points[j + 10, 0], points[j + 10, 1] = points[j + 10, 0] * np.cos(random.randint(0, 15) * np.pi / 8) - points[j + 10, 1] * np.sin(random.randint(0, 15) * np.pi / 8), points[j + 10, 0] * np.sin(random.randint(0, 15) * np.pi / 8) + points[j + 10, 1] * np.cos(random.randint(0, 15) * np.pi / 8)
                    count2 = 1
                elif (2 < np.argsort(eval_values)[j] < 7):
                    rate = 1 if count4 < 2 else 1.67
                    points[j] *= rate
                    points[j + 10] *= rate
                    count4 += 1
                    if (count4 >= 2):
                        points[j, 0], points[j, 1] = points[j, 0] * np.cos(random.randint(0, 15) * np.pi / 8) - points[j, 1] * np.sin(random.randint(0, 15) * np.pi / 8), points[j, 0] * np.sin(random.randint(0, 15) * np.pi / 8) + points[j, 1] * np.cos(random.randint(0, 15) * np.pi / 8)
                        points[j + 10, 0], points[j + 10, 1] = points[j + 10, 0] * np.cos(random.randint(0, 15) * np.pi / 8) - points[j + 10, 1] * np.sin(random.randint(0, 15) * np.pi / 8), points[j + 10, 0] * np.sin(random.randint(0, 15) * np.pi / 8) + points[j + 10, 1] * np.cos(random.randint(0, 15) * np.pi / 8)               
                    
                #points[j] *= random.choice([0.99, 1, 1.01])
                #points[j + 10] *= random.choice([0.99, 1, 1.01])
            else:
                points[j, 0] = result[j, 0] #+ 5 * random.choice([-1, 0, 1])
                points[j, 1] = result[j, 1] #+ 5 * random.choice([-1, 0, 1])
                points[j + 10, 0] = random.randint(-256, 256)
                points[j + 10, 1] = random.randint(-256, 256)


        a = random.random()
        if (a > 0.8):
            for j in range(20, 23):
                points[j, 0] = random.randint(-256, 256)
                points[j, 1] = random.randint(-256, 256)
        else:
            for j in range(20, 23):
                points[j, 0] = (result[j - 18, 0] + result[j - 17, 0]) / 2
                points[j, 1] = (result[j - 18, 1] + result[j - 17, 1]) / 2

    
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
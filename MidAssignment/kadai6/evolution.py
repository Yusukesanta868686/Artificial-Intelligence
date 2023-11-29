import random
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Circle

FIELD_SIZE = 256
NUM_GENERAL = 100000

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
    return (point[0] ** 2 + point[1] ** 2) * 10

def eval_first_circle(point):
    result = np.abs((point[0] ** 2 + point[1] ** 2) - 2500)
    min = np.inf
    for i in range(2):
        if (point[0] - cos2[i]) ** 2 + (point[1] - sin2[i]) ** 2 < min:
            min = (point[0] - cos2[i]) ** 2 + (point[1] - sin2[i]) ** 2 

    return (result + min) / 25

def eval_second_circle(point):
    result = np.abs((point[0] ** 2 + point[1] ** 2) - 22500)
    min = np.inf
    for i in range(4):
        if (point[0] - cos4[i]) ** 2 + (point[1] - sin4[i]) ** 2 < min:
            min = (point[0] - cos4[i]) ** 2 + (point[1] - sin4[i]) ** 2 

    return (result + min) / 225

def eval_third_circle(point):
    result = np.abs((point[0] ** 2 + point[1] ** 2) - 62500)
    min = np.inf
    for i in range(16):
        if (point[0] - cos16[i]) ** 2 + (point[1] - sin16[i]) ** 2 < min:
            min = (point[0] - cos16[i]) ** 2 + (point[1] - sin16[i]) ** 2 

    return (result + min) / 625

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
def main():
    #初めの状態をセット
    points = np.zeros((23, 2))
    set_points(points)
    best = np.zeros((23, 2))
    min_eval_value = np.inf
    #探索
    x0_g, x1_g, x2_g, y0_g, y1_g, y2_g = 0, 0, 0, 0, 0, 0
    x0_v, x1_v, x2_v, y0_v, y1_v, y2_v = 0, 0, 0, 0, 0, 0

    for i in range(NUM_GENERAL):
        eval_values = np.zeros(23)
        #ノルムの小さい順にソート
        norms = np.linalg.norm(points, axis = 1)
        sorterd_points = points[np.argsort(norms)]

        x0_g, y0_g, x1_g, y1_g, x2_g, y1_g = calc_g(sorterd_points)
        x0_v, y0_v, x1_v, y1_v, x2_v, y2_v = calc_var(sorterd_points) 
        #ノルムが一番小さい点の評価
        eval_values[0] = eval_origin(sorterd_points[0])

        #次の2点の評価
        for i in range(1, 3):
            eval_values[i] = eval_first_circle(sorterd_points[i]) + (x0_g ** 2 + y0_g ** 2) / 25 + 10 / (x0_v ** 2 + y0_v ** 2)


        #次の4点の評価
        for i in range(3, 7):
            eval_values[i] = eval_second_circle(sorterd_points[i]) + (x1_g ** 2 + y1_g ** 2) / 225 + 10 / (x1_v ** 2 + y1_v ** 2)
        
        #残りの16点の評価
        for i in range(7, 23):
            eval_values[i] = eval_third_circle(sorterd_points[i]) + (x2_g ** 2 + y2_g ** 2) / 625 + 10 / (x2_v ** 2 + y2_v ** 2)
        
        result = sorterd_points[np.argsort(eval_values)]
        

        if (np.sum(eval_values) < min_eval_value):
            best = result
            min_eval_value = np.sum(eval_values)
            print(eval_values)

        for i in range(5):
            points[i] = result[i]

        for i in range(5, 23):
            points[i][0] = random.randint(-256, 256)
            points[i][1] = random.randint(-256, 256)

    
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
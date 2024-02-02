import random
import numpy as np
from copy import deepcopy

def calculate_distance(solution):
    distance = 0
    for i in range(len(solution)):
        distance += abs(solution[i] - solution[(i + 1) % len(solution)])
    return distance

def is_valid(solution):
    for i in range(len(solution)):
        if (solution[i] == solution[(i + 1) % len(solution)]) or \
           (abs(solution[i] - solution[(i + 1) % len(solution)]) == 1) or \
           (abs(solution[i] - solution[(i + 2) % len(solution)]) == 1):
            return False
    return True

def genetic_algorithm(population_size, generations):
    population = [list(range(1, 31)) for _ in range(population_size)]
    best_solution = None

    for gen in range(generations):
        population = sorted(population, key=lambda x: calculate_distance(x))

        selected_parents = population[:int(0.2 * population_size)]
        children = []

        for _ in range(population_size - len(selected_parents)):
            parent1, parent2 = random.sample(selected_parents, 2)
            crossover_point = random.randint(1, len(parent1) - 1)
            child = parent1[:crossover_point] + [x for x in parent2 if x not in parent1[:crossover_point]]
            children.append(child)

        for child in children:
            if random.random() < 0.1:
                idx1, idx2 = random.sample(range(len(child)), 2)
                child[idx1], child[idx2] = child[idx2], child[idx1]

        population = selected_parents + children

        for solution in population:
            if is_valid(solution):
                best_solution = deepcopy(solution)
                break

        if best_solution:
            break

    return best_solution or population[0]

def simulated_annealing(solution, temperature, cooling_rate, iterations):
    current_solution = deepcopy(solution)
    best_solution = deepcopy(solution)

    for _ in range(iterations):
        new_solution = deepcopy(current_solution)

        idx1, idx2 = random.sample(range(len(new_solution)), 2)
        new_solution[idx1], new_solution[idx2] = new_solution[idx2], new_solution[idx1]

        if calculate_distance(new_solution) < calculate_distance(current_solution) or \
           random.random() < np.exp((calculate_distance(current_solution) - calculate_distance(new_solution)) / temperature):
            current_solution = deepcopy(new_solution)

        if calculate_distance(current_solution) < calculate_distance(best_solution) and is_valid(current_solution):
            best_solution = deepcopy(current_solution)

        temperature *= cooling_rate

    return best_solution

def hybrid_algorithm(population_size, generations, sa_temperature, sa_cooling_rate, sa_iterations):
    ga_solution = genetic_algorithm(population_size, generations)
    
    while not is_valid(ga_solution):
        ga_solution = genetic_algorithm(population_size, generations)

    sa_solution = simulated_annealing(ga_solution, sa_temperature, sa_cooling_rate, sa_iterations)
    
    return sa_solution

# ハイブリッドアルゴリズムの実行
population_size = 50
generations = 100
sa_temperature = 100
sa_cooling_rate = 0.95
sa_iterations = 1000

best_solution = hybrid_algorithm(population_size, generations, sa_temperature, sa_cooling_rate, sa_iterations)

print("最適なスケジュール:", best_solution)
print("移動距離:", calculate_distance(best_solution))
print("ハード制約を満たしているか:", is_valid(best_solution))

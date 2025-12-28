import matplotlib.pyplot as plt
import os
import numpy as np

# Create graphs directory if it doesn't exist
if not os.path.exists('graphs'):
    os.makedirs('graphs')

# Data from Report

# Scenario 1: Variation of Number of Vertices (V)
# Density ~ 20%
v_s1 = [5, 10, 15, 20, 25, 27, 30]
time_s1 = [0.000025, 0.0005, 0.005, 0.098, 0.033, 0.228, 0.243]
ops_s1 = [30, 769, 7799, 121673, 87430, 432387, 582231]

# Scenario 2: Variation of Density
# V = 15
density_s2 = [10, 30, 50, 70, 90]
time_s2 = [0.018, 0.002, 0.0001, 0.00002, 0.000007]
ops_s2 = [20201, 4214, 578, 121, 43]

# Scenario 3: Weighted vs Unweighted Comparison
# V = 30
labels_s3 = ['Unweighted', 'Weighted']
ops_s3 = [582231, 1200000] # 582k vs 1.2M approx

# Plotting Functions

def plot_scenario1():
    # Time vs V
    plt.figure(figsize=(10, 6))
    plt.plot(v_s1, time_s1, marker='o', linestyle='-', color='b', label='Tempo de Execução')
    plt.title('Figura 1: Tempo de Execução vs Número de Vértices (V)')
    plt.xlabel('Número de Vértices (V)')
    plt.ylabel('Tempo (s)')
    plt.grid(True)
    plt.legend()
    plt.savefig('graphs/scenario1_time.png')
    plt.close()

    # Operations vs V
    plt.figure(figsize=(10, 6))
    plt.plot(v_s1, ops_s1, marker='s', linestyle='--', color='r', label='Chamadas Recursivas')
    plt.title('Figura 2: Operações vs Número de Vértices (V)')
    plt.xlabel('Número de Vértices (V)')
    plt.ylabel('Número de Operações')
    plt.grid(True)
    plt.legend()
    plt.savefig('graphs/scenario1_ops.png')
    plt.close()
    
    # Operations vs V (Log Scale)
    plt.figure(figsize=(10, 6))
    plt.plot(v_s1, ops_s1, marker='s', linestyle='--', color='r', label='Chamadas Recursivas')
    plt.yscale('log')
    plt.title('Figura 3: Operações vs Número de Vértices (V) - Escala Logarítmica')
    plt.xlabel('Número de Vértices (V)')
    plt.ylabel('Número de Operações (Escala Logarítmica)')
    plt.grid(True)
    plt.legend()
    plt.savefig('graphs/scenario1_ops_log.png')
    plt.close()

def plot_scenario2():
    # Time vs Density
    plt.figure(figsize=(10, 6))
    plt.plot(density_s2, time_s2, marker='o', linestyle='-', color='g', label='Tempo de Execução')
    plt.title('Figura 4: Tempo de Execução vs Densidade do Grafo')
    plt.xlabel('Densidade (%)')
    plt.ylabel('Tempo (s)')
    plt.grid(True)
    plt.legend()
    plt.savefig('graphs/scenario2_time.png')
    plt.close()

    # Operations vs Density
    plt.figure(figsize=(10, 6))
    plt.plot(density_s2, ops_s2, marker='s', linestyle='--', color='m', label='Chamadas Recursivas')
    plt.title('Figura 5: Operações vs Densidade do Grafo')
    plt.xlabel('Densidade (%)')
    plt.ylabel('Número de Operações')
    plt.grid(True)
    plt.legend()
    plt.savefig('graphs/scenario2_ops.png')
    plt.close()

def plot_scenario3():
    # Bar chart for Comparison
    labels_pt = ['Não Ponderado', 'Ponderado']
    plt.figure(figsize=(8, 6))
    bars = plt.bar(labels_pt, ops_s3, color=['skyblue', 'orange'])
    plt.title('Figura 6: Algoritmo Não Ponderado vs Ponderado (V=30)')
    plt.ylabel('Número de Operações')
    
    # Add value labels on top of bars
    for bar in bars:
        yval = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2, yval, f'{int(yval):,}', ha='center', va='bottom')
        
    plt.savefig('graphs/scenario3_comparison.png')
    plt.close()

if __name__ == "__main__":
    print("A gerar gráficos...")
    plot_scenario1()
    plot_scenario2()
    plot_scenario3()
    print("Gráficos gerados na diretoria 'graphs/'.")

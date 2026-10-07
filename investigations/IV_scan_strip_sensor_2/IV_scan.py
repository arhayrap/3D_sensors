import matplotlib.pyplot as plt
import numpy as np

if __name__ == "__main__":
    bias_voltage = np.array([10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135])
    current = np.array([0.25, 0.31, 0.34, 0.38, 0.42, 0.48, 0.53, 0.61, 0.75, 0.97, 1.55, 1.7, 1.9, 1.94, 2.14, 2.32, 2.52, 2.65, 2.87, 3.37, 3.8, 4.2, 4.9, 6.2, 7.8, 10.4, 10.5])
    print(len(bias_voltage), len(current))
    # plt.plot(bias_voltage, current, marker = "o", label = "First IV scan")
    
    bias_voltage = np.array([10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 125, 126, 127])
    current_normal = np.array([0.001, 0.0015, 0.0015, 0.0015, 0.002, 0.0025, 0.0025, 0.003, 0.0025, 0.002, 0.002, 0.0015, 0.0015, 0.003, 0.016]);
    current_precision = np.array([0.0001, 0.0002, 0.0003, 0.0004, 0.0004, 0.001, 0.0005, 0.0004, 0.0008, 0.0003, 0.0003, 0.001, 0.0012, 0.005, 0.35])
    plt.plot(bias_voltage, current_normal, marker = "o", label = "Cooled down IV scan")
    
    plt.show()
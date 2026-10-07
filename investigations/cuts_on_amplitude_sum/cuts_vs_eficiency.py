import matplotlib.pyplot as plt
import numpy as np

if __name__ == "__main__":
    cuts = np.linspace(10,71,60)
    events = [4425,4425,4425,4425,4425,4425,4425,4425,4424,4419,4411,4381,4315,4206,4061,3909,3774,3686,3632,3586,3541,3505,3456,3414,3364,3304,3237,3176,3103,3035,2962,2894,2837,2768,2696,2619,2541,2466,2378,2281,2175,2072,1959,1844,1704,1547,1360,1155,944,717,507,353,239,175,114,81,54,34,22,13]
    plt.plot(cuts, events, marker = "o")
    plt.xlabel("Trigger strip percentage")
    plt.ylabel("Events")
    plt.savefig("cut_percentage_vs_events.png")
    plt.show()
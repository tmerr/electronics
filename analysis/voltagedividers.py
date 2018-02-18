"""Visualize how R1 and R2 affect output voltage"""

from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import numpy as np

def plot(X, Y, Z):
    fig = plt.figure()
    ax = fig.gca(projection='3d')
    surf = ax.plot_surface(X, Y, Z, vmin=0, vmax=3.3, cmap=cm.coolwarm,
                           linewidth=0, antialiased=False)
    fig.colorbar(surf, shrink=0.5, aspect=5)
    ax.set_xlabel("R1")
    ax.set_ylabel("R2")
    ax.set_zlabel("V (measured)")
    plt.show()


R1 = np.arange(0, 1000, 25)
R2 = np.arange(0, 1000, 25)
R1, R2 = np.meshgrid(R1, R2)
V = (3.3 * R1)/(R1 + R2)
plot(R1, R2, V)

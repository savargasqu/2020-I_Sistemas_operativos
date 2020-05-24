import matplotlib.pyplot as plt

sizes = [] # in kB
for i in range(6):
    sizes.append(10**(i))

times_bfiles = [
    0.041,
    0.005,
    0.011,
    0.059,
    0.543,
    5.028]

times_pipes = [
    0.006,
    0.008,
    0.045,
    0.388,
    3.206,
    32.471]

times_shmem = [
    0.004,
    0.004,
    0.006,
    0.019,
    0.138,
    1.290]

times_sockets = [
    0.010,
    0.015,
    0.064,
    0.565,
    4.688,
    47.362]

plt.xlabel('Tamaño de datos (kB)')
plt.ylabel('Tiempo (s)')

plt.plot(sizes, times_bfiles, 'k')
plt.plot(sizes, times_pipes, 'r')
plt.plot(sizes, times_shmem, 'g')
plt.plot(sizes, times_sockets, 'b')

plt.grid()
plt.xscale('log')
plt.yscale('log')
plt.savefig('results_graph.png')


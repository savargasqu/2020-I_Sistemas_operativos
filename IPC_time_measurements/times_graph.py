import matplotlib.pyplot as plt

sizes = [] # in kB
for i in range(6):
    sizes.append(10**(i))

bin_files_times = [
    0.015,
    0.004,
    0.007,
    0.037,
    0.325,
    3.297]

pipes_times = [
    0.005,
    0.008,
    0.026,
    0.212,
    2.075,
    21.460]

shared_mem_times = [
    0.007,
    0.008,
    0.006,
    0.009,
    None,
    None]

sockets_times = [
    0.009,
    0.018,
    0.064,
    0.565,
    5.490,
    54.540]

plt.xlabel('Tamaño de datos (kB)')
plt.ylabel('Tiempo (s)')

plt.plot(sizes, bin_files_times, 'k')
plt.plot(sizes, pipes_times, 'r')
plt.plot(sizes, shared_mem_times, 'g')
plt.plot(sizes, sockets_times, 'b')

plt.grid()
plt.xscale('log')
plt.savefig('results_graph.png')


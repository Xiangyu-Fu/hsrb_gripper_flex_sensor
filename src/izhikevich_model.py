import matplotlib.pyplot as plt
import numpy as np

# Define the model parameters
a, b, c, d = 0.02, 0.2, -65, 8
v = c  # Initial membrane potential
u = b * v  # Initial recovery variable
I = 10  # External current

# Time parameters
t_max = 100  # Total time to simulate
dt = 0.1  # Time step

# Simulation
time = np.arange(0, t_max, dt)
v_values = []
u_values = []

for t in time:
    # Update the membrane potential and recovery variable
    dv = 0.04 * v ** 2 + 5 * v + 140 - u + I
    du = a * (b * v - u)
    v += dv * dt
    u += du * dt

    # Spike condition
    if v >= 30:
        v = c
        u += d

    # Record the values
    v_values.append(v)
    u_values.append(u)

# Plotting
plt.figure(figsize=(12, 6))
plt.subplot(211)
plt.title("Izhikevich Model Simulation")
plt.plot(time, v_values, label="Membrane Potential (v)")
plt.ylabel("Membrane Potential (mV)")
plt.legend()

plt.subplot(212)
plt.plot(time, u_values, label="Recovery Variable (u)")
plt.xlabel("Time (ms)")
plt.ylabel("Recovery Variable")
plt.legend()

plt.show()


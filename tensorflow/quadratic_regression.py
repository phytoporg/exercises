import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt

def generate_points(params, num_points, domain):
    domain_width = domain[1] - domain[0]
    domain_halfwidth = domain_width / 2.0
    x_values = [x * domain_width - domain_halfwidth for x in np.random.random_sample((num_points,))]
    a = params[0]
    b = params[1]
    c = params[2]

    # TODO: add noise
    noise = np.random.normal(0.0, 10.0, (num_points,))
    return x_values, [a*x*x + b*x + c + noise[i] for i, x in enumerate(x_values)]

def main():
    # Choose some random parameters between 0 and 10
    samples = np.random.random_sample((3,))
    params = [p * 10.0 for p in samples]

    # Just plot the samples for now...
    x_values, y_values = generate_points(params, 100, [-10, 10])

    # Construct design matrix X from (x, y) above
    X = np.array([[[1, x, x*x] for x in x_values]])

    # TODO ...

    #_, ax = plt.subplots(1, 1)
    #ax.plot(x_values, y_values, 'o')

    #plt.show()

if __name__=="__main__":
   main() 


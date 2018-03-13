import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt

def model(params, x):
    a, b, c = params
    return a*x*x + b*x + c

def generate_points(params, num_points, domain):
    domain_width = domain[1] - domain[0]
    domain_halfwidth = domain_width / 2.0
    x_values = sorted([x * domain_width - domain_halfwidth for x in np.random.random_sample((num_points,))])
    noise = np.random.normal(0.0, 10.0, (num_points,))
    return x_values, [model(params, x) + noise[i] for i, x in enumerate(x_values)]

def main():
    samples = np.random.random_sample((3,))
    params = [p * 10.0 for p in samples]
    x_values, y_values = generate_points(params, 100, [-10, 10])

    # Construct design matrix X from (x, y) above
    X = tf.constant(np.array([[x*x, x, 1.0] for x in x_values]), dtype=tf.float32, name="X")
    y = tf.constant(np.array(y_values).reshape(-1, 1), dtype=tf.float32, name="y")

    XT = tf.transpose(X)
    theta = tf.matmul(tf.matmul(tf.matrix_inverse(tf.matmul(XT, X)), XT), y)

    with tf.Session() as sess:
        solved_params = theta.eval()
        print("Original parameters: {0}".format(params))
        print("Solved parameters: {0}".format(solved_params.flatten()))

    _, ax = plt.subplots(1, 1)
    ax.plot(x_values, y_values, 'bo')

    a, b, c = solved_params
    ax.plot(x_values, [model(params, x) for x in x_values], 'r-')

    plt.show()

if __name__=="__main__":
   main() 


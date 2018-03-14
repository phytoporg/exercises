import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
from sklearn.preprocessing import scale

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

    #
    # TODO: Rescale solved parameters afterwards...
    #

    # Construct design matrix X from (x, y) above
    normalized_data = scale(np.array([[x*x, x, 1.0] for x in x_values]))

    # THERE IS A BUG IN HERE :(
    theta = tf.Variable(tf.random_uniform([3, 1], -1.0, 1.0), name="theta")
    X = tf.constant(normalized_data, dtype=tf.float32, name="X")
    y = tf.constant(np.array(y_values).reshape(-1, 1), dtype=tf.float32, name="y")
    y_pred = tf.matmul(X, theta, name="predictions")
    error = y_pred - y
    mse = tf.reduce_mean(tf.square(error), name="mse")
    print(X.shape[0])
    gradients = 2/100 * tf.matmul(tf.transpose(X), error)
    training_op = tf.assign(theta, theta - 0.01 * gradients)

    init = tf.global_variables_initializer()

    with tf.Session() as sess:
        sess.run(init)

        for epoch in range(1000):
            if epoch % 100 == 0:
                print("Epoch {0}, mse = {1}".format(epoch, mse.eval()))
            sess.run(training_op)

        solved_params = theta.eval()
        print("Original parameters: {0}".format(params))
        print("Solved parameters: {0}".format(solved_params.flatten()))

    _, ax = plt.subplots(1, 1)
    ax.plot(x_values, y_values, 'bo')

    a, b, c = solved_params
    ax.plot(x_values, [model(solved_params, x) for x in x_values], 'r-')

    plt.show()

if __name__=="__main__":
   main() 


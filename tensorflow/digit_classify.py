import tensorflow as tf
import numpy as np
import argparse
import os
import cv2

#
# MNISTy, but applied to our scenario!
#
def cnn_model(features, labels, mode):
    print(features["x"].shape)
    #input_layer = tf.reshape(features["x"], [-1, 28, 28, 1])
    input_layer = tf.reshape(features["x"], [-1, 30, 30, 1])
    print(input_layer)

    # Conv layer 1
    conv1 = tf.layers.conv2d(
        inputs=input_layer,
        filters=32,
        kernel_size=5,
        padding="same",
        activation=tf.nn.relu
    )

    # Pooling layer 1
    pool1 = tf.layers.max_pooling2d(inputs=conv1, pool_size=[2, 2], strides=2)

    # Convolutional layer #2
    conv2 = tf.layers.conv2d(
        inputs=pool1,
        filters=64,
        kernel_size=5,
        padding="same",
        activation=tf.nn.relu
    )

    # Pooling layer 2
    pool2 = tf.layers.max_pooling2d(inputs=conv2, pool_size=[2, 2], strides=2)

    # Dense layer
    _, w, h, _ = pool2.shape
    pool2_flat = tf.reshape(pool2, [-1, w * h * 64])
    dense = tf.layers.dense(inputs=pool2_flat, units=1024, activation=tf.nn.relu)

    #
    # TODO: Maybe try batch normalization instead?
    #
    dropout = tf.layers.dropout(
        inputs=dense,
        rate=0.4,
        training = mode == tf.estimator.ModeKeys.TRAIN
    )

    #
    # Logits (?) layer
    #
    logits = tf.layers.dense(inputs=dropout, units=10)

    predictions = {
        # Generate predictions
        "classes": tf.argmax(input=logits, axis=1),
        "probabilities": tf.nn.softmax(logits, name="softmax_tensor")
    }

    if mode == tf.estimator.ModeKeys.PREDICT:
        return tf.estimator.EstimatorSpec(mode=mode, predictions=predictions)

    #
    # Compute loss - TODO: onehot encoding stuff
    #
    onehot_labels = tf.one_hot(indices=tf.cast(labels, tf.int32), depth=10)
    loss = tf.losses.softmax_cross_entropy(onehot_labels=onehot_labels, logits=logits)


    #
    # Configure training op
    #
    if mode == tf.estimator.ModeKeys.TRAIN:
        optimizer = tf.train.GradientDescentOptimizer(learning_rate=0.0001)
        train_op = optimizer.minimize(
            loss=loss,
            global_step=tf.train.get_global_step())
        return tf.estimator.EstimatorSpec(mode=mode, loss=loss, train_op=train_op)

    #
    # Add evaluation metrics for EVAL mode
    #
    eval_metric_ops = {
        "accuracy" : tf.metrics.accuracy(
            labels=labels,
            predictions=predictions["classes"]
            )
    }

    return tf.estimator.EstimatorSpec(mode=mode, loss=loss, eval_metric_ops=eval_metric_ops)

def load_digits(path):
    labels = []
    digits = []

    for root, _, files in os.walk(path):
        for f in files:
            if ".png" in f:
                cur_dir = root.split('\\')[-1]
                if cur_dir == "none":
                    continue

                labels.append(int(cur_dir))

                full_path = os.path.join(root, f)
                img = cv2.resize(cv2.imread(full_path), (30, 30))
                binary_image = cv2.inRange(img, np.array([50, 50, 50]), np.array([255, 255, 255]))
                digits.append(np.asarray(binary_image, dtype=np.float32))

    return (np.array(labels), np.array(digits))

def main(args):
    if not os.path.exists(args.root_dir):
        print("{} is not a valid root path! Bailing.".format(args.root_dir))
        exit(-1)

    tf.logging.set_verbosity(tf.logging.INFO)

    print("Loading digits...", end='')
    labels, images = load_digits(args.root_dir)
    print("done!")
    
    training_ratio = .80
    num_samples = len(labels)
    num_training = int(num_samples * training_ratio)
    indices = np.random.shuffle([i for i in range(num_samples)])

    labels[:] = labels[indices]
    images[:] = images[indices]

    training_labels, training_images = labels[:num_training].reshape((-1,)), images[:num_training].reshape((-1, 900))
    testing_labels, testing_images = labels[num_training:].reshape((-1,)), images[num_training:].reshape((-1, 900))
    
    tensors_to_log = { "probabilities" : "softmax_tensor" }
    logging_hook = tf.train.LoggingTensorHook(tensors=tensors_to_log, every_n_iter=50)

    classifier = tf.estimator.Estimator(model_fn=cnn_model, model_dir=args.model_out_path)
    train_input_fn = tf.estimator.inputs.numpy_input_fn(
        x={"x" : training_images},
        y=training_labels,
        batch_size=100,
        num_epochs=None,
        shuffle=True
    )

    classifier.train(input_fn=train_input_fn, steps=50000, hooks=[logging_hook])

    eval_input_fn = tf.estimator.inputs.numpy_input_fn(
        x={"x" : testing_images},
        y=testing_labels,
        num_epochs=1,
        shuffle=False
    )

    eval_results = classifier.evaluate(input_fn=eval_input_fn)
    print(eval_results)

if __name__=="__main__":
    parser = argparse.ArgumentParser(description="Train some digits, yo")
    parser.add_argument("--root-dir", type=str, help="Path to digit dirs")
    parser.add_argument("--model-out-path", type=str, help="Path to save the tensorflow model")

    main(parser.parse_args())

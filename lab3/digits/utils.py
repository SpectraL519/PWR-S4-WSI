# ------------------------------------
# Imports

import sys
import os
import zipfile
import pathlib

import numpy as np
import tensorflow as tf

import cv2
import matplotlib.pyplot as plt

this = sys.modules[__name__]



# ------------------------------------
# Constants

this.IMAGE_SIZE = (28, 28)
this.IMAGE_SHAPE = this.IMAGE_SIZE + (1,)
this.BATCH_SIZE = 32
this.EPOCHS = 7



# ------------------------------------
# Functions

def get_zipped_data(zip_path: str) -> tuple:
    zip_ref = zipfile.ZipFile(zip_path, "r")
    zip_ref.extractall()
    zip_ref.close()

    data, labels = [], []

    dataset_path = zip_path.removesuffix('.zip')
    for root, dirs, files in os.walk(dataset_path):
        for file in files:
            if file.endswith(".jpg"):
                img = cv2.imread(os.path.join(root, file), cv2.IMREAD_GRAYSCALE)
                img = cv2.resize(img, (28, 28), interpolation=cv2.INTER_LINEAR)
                img = img / 255.
                img = cv2.bitwise_not(img)
                img = np.expand_dims(img, -1)
                data.append(img)
                labels.append(int(os.path.split(root)[-1]))

    return np.array(data), np.array(labels)
    

def prep_data(train_data, test_data, info):
    train_data = train_data.map(prep_image, num_parallel_calls=tf.data.AUTOTUNE)
    train_data = train_data.cache()
    train_data = train_data.shuffle(info.splits['train'].num_examples)
    train_data = train_data.batch(this.BATCH_SIZE)
    train_data = train_data.prefetch(tf.data.AUTOTUNE)

    test_data = test_data.map(prep_image, num_parallel_calls=tf.data.AUTOTUNE)
    test_data = test_data.cache()
    test_data = test_data.shuffle(info.splits['test'].num_examples)
    test_data = test_data.batch(this.BATCH_SIZE)
    test_data = test_data.prefetch(tf.data.AUTOTUNE)

    return train_data, test_data, info


def prep_image(image, label):
    image = tf.cast(image, tf.float32) / 255.
    image = tf.image.resize(image, size=this.IMAGE_SIZE)
    return image, label


def build_model(**kwargs) -> tf.keras.Model:
    filters=kwargs.get('filters', 10) 
    kernel_size=kwargs.get('kernel_size', 2)
    conv_activation=kwargs.get('conv_activation', 'relu')
    padding=kwargs.get('padding', 'same')

    model = tf.keras.Sequential([
        tf.keras.layers.Conv2D(
            filters=filters,
            kernel_size=kernel_size,
            activation=conv_activation,
            padding=padding,
            input_shape=this.IMAGE_SHAPE,
        ),
        tf.keras.layers.Conv2D(
            filters=filters,
            kernel_size=kernel_size,
            activation=conv_activation,
            padding=padding
        ),
        tf.keras.layers.Conv2D(
            filters=filters,
            kernel_size=kernel_size,
            activation=conv_activation,
            padding=padding
        ),
        tf.keras.layers.Flatten(),
        tf.keras.layers.Dense(10, activation=kwargs.get('out_activation', 'softmax')) 
    ])

    return model


def plot_loss_curves(
    history: tf.keras.callbacks.History, 
    figsize: tuple = (12, 6)
):
    loss = history.history['loss']
    val_loss = history.history['val_loss']
    accuracy = history.history['sparse_categorical_accuracy']
    val_accuracy = history.history['val_sparse_categorical_accuracy']
    epochs = range(len(history.history['loss']))

    plt.figure(figsize=figsize)

    # Plot loss
    loss_ax = plt.subplot2grid((1, 2), (0, 0), colspan=1, rowspan=1)
    loss_ax.plot(epochs, loss, label='train_loss')
    loss_ax.plot(epochs, val_loss, label='valid_loss')
    loss_ax.set_title('Loss')
    loss_ax.set_xlabel('Epochs')
    loss_ax.legend()

    # Plot accuracy
    acc_ax = plt.subplot2grid((1, 2), (0, 1), colspan=1, rowspan=1)
    acc_ax.plot(epochs, accuracy, label='train_accuracy')
    acc_ax.plot(epochs, val_accuracy, label='valid_accuracy')
    acc_ax.set_title('Accuracy')
    acc_ax.set_xlabel('Epochs')
    acc_ax.legend()

    plt.show();


def get_prediction(
    model: tf.keras.Model,
    image: tf.Tensor,
) -> int:
    pred = model.predict(tf.expand_dims(image, axis=0))
    pred_digit = tf.argmax(pred[0])
    return pred_digit

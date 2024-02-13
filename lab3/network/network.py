import random
import numpy as np

from activation import Activation
from metrics import Metric
from losses import Loss
from normalizers import Normalizer



class FeedForwardDenseNetwork:
    def __init__(self, 
        neurons: list, # including input and output layers
        activation: Activation,
        metric: Metric,
        loss: Loss,
        normalizer: Normalizer = None
    ):
        self.num_layers = len(neurons)
        self.num_hidden_layers = self.num_layers - 1
        assert(self.num_layers > 1)

        self.neurons = neurons
        self.W = [np.random.randn(y, x) for x, y in zip(self.neurons[:-1], self.neurons[1:])] # weights
        self.B = [np.zeros((y, 1)) for y in self.neurons[1:]] # biases
        self.A = [None] * self.num_hidden_layers # activations
        self.Z = [None] * self.num_hidden_layers # weghted sums

        self.activation = activation
        self.metric = metric
        self.loss = loss
        self.normalizer = normalizer


    def summary(self):
        print('\n'.join([
            f"Number of layers: {self.num_layers}",
            f"Number of neurons per layer:",
            '\n'.join([f"\t{i + 1}: {n}" for i, n in enumerate(self.neurons)]),
            f"Activation: {self.activation.name()}",
            f"Metric: {self.metric.name()}",
            f"Loss: {self.loss.name()}",
            f"Normalization: {self.normalizer.name() if self.normalizer else 'none'}"
        ]))


    def _feed_forward(self, X: np.ndarray):
        for i, (b, w) in enumerate(zip(self.B, self.W)):
            self.Z[i] = np.dot(w, (X if i == 0 else self.A[i - 1])) + b
            self.A[i] = self.activation.calculate(self.Z[i])

    
    def _back_propagate(self, 
        X: np.ndarray, 
        Y: np.ndarray, 
        learning_rate: float
    ):
        m = X.shape[1]

        # derivative vectors
        dB = [None] * self.num_hidden_layers
        dW = [None] * self.num_hidden_layers
        dZ = [None] * self.num_hidden_layers

        # output layer
        dZ[-1] = self.A[-1] - Y
        dW[-1] = np.dot(dZ[-1], self.A[-2].T) / m
        dB[-1] = np.sum(dZ[-1], axis=1, keepdims=True) / m

        # hidden layers
        for i in reversed(range(0, self.num_hidden_layers - 1)):
            dZ[i] = np.dot(self.W[i + 1].T, dZ[i + 1]) * self.activation.calulate_derivative(self.Z[i])
            dW[i] = np.dot(dZ[i], (X.T if i == 0 else self.A[i - 1].T)) / m
            dB[i] = np.sum(dZ[i], axis=1, keepdims=True) / m

        for i in range(self.num_hidden_layers):
            self.W[i] -= learning_rate * dW[i]
            self.B[i] -= learning_rate * dB[i]


    def fit(self, 
        X: np.ndarray, 
        Y: np.ndarray, 
        learning_rate: float = 0.001, 
        iterations: int = 1000,
        verbose: bool = True
    ):
        if self.normalizer:
            X = self.normalizer.normalize(X)

        step = iterations / 10
        for i in range(iterations):
            self._feed_forward(X)
            self._back_propagate(X, Y, learning_rate)
            if verbose and (i + 1) % step == 0:
                print(
                    f"iter {i + 1}: loss = {round(self.loss.calculate(self.A[-1], Y), 4)}",
                    flush=True
                )


    def predict(self, X):
        if (self.normalizer):
            X = self.normalizer.normalize(X)

        self._feed_forward(X)
        return np.where(self.A[-1] >= 0.5, 1, 0)

    def evaluate(self, X, Y):
        if (self.normalizer):
            X = self.normalizer.normalize(X)

        pred = self.predict(X)
        loss = self.loss.calculate(pred, Y)
        metric = self.metric.calculate(pred, Y)
        return loss, metric

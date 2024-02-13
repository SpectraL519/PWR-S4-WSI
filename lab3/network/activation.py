import numpy as np
from abc import ABC, abstractmethod



class Activation(ABC):
    @staticmethod
    @abstractmethod
    def name():
        pass

    @staticmethod
    @abstractmethod
    def calculate(x: np.ndarray):
        pass

    @staticmethod
    @abstractmethod
    def calulate_derivative(x: np.ndarray):
        pass


class Sigmoid(Activation):
    @staticmethod
    def name():
        return "sigmoid"

    @staticmethod
    def calculate(x: np.ndarray):
        return 1.0 / (1.0 + np.exp(-x))

    @staticmethod
    def calulate_derivative(x: np.ndarray):
        sig = Sigmoid.calculate(x)
        return sig * (1 - sig)
    

class Relu(Activation):
    @staticmethod
    def name():
        return "relu"

    @staticmethod
    def calculate(x: np.ndarray):
        return np.maximum(0, x)
    
    @staticmethod
    def calulate_derivative(x: np.ndarray):
        return np.where(x > 0, 1, 0)
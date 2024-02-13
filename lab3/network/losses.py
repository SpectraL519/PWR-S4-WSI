import numpy as np
from abc import ABC, abstractmethod



class Loss(ABC):
    @staticmethod
    @abstractmethod
    def name():
        pass

    @staticmethod
    @abstractmethod
    def calculate(y_pred: np.ndarray, y_true: np.ndarray):
        pass


class MSE(Loss):
    @staticmethod
    def name():
        return "mean_sqared_error"

    @staticmethod
    def calculate(y_pred: np.ndarray, y_true: np.ndarray):
        return np.mean(np.square(y_pred - y_true))
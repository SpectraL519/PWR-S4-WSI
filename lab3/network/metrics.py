import numpy as np
from abc import ABC, abstractmethod



class Metric(ABC):
    @staticmethod
    @abstractmethod
    def name():
        pass

    @staticmethod
    @abstractmethod
    def calculate(y_pred: np.ndarray, y_true: np.ndarray):
        pass


class Accuracy(Metric):
    @staticmethod
    def name():
        return "accuracy"

    @staticmethod
    def calculate(y_pred: np.ndarray, y_true: np.ndarray):
        return np.sum(y_pred == y_true) / len(y_true)
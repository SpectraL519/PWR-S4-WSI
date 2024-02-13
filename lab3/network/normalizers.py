import numpy as np
from abc import ABC, abstractmethod



class Normalizer(ABC):
    @staticmethod
    @abstractmethod
    def name():
        pass

    @staticmethod
    @abstractmethod
    def normalize(data: np.ndarray):
        pass


class L1(Normalizer):
    @staticmethod
    def name():
        return "L1"

    @staticmethod
    def normalize(data: np.ndarray):
        return data / np.linalg.norm(data, ord=1)


class L2(Normalizer):
    @staticmethod
    def name():
        return "L2"

    @staticmethod
    def normalize(data: np.ndarray):
        return data / np.linalg.norm(data, ord=2)
import numpy as np

class Network(object):
    def __init__(self, sizes):
        self.num_layer = len(sizes)
        self.sizes = sizes
        self.biases = [np.random.randn(y,1) for y in sizes[1:]]
        self.weights = [np.random.randn(y, x) in zip(sizes[:-1], sizes[1:])]

    def sigmoid(self, z):
        return 1.0/(1.0+np.exp(-z))
    
    def feedforward(self, a):
        for b, w in zip(self.biases, self.weights):
            a = self.sigmoid(np.dot(w,a) + b)
        return a
    
    def SGD(self, training_data, epochs, mini_batch_size, eta, test_data=None):
        if test_data:
            n_test = len(test_data)
        n = len(training_data)

        for j in range(epochs):
            np.random.shuffle(training_data)
            mini_batches = [training_data[k:k+mini_batch_size] for k in range(0, n, mini_batch_size)]
            for mini_batch in mini_batches:
                self.update_mini_batch(mini_batch, eta)
            if test_data:
                print(f'Epoc{j} : {self.evaluate(test_data)} / {n_test}')
            else:
                print(f'Epoch {j} complete')
    
    def update_mini_batch(self, mini_batch, eta):
        pass
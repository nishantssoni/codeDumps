import torch
import torch.nn as nn
import math

class InputEmbedding(nn.Module):
    def __init__(self, d_model: int,vocab_size: int):
        super().__init__()
        self.d_model = d_model
        self.embedding = nn.Embedding(vocab_size, d_model)

    def forward(self, x):
        return self.embedding(x) * math.sqrt(self.d_model)
    

class PositionalEncoding(nn.Module):
    def __init__(self, d_model: int, seq_len: int, dropout: float = 0.1):
        super().__init__()
        self.d_model = d_model # embedding dimension
        self.seq_len = seq_len # sequence length (number of tokens)
        self.dropout = nn.Dropout(p=dropout) # dropout (to avoid overfitting)

        # create a matrix of shape (seq_len, d_model)
        pe = torch.zeros(seq_len, d_model)

        # calculate the positional encoding
        # crate a vecotr of shape (seq_len, 1)
        position = torch.arange(0, seq_len, dtype=torch.float).unsqueeze(1)
        div_term = torch.exp(torch.arange(0, d_model, 2).float() * (-math.log(10000.0) / d_model))
        
        # apply the positional encoding
        pe[:, 0, 0::2] = torch.sin(position * div_term)
        pe[:, 0, 1::2] = torch.cos(position * div_term)

        # we will have batch of sentences so we need to unsqueeze the positional encoding
        # shape (1, seq_len, d_model) 
        pe = pe.unsqueeze(0)

        # register the buffer
        self.register_buffer('pe', pe)

    def forward(self, x):
        # add the positional encoding and didn't train it
        x = x + self.pe[:, :x.shape[1], :].requires_grad_(False)
        return self.dropout(x)


if __name__ == "__main__":
    model = InputEmbedding(512, 100)
    x = torch.randint(0, 100, (64, 32))
    out = model(x)
    print(out.shape)
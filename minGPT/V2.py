import torch
import torch.nn as nn
from torch.nn import functional as F

# hyperparameteres
block_size = 32
batch_size = 8
max_iters = 5000
eval_interval = 500
learning_rate = 1e-3
device = 'cuda' if torch.cuda.is_available() else 'cpu'
print("current device running on :: ", device )
eval_iters = 200
n_embd = 32

# -------------------------------
# seed
torch.manual_seed(1337)

# read the data
with open("data.txt" , 'r',encoding='utf-8') as f:
    text = f.read()

# all the unique characters that occurs in the text
chars = sorted(list(set(text)))
vocab_size = len(chars)

# mappint from character to integers and vice-versa
stoi = {ch:i for i, ch in enumerate(chars)}
itos = {i:ch for i, ch in enumerate(chars)}
encode = lambda s: [stoi[c] for c in s]
decode = lambda s: (''.join([itos[c] for c in s]))

# train and test split
data = torch.tensor(encode(text), dtype=torch.long)
n = int(0.9 * len(data))
train_data = data[:n]
val_data = data[n:]

# data loading
def get_batch(split):
    # generate a small size batches of inputs x and target y
    data = train_data if split == 'train' else val_data
    ix = torch.randint(len(data) - block_size, (batch_size,))
    x = torch.stack([data[i:i+block_size] for i in ix])
    y = torch.stack([data[i+1:i+block_size+1] for i in ix])
    x, y = x.to(device), y.to(device)
    return x, y


@torch.no_grad()
def estimate_loss():
    out = {}
    model.eval()
    for split in ['train', 'val']:
        losses = torch.zeros(eval_iters)
        for k in range(eval_iters):
            X, Y = get_batch(split)
            logits, loss = model(X, Y)
            losses[k] = loss.item()
        out[split] = losses.mean()
    model.train()
    return out



class Head(nn.Module):
    def __init__(self, head_size):
        super().__init__()
        self.key = nn.Linear(n_embd, head_size, bias=False)
        self.query = nn.Linear(n_embd, head_size, bias=False)
        self.value = nn.Linear(n_embd, head_size, bias=False)
        self.register_buffer('tril', torch.tril(torch.ones(block_size, block_size)))

    def forward(self, x):
        B, T, C = x.shape
        k = self.key(x)   #(B, T, C)
        q = self.query(x) #(B, T, C)
        
        # compute attention score(affinities)
        wei = q @ k.transpose(-2,-1) * C**-0.5  #(B, T, 16) @ (B, 16, T)--->(B, T, T)
        wei = wei.masked_fill(self.tril[:T, :T] == 0, float('-inf'))
        wei = F.softmax(wei, dim = -1) #(B, T, T)
        
        # perform the weighted aggregation of the values
        v = self.value(x) #(B, T, C)
        out = wei @ v  #(B, T, T) @ (B, T, C) --> (B, T, C)
        return out

 
class MultipleHeadAttention(nn.Module):
    """Multiple heads of self attention"""
    def __init__(self, num_heads, head_size):
        super().__init__()
        self.heads = nn.ModuleList([Head(head_size) for _ in range(num_heads)])
    
    def forward(self, x):
        return torch.cat([h(x) for h in self.heads], dim=-1)


class FeedFoward(nn.Module):
    """ a simple linear layer followed by a non-linearity """

    def __init__(self, n_embd):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(n_embd, n_embd),
            nn.ReLU(),
        )

    def forward(self, x):
        return self.net(x)

class Block(nn.Module):
    """ Transformer block: communication followed by computation """

    def __init__(self, n_embd, n_head):
        # n_embd: embedding dimension, n_head: the number of heads we'd like
        super().__init__()
        head_size = n_embd // n_head
        self.sa = MultipleHeadAttention(n_head, head_size)
        self.ffwd = FeedFoward(n_embd)

    def forward(self, x):
        x =self.sa(x)
        x =self.ffwd(x)
        return x


# simple bigram language model loading
class BigramLanguageModel(nn.Module):
    def __init__(self):
        super().__init__()
        self.token_embedding_table = nn.Embedding(vocab_size, n_embd)
        self.position_embedding_table = nn.Embedding(block_size, n_embd)
        self.blocks = nn.Sequential(
            Block(n_embd, n_head=4),
            Block(n_embd, n_head=4),
            Block(n_embd, n_head=4),
        )
        self.lm_head = nn.Linear(n_embd, vocab_size)
    
    def forward(self, idx, targets=None):
        B, T = idx.shape
        token_emb = self.token_embedding_table(idx)  #(B, T, C)
        pos_emb = self.position_embedding_table(torch.arange(T, device=device)) #(T, C)
        x = token_emb + pos_emb #(B, T, C)
        x = self.blocks(x)
        logits = self.lm_head(x) #(B, T, vocab_size)
        
        if targets is None:
            loss = 0
        else:
            B, T, C = logits.shape
            logits = logits.view(B*T, C)
            targets = targets.view(B*T)
            loss = F.cross_entropy(logits, targets)

        return logits, loss

    def generate(self, idx, max_new_tokens):
        # idx is (B, T) array of indecis of current contex
        for _ in range(max_new_tokens):
            idx_cond = idx[:, -block_size:]
            logits, loss = self(idx_cond)  #call forward and get prediction
            logits = logits[:,-1, :] #becomes (B, C)
            probs = F.softmax(logits, dim=1) #softmax to get probablities
            idx_next = torch.multinomial(probs, num_samples=1)  #(B, 1)
            idx = torch.cat((idx, idx_next), dim=1)  #(B, T+1)
        return idx

model = BigramLanguageModel()
m = model.to(device)


# creating python optimizer
optimizer = torch.optim.AdamW(m.parameters(), lr=learning_rate)


for iter in range(max_iters):
    # ever once or which eval the loss on train and val sets
    if iter % eval_interval == 0:
        losses = estimate_loss()
        print(f"step {iter}: train loss {losses['train']:.4f}, val loss {losses['val']:.4f}")
    
    # sample a batch of data
    xb, yb = get_batch('train')

    # eval the loss
    logits, loss = m(xb, yb)
    optimizer.zero_grad(set_to_none=True)
    loss.backward()
    optimizer.step()

# generate from model
contex = torch.zeros((1,1), dtype=torch.long, device=device)
print(decode(m.generate(idx = contex, max_new_tokens=300)[0].tolist()))
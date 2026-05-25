"""
Deep Q-Network (DQN) Maze — GPU Edition
====================================
Upgraded to use PyTorch and CUDA for neural network training.
"""

import pygame
import numpy as np
import sys
import math
import random
from collections import deque

import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F

# ─────────────────────────────────────────────
# 0.  Hardware Setup (Your 3050 Ti)
# ─────────────────────────────────────────────

# This forces PyTorch to use your NVIDIA GPU
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print(f"Neural Network running on: {device}")
if device.type == "cuda":
    print(f"GPU Detected: {torch.cuda.get_device_name(0)}")

# ─────────────────────────────────────────────
# 1.  Maze Definition  (0=open, 1=wall, 2=trap, 3=goal)
# ─────────────────────────────────────────────

MAZE = np.array([
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    [1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1],
    [1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1],
    [1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1],
    [1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1],
    [1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1],
    [1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1],
    [1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1],
    [1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1],
    [1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1],
    [1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1],
    [1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 3, 1],
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
], dtype=int)

ROWS, COLS = MAZE.shape
START = (1, 1)
GOAL  = tuple(zip(*np.where(MAZE == 3)))[0]   # (13, 13)
STATE_SIZE = ROWS * COLS  # Flattened grid for neural net input

# ─────────────────────────────────────────────
# 2.  Display Constants
# ─────────────────────────────────────────────

CELL      = 52
HUD_H     = 110
WIN_W     = COLS * CELL
WIN_H     = ROWS * CELL + HUD_H

BG          = (15,  17,  26)
WALL_DARK   = (38,  42,  58)
WALL_LIGHT  = (60,  66,  88)
FLOOR       = (24,  28,  42)
GOAL_COL    = (255, 210,  50)
AGENT_COL   = (80, 180, 255)
AGENT_EYE   = (20,  20,  40)
ARROW_COL   = (120, 200, 120)
ARROW_WEAK  = (50,  80,  50)
TEXT_BRIGHT = (230, 235, 255)
TEXT_DIM    = (120, 130, 160)
HUD_BG      = (10,  12,  20)
FLASH_G     = (60, 220, 100)
FLASH_R     = (220,  60,  80)

# ─────────────────────────────────────────────
# 3.  Neural Network & DQN Setup
# ─────────────────────────────────────────────

ACTIONS      = [(-1, 0), (1, 0), (0, -1), (0, 1)]   # Up Down Left Right
NUM_ACTIONS  = 4

# Neural Network Architecture
class DQN(nn.Module):
    def __init__(self, input_dim, output_dim):
        super(DQN, self).__init__()
        # 3 fully connected layers
        self.fc1 = nn.Linear(input_dim, 128)
        self.fc2 = nn.Linear(128, 128)
        self.fc3 = nn.Linear(128, output_dim)

    def forward(self, x):
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        return self.fc3(x)

# DQN Hyper-parameters
BATCH_SIZE   = 64
GAMMA        = 0.95
LR           = 0.001
TARGET_UPDATE= 10     # Update target network every 10 episodes
MEMORY_SIZE  = 10000

EPSILON_START= 1.0
EPSILON_MIN  = 0.05
EPSILON_DECAY= 0.985
TOTAL_EPISODES = 200
MAX_STEPS    = 300    

def get_state_tensor(r, c):
    """Converts a grid position into a 1D tensor for the neural network"""
    state = np.zeros(STATE_SIZE, dtype=np.float32)
    state[r * COLS + c] = 1.0
    return torch.tensor(state, dtype=torch.float32, device=device)

# ─────────────────────────────────────────────
# 4.  Helper Utilities
# ─────────────────────────────────────────────

def cell_rect(r, c):
    return pygame.Rect(c * CELL, r * CELL, CELL, CELL)

def cell_center(r, c):
    return (c * CELL + CELL // 2, r * CELL + CELL // 2)

def lerp_color(a, b, t):
    t = max(0.0, min(1.0, t))
    return tuple(int(a[i] + (b[i] - a[i]) * t) for i in range(3))

def draw_arrow(surf, cx, cy, dr, dc, strength, cell_size):
    half = cell_size // 2 - 6
    tip_x, tip_y = cx + dc * half, cy + dr * half
    base_x, base_y = cx - dc * (half // 2), cy - dr * (half // 2)
    t = min(1.0, max(0.0, (strength + 20) / 120))
    col = lerp_color(ARROW_WEAK, ARROW_COL, t)
    pygame.draw.line(surf, col, (base_x, base_y), (tip_x, tip_y), 2)
    perp = (-dc, dr)
    head_len = 5
    h1 = (tip_x - dc * head_len + perp[0] * head_len // 2, tip_y - dr * head_len + perp[1] * head_len // 2)
    h2 = (tip_x - dc * head_len - perp[0] * head_len // 2, tip_y - dr * head_len - perp[1] * head_len // 2)
    pygame.draw.polygon(surf, col, [(tip_x, tip_y), h1, h2])

def phase_label(eps):
    if eps > 0.6: return "Exploring", (220, 100,  80)
    elif eps > 0.2: return "Transitioning", (220, 180,  60)
    else: return "Exploiting", (80,  210, 120)

# ─────────────────────────────────────────────
# 5.  Main Loop
# ─────────────────────────────────────────────

def main():
    pygame.init()
    screen = pygame.display.set_mode((WIN_W, WIN_H))
    pygame.display.set_caption("Deep Q-Network Maze — RTX 3050 Ti Edition")
    clock = pygame.time.Clock()

    font_lg = pygame.font.SysFont("consolas", 18, bold=True)
    font_md = pygame.font.SysFont("consolas", 14)
    font_sm = pygame.font.SysFont("consolas", 11)

    # ── Initialize DQN Models ──────────────────
    policy_net = DQN(STATE_SIZE, NUM_ACTIONS).to(device)
    target_net = DQN(STATE_SIZE, NUM_ACTIONS).to(device)
    target_net.load_state_dict(policy_net.state_dict())
    target_net.eval() # Target net doesn't train, it just gives stable goals

    optimizer = optim.Adam(policy_net.parameters(), lr=LR)
    memory = deque(maxlen=MEMORY_SIZE)

    # ── State ──────────────────────────────────
    visit_count  = np.zeros((ROWS, COLS), dtype=int)
    episode      = 0
    epsilon      = EPSILON_START
    best_steps   = None
    history      = []
    flash_timer  = 0
    flash_col    = FLASH_G
    paused       = False
    speed_delay  = 5
    agent        = list(START)
    step_count   = 0
    ep_done      = False
    last_dir     = (0, 1)

    def reset_episode():
        nonlocal agent, step_count, ep_done
        agent      = list(START)
        step_count = 0
        ep_done    = False

    # ── Pre-render static maze ───────────────
    maze_surf = pygame.Surface((WIN_W, ROWS * CELL))
    maze_surf.fill(BG)
    for r in range(ROWS):
        for c in range(COLS):
            rect = cell_rect(r, c)
            if MAZE[r, c] == 1:
                pygame.draw.rect(maze_surf, WALL_DARK, rect)
                pygame.draw.line(maze_surf, WALL_LIGHT, rect.topleft, rect.topright, 1)
                pygame.draw.line(maze_surf, WALL_LIGHT, rect.topleft, rect.bottomleft, 1)
            else:
                pygame.draw.rect(maze_surf, FLOOR, rect)
            pygame.draw.rect(maze_surf, BG, rect, 1)

    # ── Training Function ──────────────────────
    def optimize_model():
        if len(memory) < BATCH_SIZE:
            return # Wait until we have enough memories to form a batch
        
        # Pull a random batch of memories
        transitions = random.sample(memory, BATCH_SIZE)
        batch_state, batch_action, batch_reward, batch_next, batch_done = zip(*transitions)

        # Convert to PyTorch tensors on your 3050 Ti
        state_batch = torch.stack(batch_state)
        action_batch = torch.tensor(batch_action, device=device).unsqueeze(1)
        reward_batch = torch.tensor(batch_reward, dtype=torch.float32, device=device)
        next_state_batch = torch.stack(batch_next)
        done_batch = torch.tensor(batch_done, dtype=torch.float32, device=device)

        # Compute Q(s, a) using Policy Net
        state_action_values = policy_net(state_batch).gather(1, action_batch).squeeze()

        # Compute V(s') using Target Net for stability
        next_state_values = target_net(next_state_batch).max(1)[0].detach()
        # If the state was a game-over, expected future reward is 0
        expected_state_action_values = reward_batch + (GAMMA * next_state_values * (1 - done_batch))

        # Calculate Loss (Huber Loss is stable)
        loss = F.smooth_l1_loss(state_action_values, expected_state_action_values)

        # Backpropagation
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

    # ── Main loop ──────────────────────────────
    running = True
    while running:
        clock.tick(60)

        for ev in pygame.event.get():
            if ev.type == pygame.QUIT: running = False
            elif ev.type == pygame.KEYDOWN:
                if ev.key in (pygame.K_q, pygame.K_ESCAPE): running = False
                elif ev.key == pygame.K_SPACE: paused = not paused
                elif ev.key in (pygame.K_PLUS, pygame.K_EQUALS): speed_delay = max(0, speed_delay - 10)
                elif ev.key in (pygame.K_MINUS): speed_delay = min(200, speed_delay + 10)

        if not paused and not ep_done and episode < TOTAL_EPISODES:
            pygame.time.delay(speed_delay)

            r, c = agent
            state_tensor = get_state_tensor(r, c)

            # Choose action (Epsilon Greedy)
            if random.random() < epsilon:
                a = random.randrange(NUM_ACTIONS)
            else:
                with torch.no_grad():
                    q_values = policy_net(state_tensor)
                    a = q_values.argmax().item()

            dr, dc = ACTIONS[a]
            last_dir = (dr, dc)
            nr, nc = r + dr, c + dc

            # Environment physics
            cell_val = MAZE[nr, nc]
            is_done = False
            
            if cell_val == 1:                      # Wall
                reward = -5.0
                next_pos = [r, c]                  # Bounce back
                flash_col, flash_timer = FLASH_R, 4
            elif cell_val == 3:                    # Goal
                reward = 100.0
                next_pos = [nr, nc]
                is_done = True
                ep_done = True
                flash_col, flash_timer = FLASH_G, 10
            else:                                  # Floor
                reward = -0.1
                next_pos = [nr, nc]

            if step_count >= MAX_STEPS:
                is_done = True
                ep_done = True

            next_state_tensor = get_state_tensor(*next_pos)

            # Store experience in memory buffer
            memory.append((state_tensor, a, reward, next_state_tensor, is_done))

            # Train the neural network
            optimize_model()

            agent = next_pos
            visit_count[next_pos[0], next_pos[1]] += 1
            step_count += 1

        if ep_done and episode < TOTAL_EPISODES:
            history.append(step_count)
            if best_steps is None or (step_count < best_steps and MAZE[agent[0], agent[1]] == 3):
                best_steps = step_count
            epsilon = max(EPSILON_MIN, epsilon * EPSILON_DECAY)
            episode += 1
            
            # Sync Target Network periodically
            if episode % TARGET_UPDATE == 0:
                target_net.load_state_dict(policy_net.state_dict())
                
            reset_episode()

        # ── Visual Rendering ─────────────────────
        screen.fill(BG)
        screen.blit(maze_surf, (0, 0))

        # Render Heatmap
        max_v = max(1, visit_count.max())
        heat_surf = pygame.Surface((WIN_W, ROWS * CELL), pygame.SRCALPHA)
        for r in range(ROWS):
            for c in range(COLS):
                if MAZE[r, c] != 1 and visit_count[r, c] > 0:
                    t = visit_count[r, c] / max_v
                    col = lerp_color((20, 40, 100), (40, 140, 255), t)
                    s = pygame.Surface((CELL, CELL), pygame.SRCALPHA)
                    s.fill((*col, int(30 + 120 * t)))
                    heat_surf.blit(s, cell_rect(r, c).topleft)
        screen.blit(heat_surf, (0, 0))

        # Query Neural Network for entire grid to draw Green Arrows
        if not paused and len(memory) > BATCH_SIZE:
            with torch.no_grad():
                # We build a batch of all empty floor coordinates to query the GPU once per frame
                open_cells = [(r, c) for r in range(ROWS) for c in range(COLS) if MAZE[r, c] != 1]
                if open_cells:
                    states_batch = torch.stack([get_state_tensor(r, c) for r, c in open_cells])
                    q_vals_batch = policy_net(states_batch) # GPU prediction for the whole board
                    
                    for i, (r, c) in enumerate(open_cells):
                        q_vals = q_vals_batch[i]
                        best_a = q_vals.argmax().item()
                        best_v = q_vals[best_a].item()
                        
                        if best_v > 0.1: # Only draw if the network has a confident opinion
                            dr, dc = ACTIONS[best_a]
                            draw_arrow(screen, cell_center(r,c)[0], cell_center(r,c)[1], dr, dc, best_v, CELL)

        # Draw Goal
        gx, gy = cell_center(*GOAL)
        pulse = 0.5 + 0.5 * math.sin(pygame.time.get_ticks() * 0.005)
        gsize = int(CELL * 0.35 + 4 * pulse)
        pygame.draw.circle(screen, (*GOAL_COL, 80), (gx, gy), gsize + 8)
        pygame.draw.circle(screen, GOAL_COL, (gx, gy), gsize)

        if flash_timer > 0:
            fsurf = pygame.Surface((WIN_W, ROWS * CELL), pygame.SRCALPHA)
            fsurf.fill((*flash_col, int(60 * flash_timer / 10)))
            screen.blit(fsurf, (0, 0))
            flash_timer -= 1

        # Draw Agent
        ax, ay = cell_center(*agent)
        bob = int(3 * math.sin(pygame.time.get_ticks() * 0.008))
        radius = CELL // 2 - 6
        pygame.draw.circle(screen, AGENT_COL, (ax, ay + bob), radius)
        eye_x, eye_y = ax + last_dir[1] * (radius // 2), ay + bob + last_dir[0] * (radius // 2)
        pygame.draw.circle(screen, AGENT_EYE, (eye_x, eye_y), 4)
        pygame.draw.circle(screen, TEXT_BRIGHT, (ax, ay + bob), radius, 2)

        # HUD
        hud_y = ROWS * CELL
        pygame.draw.rect(screen, HUD_BG, (0, hud_y, WIN_W, HUD_H))
        pygame.draw.line(screen, WALL_LIGHT, (0, hud_y), (WIN_W, hud_y), 1)

        phase_txt, phase_col = phase_label(epsilon)
        cols_hud = [
            ("EPISODE",  f"{min(episode + 1, TOTAL_EPISODES)} / {TOTAL_EPISODES}"),
            ("STEPS",    str(step_count)),
            ("BEST",     str(best_steps) if best_steps else "—"),
            ("ε (epsilon)", f"{epsilon:.3f}"),
            ("NETWORK",  "TRAINING" if len(memory) >= BATCH_SIZE else "WARMING UP"),
            ("SPEED",    f"{max(1, 100 - speed_delay)}%"),
        ]
        col_w = WIN_W // len(cols_hud)
        for i, (label, value) in enumerate(cols_hud):
            x = i * col_w + col_w // 2
            lbl_surf  = font_sm.render(label, True, TEXT_DIM)
            val_col   = phase_col if label == "NETWORK" else TEXT_BRIGHT
            val_surf  = font_lg.render(value, True, val_col)
            screen.blit(lbl_surf, lbl_surf.get_rect(center=(x, hud_y + 22)))
            screen.blit(val_surf, val_surf.get_rect(center=(x, hud_y + 50)))

        pygame.display.flip()

    pygame.quit()
    sys.exit()

if __name__ == "__main__":
    main()
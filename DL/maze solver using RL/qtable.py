"""
Q-Learning Maze — Enhanced Edition
====================================
A visually rich Q-learning demo with:
  • 13×15 complex maze with dead-ends and long corridors
  • Animated agent with direction indicator
  • Trail / heat-map showing visited cells
  • Q-value arrows drawn on every cell
  • Live HUD: episode, steps, epsilon, best score
  • Colour-coded reward flashes (green=goal, red=wall)
  • Speed control (+/-) and pause (SPACE)
  • Phase label: Exploring → Transitioning → Exploiting

Controls
--------
  SPACE   – pause / resume
  +  / =  – speed up
  -       – slow down
  R       – reset everything
  Q / ESC – quit
"""

import pygame
import numpy as np
import sys
import math

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

# ─────────────────────────────────────────────
# 2.  Display Constants
# ─────────────────────────────────────────────

CELL      = 52          # pixels per cell
HUD_H     = 110         # pixels for bottom HUD
WIN_W     = COLS * CELL
WIN_H     = ROWS * CELL + HUD_H

# Palette
BG          = (15,  17,  26)
WALL_DARK   = (38,  42,  58)
WALL_LIGHT  = (60,  66,  88)
FLOOR       = (24,  28,  42)
GOAL_COL    = (255, 210,  50)
AGENT_COL   = (80, 180, 255)
AGENT_EYE   = (20,  20,  40)
TRAIL_BASE  = (30,  60, 120)
ARROW_COL   = (120, 200, 120)
ARROW_WEAK  = (50,  80,  50)
TEXT_BRIGHT = (230, 235, 255)
TEXT_DIM    = (120, 130, 160)
HUD_BG      = (10,  12,  20)
FLASH_G     = (60, 220, 100)
FLASH_R     = (220,  60,  80)

# ─────────────────────────────────────────────
# 3.  Q-Learning Hyper-parameters
# ─────────────────────────────────────────────

ACTIONS      = [(-1, 0), (1, 0), (0, -1), (0, 1)]   # Up Down Left Right
NUM_ACTIONS  = 4
ALPHA        = 0.15     # learning rate
GAMMA        = 0.92     # discount
EPSILON_START= 1.0
EPSILON_MIN  = 0.04
EPSILON_DECAY= 0.97
TOTAL_EPISODES = 120
MAX_STEPS    = 800      # per episode safety cap

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
    """Draw a small arrow from centre toward (dr,dc) direction."""
    half = cell_size // 2 - 6
    tip_x = cx + dc * half
    tip_y = cy + dr * half
    base_x = cx - dc * (half // 2)
    base_y = cy - dr * (half // 2)
    t = min(1.0, max(0.0, (strength + 20) / 120))
    col = lerp_color(ARROW_WEAK, ARROW_COL, t)
    pygame.draw.line(surf, col, (base_x, base_y), (tip_x, tip_y), 2)
    # arrowhead
    perp = (-dc, dr)
    head_len = 5
    h1 = (tip_x - dc * head_len + perp[0] * head_len // 2,
          tip_y - dr * head_len + perp[1] * head_len // 2)
    h2 = (tip_x - dc * head_len - perp[0] * head_len // 2,
          tip_y - dr * head_len - perp[1] * head_len // 2)
    pygame.draw.polygon(surf, col, [(tip_x, tip_y), h1, h2])

def phase_label(eps):
    if eps > 0.6:
        return "Exploring", (220, 100,  80)
    elif eps > 0.2:
        return "Transitioning", (220, 180,  60)
    else:
        return "Exploiting", (80,  210, 120)

# ─────────────────────────────────────────────
# 5.  Main
# ─────────────────────────────────────────────

def main():
    pygame.init()
    screen = pygame.display.set_mode((WIN_W, WIN_H))
    pygame.display.set_caption("Q-Learning Maze — Enhanced Edition")
    clock  = pygame.time.Clock()

    font_lg  = pygame.font.SysFont("consolas", 18, bold=True)
    font_md  = pygame.font.SysFont("consolas", 14)
    font_sm  = pygame.font.SysFont("consolas", 11)

    # ── State ──────────────────────────────────
    q_table      = np.zeros((ROWS, COLS, NUM_ACTIONS))
    visit_count  = np.zeros((ROWS, COLS), dtype=int)   # heat-map
    episode      = 0
    epsilon      = EPSILON_START
    best_steps   = None
    history      = []          # steps per episode
    flash_timer  = 0           # frames remaining for reward flash
    flash_col    = FLASH_G
    paused       = False
    speed_delay  = 25          # ms between steps
    agent        = list(START)
    step_count   = 0
    ep_done      = False
    last_dir     = (0, 1)      # for agent drawing

    def reset_episode():
        nonlocal agent, step_count, ep_done
        agent      = list(START)
        step_count = 0
        ep_done    = False

    reset_episode()

    # ── Pre-render static maze surface ─────────
    maze_surf = pygame.Surface((WIN_W, ROWS * CELL))
    maze_surf.fill(BG)

    # draw checker pattern on floor cells
    for r in range(ROWS):
        for c in range(COLS):
            rect = cell_rect(r, c)
            if MAZE[r, c] == 1:
                pygame.draw.rect(maze_surf, WALL_DARK, rect)
                # bevel highlight
                pygame.draw.line(maze_surf, WALL_LIGHT,
                                 rect.topleft, rect.topright, 1)
                pygame.draw.line(maze_surf, WALL_LIGHT,
                                 rect.topleft, rect.bottomleft, 1)
            else:
                pygame.draw.rect(maze_surf, FLOOR, rect)
            pygame.draw.rect(maze_surf, BG, rect, 1)   # grid line

    # ── Main loop ──────────────────────────────
    running = True
    while running:
        dt = clock.tick(60)

        # ── Events ─────────────────────────────
        for ev in pygame.event.get():
            if ev.type == pygame.QUIT:
                running = False
            elif ev.type == pygame.KEYDOWN:
                if ev.key in (pygame.K_q, pygame.K_ESCAPE):
                    running = False
                elif ev.key == pygame.K_SPACE:
                    paused = not paused
                elif ev.key in (pygame.K_PLUS, pygame.K_EQUALS, pygame.K_KP_PLUS):
                    speed_delay = max(5, speed_delay - 10)
                elif ev.key in (pygame.K_MINUS, pygame.K_KP_MINUS):
                    speed_delay = min(200, speed_delay + 10)
                elif ev.key == pygame.K_r:
                    q_table[:]     = 0
                    visit_count[:] = 0
                    episode        = 0
                    epsilon        = EPSILON_START
                    best_steps     = None
                    history.clear()
                    flash_timer    = 0
                    reset_episode()

        # ── Step logic (throttled) ──────────────
        if not paused and not ep_done and episode < TOTAL_EPISODES:
            pygame.time.delay(speed_delay)

            r, c = agent
            # choose action
            if np.random.rand() < epsilon:
                a = np.random.randint(NUM_ACTIONS)
            else:
                a = int(np.argmax(q_table[r, c]))

            dr, dc = ACTIONS[a]
            last_dir = (dr, dc)
            nr, nc = r + dr, c + dc

            # reward + transition
            cell_val = MAZE[nr, nc]
            if cell_val == 1:                      # wall
                reward   = -8
                next_pos = [r, c]
                flash_col, flash_timer = FLASH_R, 4
            elif cell_val == 3:                    # goal
                reward   = 100
                next_pos = [nr, nc]
                ep_done  = True
                flash_col, flash_timer = FLASH_G, 10
            else:                                  # open floor
                reward   = -1
                next_pos = [nr, nc]

            # bellman update
            old_q      = q_table[r, c, a]
            max_next   = np.max(q_table[next_pos[0], next_pos[1]])
            q_table[r, c, a] = old_q + ALPHA * (reward + GAMMA * max_next - old_q)

            agent = next_pos
            visit_count[next_pos[0], next_pos[1]] += 1
            step_count += 1

            if step_count >= MAX_STEPS:
                ep_done = True

        # episode bookkeeping
        if ep_done and episode < TOTAL_EPISODES:
            history.append(step_count)
            if best_steps is None or step_count < best_steps:
                best_steps = step_count
            epsilon = max(EPSILON_MIN, epsilon * EPSILON_DECAY)
            episode += 1
            reset_episode()

        # ── Draw ───────────────────────────────
        screen.fill(BG)

        # -- maze base
        screen.blit(maze_surf, (0, 0))

        # -- heat-map overlay
        max_v = max(1, visit_count.max())
        heat_surf = pygame.Surface((WIN_W, ROWS * CELL), pygame.SRCALPHA)
        for r in range(ROWS):
            for c in range(COLS):
                if MAZE[r, c] != 1 and visit_count[r, c] > 0:
                    t   = visit_count[r, c] / max_v
                    col = lerp_color((20, 40, 100), (40, 140, 255), t)
                    alpha = int(30 + 120 * t)
                    rect = cell_rect(r, c)
                    s = pygame.Surface((CELL, CELL), pygame.SRCALPHA)
                    s.fill((*col, alpha))
                    heat_surf.blit(s, rect.topleft)
        screen.blit(heat_surf, (0, 0))

        # -- Q-value arrows (skip walls)
        for r in range(ROWS):
            for c in range(COLS):
                if MAZE[r, c] == 1:
                    continue
                cx, cy = cell_center(r, c)
                best_a = int(np.argmax(q_table[r, c]))
                best_v = q_table[r, c, best_a]
                if abs(best_v) > 0.5:
                    dr, dc = ACTIONS[best_a]
                    draw_arrow(screen, cx, cy, dr, dc, best_v, CELL)

        # -- goal cell glow
        gr, gc = GOAL
        gx, gy = cell_center(gr, gc)
        pulse   = 0.5 + 0.5 * math.sin(pygame.time.get_ticks() * 0.005)
        gsize   = int(CELL * 0.35 + 4 * pulse)
        pygame.draw.circle(screen, (*GOAL_COL, 80),
                           (gx, gy), gsize + 8)
        pygame.draw.circle(screen, GOAL_COL, (gx, gy), gsize)
        lbl = font_sm.render("GOAL", True, (40, 30, 0))
        screen.blit(lbl, lbl.get_rect(center=(gx, gy)))

        # -- reward flash overlay
        if flash_timer > 0:
            fsurf = pygame.Surface((WIN_W, ROWS * CELL), pygame.SRCALPHA)
            alpha = int(60 * flash_timer / 10)
            fsurf.fill((*flash_col, alpha))
            screen.blit(fsurf, (0, 0))
            flash_timer -= 1

        # -- agent
        ar, ac = agent
        ax, ay = cell_center(ar, ac)
        bob     = int(3 * math.sin(pygame.time.get_ticks() * 0.008))
        radius  = CELL // 2 - 6
        pygame.draw.circle(screen, AGENT_COL, (ax, ay + bob), radius)
        # direction dot
        eye_x = ax + last_dir[1] * (radius // 2)
        eye_y = ay + bob + last_dir[0] * (radius // 2)
        pygame.draw.circle(screen, AGENT_EYE, (eye_x, eye_y), 4)
        # white outline
        pygame.draw.circle(screen, TEXT_BRIGHT, (ax, ay + bob), radius, 2)

        # ── HUD ────────────────────────────────
        hud_y = ROWS * CELL
        pygame.draw.rect(screen, HUD_BG, (0, hud_y, WIN_W, HUD_H))
        pygame.draw.line(screen, WALL_LIGHT, (0, hud_y), (WIN_W, hud_y), 1)

        phase_txt, phase_col = phase_label(epsilon)

        cols_hud = [
            ("EPISODE",  f"{min(episode + 1, TOTAL_EPISODES)} / {TOTAL_EPISODES}"),
            ("STEPS",    str(step_count)),
            ("BEST",     str(best_steps) if best_steps else "—"),
            ("ε (epsilon)", f"{epsilon:.3f}"),
            ("PHASE",    phase_txt),
            ("SPEED",    f"{max(1, 100 - speed_delay)}%"),
        ]
        col_w = WIN_W // len(cols_hud)
        for i, (label, value) in enumerate(cols_hud):
            x = i * col_w + col_w // 2
            lbl_surf  = font_sm.render(label, True, TEXT_DIM)
            val_col   = phase_col if label == "PHASE" else TEXT_BRIGHT
            val_surf  = font_lg.render(value, True, val_col)
            screen.blit(lbl_surf, lbl_surf.get_rect(center=(x, hud_y + 22)))
            screen.blit(val_surf, val_surf.get_rect(center=(x, hud_y + 50)))

        # mini step-history bar
        if history:
            bar_x, bar_y = 10, hud_y + 80
            bar_w, bar_h = WIN_W - 20, 18
            pygame.draw.rect(screen, WALL_DARK, (bar_x, bar_y, bar_w, bar_h), border_radius=3)
            max_h = max(history)
            for i, s in enumerate(history[-bar_w:]):
                bx  = bar_x + i
                bh  = max(2, int(bar_h * s / max(1, max_h)))
                t   = 1.0 - s / max(1, max_h)
                bc  = lerp_color((180, 60, 60), (60, 210, 120), t)
                pygame.draw.line(screen, bc, (bx, bar_y + bar_h), (bx, bar_y + bar_h - bh), 1)
            hint = font_sm.render("steps/episode (green=fewer)", True, TEXT_DIM)
            screen.blit(hint, (bar_x + 2, bar_y + 2))

        # controls hint
        ctrl = font_sm.render("SPACE pause  +/- speed  R reset  Q quit", True, TEXT_DIM)
        screen.blit(ctrl, ctrl.get_rect(bottomright=(WIN_W - 6, WIN_H - 3)))

        # paused banner
        if paused:
            banner = font_lg.render("— PAUSED —", True, (255, 200, 60))
            screen.blit(banner, banner.get_rect(center=(WIN_W // 2, ROWS * CELL // 2)))

        # all-done banner
        if episode >= TOTAL_EPISODES and ep_done:
            overlay = pygame.Surface((WIN_W, ROWS * CELL), pygame.SRCALPHA)
            overlay.fill((0, 0, 0, 140))
            screen.blit(overlay, (0, 0))
            done_txt = font_lg.render(
                f"Training complete!  Best: {best_steps} steps — press R to restart",
                True, GOAL_COL)
            screen.blit(done_txt, done_txt.get_rect(center=(WIN_W // 2, ROWS * CELL // 2)))

        pygame.display.flip()

    pygame.quit()
    sys.exit()


if __name__ == "__main__":
    main()
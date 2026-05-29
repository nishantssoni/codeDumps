import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

X = np.array([
    [1.0, 2.0, 1.0], [4.0, 5.0, 1.0], [7.0, 3.0, 1.0], [3.0, 1.0, 1.0],
    [2.0, 6.0, 1.0], [1.5, 4.0, 1.0], [6.0, 1.0, 1.0], [8.0, 4.0, 1.0], [5.0, 2.0, 1.0]
])
y = np.array([1, 1, -1, -1, 1, 1, -1, -1, -1])

weights = np.array([1.0, -1.0, -6.0])
learning_rate = 0.5

state = "WAITING"
frame_counter = 0
PAUSE_FRAMES = 25
TRANSITION_FRAMES = 20

current_idx = 0
epoch = 1
old_weights = weights.copy()
target_weights = weights.copy()
is_correct = False

# We use this to track if we've successfully classified all points in a row
correct_streak = 0 

fig, ax = plt.subplots(figsize=(6, 6))
fig.patch.set_facecolor("#1a1a2e")
ax.set_facecolor("#16213e")
ax.set_xlim(-1, 11)
ax.set_ylim(-1, 11)
ax.tick_params(colors="#aaaacc")

for spine in ax.spines.values():
    spine.set_edgecolor("#333355")
ax.grid(True, linestyle="--", alpha=0.25, color="#aaaacc")
ax.set_title("Perceptron Learning", color="#e0e0ff", fontsize=13, pad=12)

pos_mask = y == 1
neg_mask = y == -1
ax.scatter(X[pos_mask, 0], X[pos_mask, 1], color="#00e676", marker="+", s=200, linewidths=2, zorder=4, label="Positive (+1)")
ax.scatter(X[neg_mask, 0], X[neg_mask, 1], color="#ff5252", marker="_", s=200, linewidths=2, zorder=4, label="Negative (−1)")

highlight, = ax.plot([], [], "o", color="#ffd600", markersize=18, fillstyle="none", markeredgewidth=2, zorder=5, alpha=0)
decision_ln, = ax.plot([], [], color="#ff6ec7", linewidth=2.5, solid_capstyle="round", zorder=3, label="Decision boundary")

info_box = ax.text(
    0.02, 0.98, "", transform=ax.transAxes, va="top", ha="left",
    fontsize=8, color="#e0e0ff",
    bbox=dict(boxstyle="round,pad=0.5", facecolor="#0f3460", edgecolor="#4444aa", alpha=0.92),
    linespacing=1.6,
)

ax.legend(loc="lower right", facecolor="#0f3460", edgecolor="#4444aa", labelcolor="#e0e0ff", fontsize=8)

pos_region = None
neg_region = None

def decision_boundary_xy(w):
    a, b, c = w
    x_plot = np.array([-2.0, 13.0])
    if abs(b) < 1e-9:
        xv = -c / a
        return np.array([xv, xv]), np.array([-2.0, 13.0])
    y_plot = (-a * x_plot - c) / b
    return x_plot, y_plot

def set_line(w):
    global pos_region, neg_region
    
    if pos_region is not None:
        try: pos_region.remove()
        except: pass
    if neg_region is not None:
        try: neg_region.remove()
        except: pass

    xv, yv = decision_boundary_xy(w)
    decision_ln.set_data(xv, yv)

    a, b, c = w
    if abs(b) < 1e-9:
        if a > 0:
            pos_region = ax.fill_betweenx([-2.0, 13.0], xv[0], 13.0, color="#00e676", alpha=0.15, zorder=1)
            neg_region = ax.fill_betweenx([-2.0, 13.0], -2.0, xv[0], color="#ff5252", alpha=0.15, zorder=1)
        else:
            pos_region = ax.fill_betweenx([-2.0, 13.0], -2.0, xv[0], color="#00e676", alpha=0.15, zorder=1)
            neg_region = ax.fill_betweenx([-2.0, 13.0], xv[0], 13.0, color="#ff5252", alpha=0.15, zorder=1)
    else:
        if b > 0:
            pos_region = ax.fill_between(xv, yv, 13.0, color="#00e676", alpha=0.15, zorder=1)
            neg_region = ax.fill_between(xv, yv, -2.0, color="#ff5252", alpha=0.15, zorder=1)
        else:
            pos_region = ax.fill_between(xv, yv, -2.0, color="#00e676", alpha=0.15, zorder=1)
            neg_region = ax.fill_between(xv, yv, 13.0, color="#ff5252", alpha=0.15, zorder=1)

def build_info(pt, lbl, status, w):
    sign = "+" if lbl == 1 else "−"
    eq = (f"{w[0]:+.2f}·x  {w[1]:+.2f}·y  {w[2]:+.2f}  =  0")
    return (
        f"Epoch : {epoch}   |   Point {current_idx+1}/{len(X)}\n"
        f"Target: ({pt[0]:.0f}, {pt[1]:.0f})  [{sign}]\n"
        f"Status: {status}\n"
        f"Line  : {eq}"
    )

def init():
    set_line(weights)
    highlight.set_alpha(0)
    info_box.set_text("Waiting for user...")
    return decision_ln, highlight, info_box

def update(_frame):
    global weights, current_idx, epoch
    global state, frame_counter
    global old_weights, target_weights, is_correct, correct_streak

    if state == "WAITING":
        return decision_ln, highlight, info_box

    # If we already finished, just hold the final screen
    if state == "CONVERGED":
        highlight.set_alpha(0)
        info_box.set_text(f"Epoch : {epoch}\nStatus: ★ CONVERGED! ★\nAll points perfectly classified.")
        # Stops matplotlib from needlessly running updates in the background
        ani.event_source.stop() 
        return decision_ln, highlight, info_box

    point = X[current_idx]
    true_label = y[current_idx]

    if state == "EVALUATE":
        if frame_counter == 0:
            highlight.set_data([point[0]], [point[1]])
            highlight.set_alpha(1.0)

            score = np.dot(weights, point)
            pred_label = 1 if score >= 0 else -1
            is_correct = (pred_label == true_label)

            if is_correct:
                correct_streak += 1
            else:
                correct_streak = 0
                old_weights = weights.copy()
                target_weights = weights + learning_rate * true_label * point

            # Check if we just solved the whole dataset perfectly
            if correct_streak >= len(X):
                state = "CONVERGED"
                return decision_ln, highlight, info_box

        status = "✓  Correct — moving on" if is_correct else "✗  Misclassified — updating line"
        info_box.set_text(build_info(point, true_label, status, weights))

        frame_counter += 1
        if frame_counter >= PAUSE_FRAMES:
            frame_counter = 0
            highlight.set_alpha(0)
            if is_correct:
                _advance_point()
            else:
                state = "TRANSITION"

    elif state == "TRANSITION":
        t = frame_counter / float(TRANSITION_FRAMES)
        t = t * t * (3 - 2 * t)
        weights = old_weights * (1 - t) + target_weights * t

        info_box.set_text(build_info(point, true_label, "↻  Updating …", weights))

        frame_counter += 1
        if frame_counter > TRANSITION_FRAMES:
            weights = target_weights.copy()
            state = "EVALUATE"
            frame_counter = 0
            _advance_point()

    set_line(weights)
    return decision_ln, highlight, info_box

def _advance_point():
    global current_idx, epoch
    current_idx += 1
    if current_idx >= len(X):
        current_idx = 0
        epoch += 1

start_text = ax.text(5, 5, "Click Anywhere to Start!", 
                     color="white", fontsize=14, ha="center", va="center", weight="bold",
                     bbox=dict(facecolor="#ff5252", alpha=0.9, edgecolor="none", boxstyle="round,pad=1"), zorder=10)

def on_click(event):
    global state
    if state == "WAITING":
        state = "EVALUATE"
        start_text.set_visible(False)
        fig.canvas.draw()

fig.canvas.mpl_connect('button_press_event', on_click)

# Dramatically increased frames so it doesn't time out early
ani = FuncAnimation(fig, update, frames=5000, init_func=init, blit=False, interval=10, repeat=False)

plt.tight_layout()
plt.show()
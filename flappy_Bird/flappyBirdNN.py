from ursina import *
import random as r
import math
import time
import copy

# ---------------------------
# Neural network + GA helpers
# ---------------------------

def sigmoid(x):
    return 1 / (1 + math.exp(-x))

class NeuralNetwork:
    # Simple fully connected feedforward network
    def __init__(self, layer_sizes=None, weights=None, biases=None):
        if layer_sizes is None:
            layer_sizes = [3, 6, 1]  # default: 3 inputs -> 6 hidden -> 1 output
        self.sizes = layer_sizes
        if weights is None:
            # weights[l] maps from layer l to l+1, shape: (next_size, cur_size)
            self.weights = [ [[(r.random()*2-1) for _ in range(self.sizes[i])] for __ in range(self.sizes[i+1])]
                              for i in range(len(self.sizes)-1) ]
        else:
            self.weights = weights
        if biases is None:
            self.biases = [ [(r.random()*2-1) for _ in range(self.sizes[i+1])] for i in range(len(self.sizes)-1)]
        else:
            self.biases = biases

    def forward(self, inputs):
        a = inputs[:]
        for w, b in zip(self.weights, self.biases):
            next_a = []
            for neuron_w, neuron_b in zip(w, b):
                z = neuron_b
                # dot product
                for wi, ai in zip(neuron_w, a):
                    z += wi * ai
                next_a.append(sigmoid(z))
            a = next_a
        return a

    def copy(self):
        new_w = [ [ list(neuron_w) for neuron_w in layer ] for layer in self.weights ]
        new_b = [ list(layer) for layer in self.biases ]
        return NeuralNetwork(layer_sizes=list(self.sizes), weights=new_w, biases=new_b)

    def mutate(self, rate=0.1, scale=0.5):
        # mutate weights and biases with given probability and step size
        for l in range(len(self.weights)):
            for i in range(len(self.weights[l])):
                for j in range(len(self.weights[l][i])):
                    if r.random() < rate:
                        self.weights[l][i][j] += (r.random()*2-1) * scale
            for i in range(len(self.biases[l])):
                if r.random() < rate:
                    self.biases[l][i] += (r.random()*2-1) * scale

    @staticmethod
    def crossover(a, b):
        # simple uniform crossover
        child = a.copy()
        for l in range(len(child.weights)):
            for i in range(len(child.weights[l])):
                for j in range(len(child.weights[l][i])):
                    if r.random() < 0.5:
                        child.weights[l][i][j] = b.weights[l][i][j]
            for i in range(len(child.biases[l])):
                if r.random() < 0.5:
                    child.biases[l][i] = b.biases[l][i]
        return child

# ---------------------------
# Game + Agent definitions
# ---------------------------

app = Ursina()
window.title = "Flappy Bird - NN + GA"
Sky()
camera.orthographic = True
camera.fov = 20

# Pipe template
pipe_template = Entity(
    model='quad',
    color=color.green,
    texture='white_cube',
    position=(20, 10),
    scale=(3, 15, 1),
    collider='box',
    enabled=False
)

# Simulation parameters
POPULATION = 20
GAP = 20
PIPE_INTERVAL = 4.0
PIPE_SPEED = 5.0
MUTATION_RATE = 0.5
MUTATION_SCALE = 2.0
ELITISM = 4

# Globals
pipes = []  # list of tuples (top_pipe, bottom_pipe)
pipe_timer = 0.0
generation = 1
best_score_ever = 0

# Agent wrapper: holds a bird entity + NN + state
class Agent:
    def __init__(self, x= -6, y=0, network=None):
        # Visual / physics state
        self.entity = Animation(
            r'assets_flappyBird/img',
            collider='box',
            scale=(2,2,2),
            position=(x,y,0)
        )
        self.velocity = 0.0
        self.alive = True
        self.fitness = 0.0
        self.score = 0  # number of pipes passed

        # Brain
        if network is None:
            self.brain = NeuralNetwork(layer_sizes=[3,6,1])
        else:
            self.brain = network

    def reset(self):
        self.entity.enabled = True
        self.entity.x = -6
        self.entity.y = 0
        self.velocity = 0.0
        self.alive = True
        self.fitness = 0.0
        self.score = 0

    def kill(self):
        self.alive = False
        self.entity.enabled = False

    def think_and_act(self, nearest_pipe):
        # Inputs: normalized horizontal distance to pipe, vertical offset to gap center, velocity
        # All inputs scaled to roughly [-1,1] or [0,1]
        if nearest_pipe is None:
            # no pipes yet; small random chance to flap
            inputs = [1.0, 0.0, 0.0]
        else:
            pipe_x = nearest_pipe.x
            gap_center_y = nearest_pipe.user_data['gap_y']
            # horizontal distance (bird.x -> pipe_x) normalized
            dx = (pipe_x - self.entity.x) / 40.0   # 40 is game width-ish scaling
            dy = (gap_center_y - self.entity.y) / 20.0
            vel = self.velocity / 10.0
            inputs = [dx, dy, vel]
        out = self.brain.forward(inputs)[0]  # single output in [0,1]
        if out > 0.55:
            self.flap()

    def flap(self):
        self.velocity = 3.0

# Create initial population
population = [Agent(y=0) for _ in range(POPULATION)]

# ---------------------------
# Pipe spawning / helpers
# ---------------------------
def spawn_pipe():
    y = r.randint(4, 12)
    top = duplicate(pipe_template, y=y, x=20, enabled=True)
    bottom = duplicate(pipe_template, y=y - GAP - pipe_template.scale_y, x=20, enabled=True)
    # we will store an entity for pair info on top
    top.user_data = {'is_top': True, 'pair': bottom, 'gap_y': y - GAP/2}
    bottom.user_data = {'is_top': False, 'pair': top, 'gap_y': y - GAP/2}
    pipes.append((top, bottom))

def reset_pipes():
    global pipes
    for t,b in pipes:
        destroy(t, delay=0)
        destroy(b, delay=0)
    pipes = []

# ---------------------------
# Utility: find nearest pipe for an agent
# ---------------------------
def nearest_pipe_for(agent):
    # Return top pipe entity with x > agent.x (the upcoming pipe). We'll store gap center in user_data.
    candidates = []
    for top, bottom in pipes:
        if top.x + (top.scale_x/2) >= agent.entity.x - 0.1:
            candidates.append(top)
    if not candidates:
        return None
    nearest = min(candidates, key=lambda p: p.x - agent.entity.x)
    # ensure gap_y saved
    if 'gap_y' not in nearest.user_data:
        top_y = nearest.y
        gap_center = top_y - GAP/2
        nearest.user_data['gap_y'] = gap_center
    return nearest

# ---------------------------
# Evolution functions
# ---------------------------
def evaluate_fitness_and_select():
    # sort population by fitness (which we update during death) and return sorted list
    pop_sorted = sorted(population, key=lambda a: a.fitness, reverse=True)
    return pop_sorted

def new_generation():
    global population, generation, best_score_ever
    print(f'--- Generation {generation} ended ---')
    # Evaluate fitness
    for agent in population:
        # final fitness already updated in death or during run; if alive at reset, ensure it's reflected
        pass

    pop_sorted = evaluate_fitness_and_select()
    best = pop_sorted[0]
    if best.score > best_score_ever:
        best_score_ever = best.score
    print(f'Best fitness: {best.fitness:.2f}, score: {best.score}, best ever score: {best_score_ever}')

    # keep elites
    new_pop = []
    for i in range(ELITISM):
        elite_net = pop_sorted[i].brain.copy()
        new_pop.append(Agent(network=elite_net))
    # fill rest via crossover + mutation
    while len(new_pop) < POPULATION:
        parent_a = tournament_select(pop_sorted)
        parent_b = tournament_select(pop_sorted)
        child_net = NeuralNetwork.crossover(parent_a.brain, parent_b.brain)
        child_net.mutate(rate=MUTATION_RATE, scale=MUTATION_SCALE)
        new_pop.append(Agent(network=child_net))
    # replace population
    population = new_pop
    # reset pipes & timers
    reset_pipes()
    spawn_pipe()
    global pipe_timer
    pipe_timer = 0.0
    # reset agents positions
    for a in population:
        a.reset()
    generation += 1

def tournament_select(sorted_pop, k=4):
    # randomly pick k and return the best among them
    sample = [ r.choice(sorted_pop) for _ in range(k) ]
    return max(sample, key=lambda a: a.fitness)

# ---------------------------
# Game loop
# ---------------------------

# spawn first pipe
spawn_pipe()

def update():
    global pipe_timer
    # spawn pipes at interval
    pipe_timer += time.dt
    if pipe_timer >= PIPE_INTERVAL:
        spawn_pipe()
        pipe_timer = 0.0

    # move pipes left
    for top, bottom in list(pipes):
        top.x -= PIPE_SPEED * time.dt
        bottom.x -= PIPE_SPEED * time.dt
        # if off screen, remove and reward agents that passed them
        if top.x < -30:
            try:
                pipes.remove((top, bottom))
            except ValueError:
                pass
            destroy(top)
            destroy(bottom)

    alive_count = 0
    for agent in population:
        if not agent.alive:
            continue
        alive_count += 1

        # apply gravity
        agent.velocity -= 4.0 * time.dt
        agent.entity.y += agent.velocity * time.dt

        # sense nearest pipe and act
        nearest = nearest_pipe_for(agent)
        agent.think_and_act(nearest)

        # collision detection: check intersects with any pipe
        hit_any = False
        for top, bottom in pipes:
            if agent.entity.intersects(top).hit or agent.entity.intersects(bottom).hit:
                hit_any = True
                break
        if hit_any:
            # penalize / finalize fitness
            agent.fitness -= 1.0
            agent.kill()
            continue

        # out of bounds
        if agent.entity.y < -12 or agent.entity.y > 25:
            agent.fitness -= 1.0
            agent.kill()
            continue

        # reward survival
        agent.fitness += 0.05 * time.dt  # small reward each frame survived

        # check if passed a pipe (increase score)
        for top, bottom in pipes:
            # if pipe's center x just passed bird.x and not already counted
            if (top.x + top.scale_x/2) < agent.entity.x and not getattr(top, 'passed_by_' + str(id(agent)), False):
                # mark as passed for this agent by setting an attribute on the top pipe entity
                setattr(top, 'passed_by_' + str(id(agent)), True)
                agent.score += 1
                agent.fitness += 5.0  # bigger reward for passing pipe

    # if all dead -> new generation
    if alive_count == 0:
        new_generation()

# handle keyboard if you want to watch a human play (optional)
def input(key):
    if key == 'space':
        # let all birds flap manually if you press space (useful for debugging / watching)
        for a in population:
            if a.alive:
                a.flap()
    if key == 'r':
        # reset generation manually
        new_generation()

# show simple on-screen info
def gui_text():
    Text(f'Generation: {generation}', position=Vec2(-0.9, 0.45), scale=1.2)
    Text(f'Population: {POPULATION}', position=Vec2(-0.9, 0.40))
    Text(f'Best score ever: {best_score_ever}', position=Vec2(-0.9, 0.35))

# add the GUI each frame
def late_update():
    # remove prior GUI (simple approach: destroy all Text children then redraw).
    # In a production version we'd update existing Text objects instead.
    for e in scene.entities:
        if isinstance(e, Text):
            destroy(e)
    gui_text()

# start app
app.run()


from ursina import *
import random as r

app = Ursina()
Sky()

# Bird setup
bird = Animation(
    r'assets_flappyBird/img',  # Use raw string to avoid backslash issues
    collider='box',
    scale=(2, 2, 2),
    y=10
)

camera.orthographic = True
camera.fov = 20

# Pipe list
pipes = []

# Pipe template
pipe = Entity(
    model='quad',
    color=color.green,
    texture='white_cube',
    position=(20, 10),
    scale=(3, 15, 1),
    collider='box',
    enabled=False  # Don't show the template
)

def newPipe():
    y = r.randint(4, 12)
    gap = 10  # Distance btween top and bottom pipes

    # Top pipe
    top_pipe = duplicate(pipe, y=y, x=20, enabled=True)
    # Bottom pipe
    bottom_pipe = duplicate(pipe, y=y - gap - pipe.scale_y, x=20, enabled=True)

    pipes.extend([top_pipe, bottom_pipe])
    invoke(newPipe, delay=2.5)

def update():
    # Gravity
    bird.y -= 4 * time.dt

    # Move pipes and check for collision
    for p in pipes:
        p.x -= 5 * time.dt
        if bird.intersects(p).hit:
            print("Hit!")
            application.quit()

    # Check if bird fell
    if bird.y < -10:
        print("Out of bounds!")
        application.quit()

def input(key):
    if key == 'space':
        bird.y += 3

newPipe()

app.run()


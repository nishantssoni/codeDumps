#nishantssoni june,3 2021
import pygame
import random
from pygame.constants import USEREVENT
from pygame.math import Vector2
import sys


class SNAKE:
    def __init__(self):
        self.body = [Vector2(5,10),Vector2(4,10),Vector2(3,10)]
        self.direction = Vector2(0, 0)
        self.new_block = False
        

        self.head_up = pygame.image.load('Graphics/head_up.png').convert_alpha()
        self.head_down = pygame.image.load('Graphics/head_down.png').convert_alpha()
        self.head_right = pygame.image.load('Graphics/head_right.png').convert_alpha()
        self.head_left = pygame.image.load('Graphics/head_left.png').convert_alpha()
        # self.head = s elf.head_up

        self.tail_up = pygame.image.load('Graphics/tail_up.png').convert_alpha()
        self.tail_down = pygame.image.load('Graphics/tail_down.png').convert_alpha()
        self.tail_right = pygame.image.load('Graphics/tail_right.png').convert_alpha()
        self.tail_left = pygame.image.load('Graphics/tail_left.png').convert_alpha()

        self.body_vertical = pygame.image.load('Graphics/body_vertical.png').convert_alpha()
        self.body_horizontal = pygame.image.load('Graphics/body_horizontal.png').convert_alpha()

        self.body_tr = pygame.image.load('Graphics/body_tr.png').convert_alpha()
        self.body_tl = pygame.image.load('Graphics/body_tl.png').convert_alpha()
        self.body_br = pygame.image.load('Graphics/body_br.png').convert_alpha()
        self.body_bl = pygame.image.load('Graphics/body_bl.png').convert_alpha()
        self.crunch_sound = pygame.mixer.Sound('Sound/crunch.wav')

    def draw_snake(self):
        self.update_head_graphic()
        self.update_tail_graphic()
        for index, block in enumerate(self.body):
            x_pos = int(block.x*cell_size)
            y_pos = int(block.y*cell_size)
        
            snake_rect = pygame.Rect(x_pos, y_pos, cell_size, cell_size)
            
            if index == 0:
                screen.blit(self.head, snake_rect)
            elif index == len(self.body) - 1:
                screen.blit(self.tail, snake_rect)
            else:
                #from this we can find the relation between two 
                # adjecent block of our current block 
                previous_block = self.body[index + 1] - block
                next_block = self.body[index - 1] - block

                if previous_block.x == next_block.x:
                    screen.blit(self.body_vertical, snake_rect)
                
                elif previous_block.y == next_block.y:
                    screen.blit(self.body_horizontal, snake_rect)
                
                else:
                    #in image name e.g bl = bottom(next block) left (previous block)
                    if previous_block.x == -1 and next_block.y == 1 or previous_block.y == 1 and next_block.x == -1:
                        screen.blit(self.body_bl, snake_rect)
                
                    elif previous_block.x == 1 and next_block.y == -1 or previous_block.y == -1 and next_block.x == 1:
                        screen.blit(self.body_tr, snake_rect)
                
                    elif previous_block.x == 1 and next_block.y == 1 or previous_block.y == 1 and next_block.x == 1:
                        screen.blit(self.body_br, snake_rect)
                
                    elif previous_block.x == -1 and next_block.y == -1 or previous_block.y == -1 and next_block.x == -1:
                        screen.blit(self.body_tl, snake_rect)
                

    def update_head_graphic(self):
        """
         
        | 4 |      |  5 |               | -1 |
                -               =  
        | 10 |     |  10 |              |  0 |
        (block)     (head)              (vector)

        from above we get that block is left to the head
        so, we go to right
        """
        #subtractiong two consicutive vector we get position vector
        head_direction = self.body[1] - self.body[0]

        if head_direction == Vector2(-1,0): self.head = self.head_right 
                
        elif head_direction == Vector2(1,0): self.head = self.head_left 
                
        elif head_direction == Vector2(0,-1): self.head = self.head_down 
                
        elif head_direction == Vector2(0,1): self.head = self.head_up

    def update_tail_graphic(self):
        tail_direction = self.body[-2] - self.body[-1]

        if tail_direction == Vector2(-1,0): self.tail = self.tail_right 
                
        elif tail_direction == Vector2(1,0): self.tail = self.tail_left 
                
        elif tail_direction == Vector2(0,-1): self.tail = self.tail_down 
                
        elif tail_direction == Vector2(0,1): self.tail = self.tail_up


    def move_snake(self):
        """snake movement logic:
            1. we copy entire list except last element and copy to new list
            2. we take first element(head) of our main list and add direction to it
            3. we now insert our head in 0th index of the new list
            4. we now copy our entire new list to the main list """
        if self.new_block:
            body_copy = self.body[:]
            self.new_block = False
        else:
            body_copy = self.body[:-1]
        
        body_copy.insert(0,body_copy[0] + self.direction)
        self.body = body_copy[:]

    def add_block(self):
        self.new_block = True

    def play_crunch_sound(self):
        self.crunch_sound.play()

    def reset(self):
        self.body = [Vector2(5,10),Vector2(4,10),Vector2(3,10)]
        self.direction = Vector2(0, 0)

class FRUIT:
    def  __init__(self):
        self.randomize()
        #fruit apple
        self.apple = pygame.image.load('Graphics/apple.png').convert_alpha()
    
    def randomize(self):
        """generate fruit in random place"""

        self.x = random.randint(0,cell_number-1)
        self.y = random.randint(0,cell_number-1)

        #making a vector 2d variable for fruit position with x and y value 
        self.pos = Vector2(self.x, self.y)

    
    def draw_fruit(self):
        """create a rectangel and draw a rectangle"""
        
        #pygame.RECT(x,y,width,height)
        fruit_rect = pygame.Rect(int(self.pos.x*cell_size), int(self.pos.y*cell_size), cell_size , cell_size)

        #screen.blit(surface,rectangle) : image import as new surface
        screen.blit(self.apple, fruit_rect)

        #pygame.draw.rect(surface, color, rectangle)
        # pygame.draw.rect(screen, (126, 166, 114), fruit_rect)


class MAIN:
    def __init__(self):
        self.snake = SNAKE()
        self.fruit = FRUIT()
    
    def update(self):
        self.snake.move_snake()
        self.fruit_eat()
        self.check_fail()

        #update screen and timer for resolution and speed
        screen = pygame.display.set_mode((cell_number*cell_size,cell_number*cell_size))
        pygame.time.set_timer(SCREEN_UPDATE, speed_milisecond)
    
    def draw_elements(self):
        self.draw_grass()
        self.fruit.draw_fruit()
        self.snake.draw_snake()
        self.draw_score()
    
    def fruit_eat(self):
        if self.snake.body[0] == self.fruit.pos:
            self.snake.play_crunch_sound()
            self.fruit.randomize()
            self.snake.add_block()
        #cheking if fruit is not in the body of snake
        for block in self.snake.body[1:]:
            if self.fruit.pos == block:
                self.fruit.randomize()

    def check_fail(self):
        if not 0 <= self.snake.body[0].x < cell_number or not 0 <= self.snake.body[0].y < cell_number:
            self.game_over()

        #check snake head is collapsed with body
        for block in self.snake.body[1:]:
            if block == self.snake.body[0]:
                self.game_over()

    def draw_grass(self):
        grass_color = (167, 209, 61)

        for row in range(cell_number):
            if row % 2 == 0:
                for col in range(cell_number):
                    if col % 2 == 0:
                        grass_rect = pygame.Rect(col * cell_size, row*cell_size, cell_size, cell_size)
                        pygame.draw.rect(screen,grass_color ,grass_rect)
            
            else:
                for col in range(cell_number):
                    if col % 2 != 0:
                        grass_rect = pygame.Rect(col * cell_size, row*cell_size, cell_size, cell_size)
                        pygame.draw.rect(screen,grass_color ,grass_rect)
   
    def game_over(self):
        self.snake.reset()

    def draw_score(self):
        score_text = str(len(self.snake.body) - 3)
        #.render(text,anti-alieases,color) anti-aliease : bool (for smoothning)
        score_surface = game_font.render(score_text, True, (56,74,12))

        #bottom right
        score_x = int(cell_size * cell_number - (cell_size*3))
        score_y = int(cell_size * cell_number - (cell_size*2))

        #rectangle around it and place in bottom right with center
        score_rect = score_surface.get_rect(center = (score_x, score_y))
        
        #apple left to the score for viewing
        apple_rect = self.fruit.apple.get_rect(midright = (score_rect.left, score_rect.centery))
        
        #ractangel aroung (score and fade apple) to cover it with 
        # linewidth width = apple image width + score width
        bg_rect = pygame.Rect(apple_rect.left,apple_rect.top, (apple_rect.width + score_rect.width) + 10, apple_rect.height )
        
        #fading apple image
        fade_apple = pygame.image.load('Graphics/apple.png').convert_alpha()
        fade_apple.set_alpha(80)
        
        #placing in display
        #rect(screen,color,rect,linewidth)
        pygame.draw.rect(screen, (5,5,5), bg_rect, 2)
        screen.blit(score_surface, score_rect)
        screen.blit(fade_apple, apple_rect)


pygame.mixer.pre_init(44100,-16,2,512)
pygame.init()

#global variables
cell_size = 40
cell_number = 20


#objects
screen = pygame.display.set_mode((cell_number*cell_size,cell_number*cell_size))
clock = pygame.time.Clock()
main_game = MAIN()
speed_milisecond = 150

#font object .Font(font,font_size)
game_font = pygame.font.Font('Font/PoetsenOne-Regular.ttf', 25)

#event of our scree update which update every 150 milisecond
SCREEN_UPDATE = pygame.USEREVENT
pygame.time.set_timer(SCREEN_UPDATE, speed_milisecond)

#game loop
while True:

    #for close button
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
        
        #now we can catch our custom event
        if event.type == SCREEN_UPDATE:
            main_game.update()

        #trigieer whenever we hit any key
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_UP:
                if main_game.snake.direction.y != 1:
                    main_game.snake.direction = Vector2(0,-1)
            
            if event.key == pygame.K_DOWN:
                if main_game.snake.direction.y != -1:
                    main_game.snake.direction = Vector2(0,1)

            if event.key == pygame.K_LEFT:
                if main_game.snake.direction.x != 1:
                        main_game.snake.direction = Vector2(-1,0)
            
            if event.key == pygame.K_RIGHT:
                if main_game.snake.direction.x != -1:
                    main_game.snake.direction = Vector2(1,0)
            
            #for screen control
            if event.key == pygame.K_COMMA:
                if cell_number != 15:
                    cell_number = 15
                    #refresing screen and fruit
                    main_game.fruit.randomize()
                    main_game.update()

            elif event.key == pygame.K_m:
                if cell_number != 20:
                    cell_number = 20
                    #refresing screen and fruit
                    main_game.fruit.randomize()
                    main_game.update()

            #for speed control
            if event.key == pygame.K_1:
                if not speed_milisecond > 150 :
                    speed_milisecond += 20
            
            elif event.key == pygame.K_2:
                if not speed_milisecond < 100:
                    speed_milisecond -= 20
    
    screen.fill(pygame.Color((175,215,70)))
    main_game.draw_elements()
    #draw all our elements
    pygame.display.update()

    #run 60 frame in one second
    clock.tick(60)  

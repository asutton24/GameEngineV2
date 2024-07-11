import pygame
from sprite import Text
from spritesheet import Tile

global mode 
global solids
global items
global enemies
global camera
global palettes
global tiles

def getActiveIDs():
    ret = []
    for i in tiles:
        ret.append(i.id)
    return ret

def drawCheckerboard(x, y, boxSize, l, h, scr):
    col = (100, 100, 100)
    bkg = False
    oldX = x
    for i in range(h):
        for j in range(l):
            pygame.draw.rect(scr, col, (x, y, boxSize, boxSize))
            if col == (100, 100, 100):
                col = (150, 150, 150)
            else:
                col = (100, 100, 100)
            x += boxSize
        y += boxSize
        x = oldX
        if not bkg:
            col = (150, 150, 150)
        else:
            col = (100, 100, 100)
        bkg = not bkg
    
class SolidObject:
    def __init__(self, x, y, s, p, scr):
        self.x = x
        self.y = y
        self.spr = s
        self.pal = p
        self.screen = scr

    def draw(self, x, y):
        act = getActiveIDs()
        for i in range(4):
            if not (self.spr + i in act):
                return
        low = act.index(self.spr)
        for i in range(2):
            for j in range(2):
                tiles[low].drawTile(x + 16 * j, y + 16 * i, palettes[self.pal], 1, self.screen)
                low += 1


def solidSheetEditor(screen):
    running = True



def main():
    mode = 'solids'
    camera = 'h'
    solids = []
    items = []
    enemies = []
    palettes = []
    tiles = []
    toInt = lambda x : int.from_bytes(x, 'little')
    palIndex = 0
    page = 0
    try:
        with open('spritesheet.bin', 'rb') as file:
            numOfTiles = int.from_bytes(file.read(2), 'little')
            for i in range(numOfTiles):
                tiles.append(Tile(file.read(16), i))
            file.close()
            print(numOfTiles)
    except:
        with open('spritesheet.bin', 'wb') as file:
            pass
            file.close()
    try:
        with open('palettes.bin', 'rb') as file:
            length = toInt(file.read(2))
            for i in range(length):
                palettes.append([])
                for j in range(3):
                    palettes[i].append((toInt(file.read(1)), toInt(file.read(1)), toInt(file.read(1))))
            file.close()
    except:
        palettes = [[(255, 0, 0), (0, 255, 0), (0, 0, 255)]]
    screen = pygame.display.set_mode([1280, 720])
    clock = pygame.time.Clock()
    running = True
    pygame.init()
    obj = SolidObject(0, 0, 0, 1, screen)
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
        screen.fill((0, 0, 0))
        obj.draw(100, 100)
        pygame.display.update()

if __name__ == "__main__":
    main()
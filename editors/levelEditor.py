import pygame
from sprite import Text
from spritesheet import Tile, getMode

global mode 
global solids
global items
global enemies
global camera
global paletteList
global tiles
global solidPal 
global editorPage

mode = 'solids'
camera = 'h'
solids = []
items = []
enemies = []
paletteList = []
tiles = []
solidPal = []
editorPage = 0

class SolidObject:
    def __init__(self, x, y, s, p, scr):
        self.x = x
        self.y = y
        self.spr = s
        self.pal = p
        self.screen = scr

    def draw(self, x, y, scale):
        act = getActiveIDs()
        for i in range(4):
            if not (self.spr + i in act):
                return
        low = act.index(self.spr)
        for i in range(2):
            for j in range(2):
                tiles[low].drawTile(x + 8 * j * scale, y + 8 * i * scale, paletteList[self.pal], scale, self.screen)
                low += 1

    def drawOnEditor(self, ex, ey):
        if (camera == 'h' and self.x // 640 != editorPage) or (camera == 'v' and self.y // 368 != editorPage):
            return
        if camera == 'h':
            rx = (self.x - editorPage * 640) // 16
            ry = 22 - (self.y // 16)
        else:
            rx = self.x // 16
            ry = 22 - ((self.y - editorPage * 368) // 16)
        self.draw(ex + 16 * rx, ey + 16 * ry, 1)


def compile():
    with open('level.bin', 'wb') as file:
        if camera == 'h':
            file.write(b'\x00')
            linearPos = lambda x, y : x * 23 / 16 + y / 16
        else:
            file.write(b'\x01')
            linearPos = lambda x, y : x / 16 + y * 5 / 2
        file.write(bytes([len(solidPal)]))
        for pal in solidPal:
            file.write(bytes(pal))
        file.write(len(solids).to_bytes(4, 'little'))
        if len(solids) == 0:
            file.close()
            return
        runningCount = 0
        currentIndex = 0
        lastPos = [0, 0]
        space = int(linearPos(solids[0][0], solids[0][1]))
        if space > 0:
            while space > 255:
                file.write(bytes([255, 255]))
                space -= 255
            file.write(bytes([255, space]))
        for i in solids:
            if runningCount == 0:
                currentIndex = i[2]
                runningCount = 1
            elif linearPos(i[0], i[1]) - lastPos == 1:
                if currentIndex == i[2]:
                    runningCount += 1
                    if runningCount == 256:
                        file.write(bytes([currentIndex, 255]))
                        runningCount = 1
                else:
                    file.write(bytes([currentIndex, runningCount]))
                    currentIndex = i[2]
                    runningCount = 1
            else:
                file.write(bytes([currentIndex, runningCount]))
                spaces = linearPos(i[0], i[1]) - lastPos - 1
                while spaces > 255:
                    file.write(bytes([255, 255]))
                    spaces -= 255
                if spaces > 0:
                    file.write(bytes([255, int(spaces)]))
                currentIndex = i[2]
                runningCount = 1
            lastPos = linearPos(i[0], i[1])
        file.write(bytes([currentIndex, runningCount]))
        file.close()

        


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


def drawTilesheet(screen, page, x, y, scale, m, pal):
    bgCol = (100, 100, 100)
    for i in range(128):
        for j in range(128):
            pygame.draw.rect(screen, bgCol, [x + j * scale, y + i * scale, scale, scale])
            if bgCol == (100, 100, 100):
                bgCol = (150, 150, 150)
            else:
                bgCol = (100, 100, 100)
        if bgCol == (100, 100, 100):
            bgCol = (150, 150, 150)
        else:
            bgCol = (100, 100, 100)
    ind = 0
    if len(tiles) == 0:
        return
    for t in tiles:
        if t.id >= page * 256:
            if t.id - (page * 256) > 255:
                return
            break
        ind += 1
    if ind == len(tiles):
        return
    pos = tiles[ind].id - (page * 256)
    while 0 <= pos <= 255:
        realPos = m.index(pos)
        tiles[ind].drawTile(x + (scale * (realPos % 16) * 8), y + (scale * (realPos // 16) * 8), pal, scale, screen)
        ind += 1
        if ind == len(tiles):
            return
        pos = tiles[ind].id - (page * 256)


def appendSolidPalette(screen):
    currentPal = 0
    page = 0
    running = True
    m = getMode(2, 2)
    while running:
        for event in pygame.event.get():
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_RETURN:
                    return
                elif event.key == pygame.K_UP:
                    currentPal += 1
                    if currentPal == len(paletteList):
                        currentPal = 0
                elif event.key == pygame.K_DOWN:
                    currentPal -= 1
                    if currentPal == -1:
                        currentPal = len(paletteList) - 1
                elif event.key == pygame.K_RIGHT:
                    page += 1
                    if page == 256:
                        page = 0
                elif event.key == pygame.K_LEFT:
                    page -= 1
                    if page == -1:
                        page = 255
        mouseB = pygame.mouse.get_pressed()
        mouseP = list(pygame.mouse.get_pos())
        if mouseB[0] and 50 <= mouseP[0] <= 690 and 50 <= mouseP[1] <= 690:
            mouseP[0] -= 50
            mouseP[0] //= 80
            mouseP[1] -= 50
            mouseP[1] //= 80
            solidPal.append(((page * 64 + mouseP[1] * 8 + mouseP[0]) * 4, currentPal))
            return
        screen.fill((0, 0, 0))
        drawTilesheet(screen, page, 50, 50, 5, m, paletteList[currentPal])    
        pygame.display.update()

def editSolids(screen):
    running = True
    guidelines = True
    current = 0
    frameCount = 0
    global editorPage
    global solids
    lastTileClicked = (-1, -1)
    clock = pygame.time.Clock()
    mouseDown = False
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                return
            elif event.type == pygame.MOUSEBUTTONDOWN:
                mouseP = pygame.mouse.get_pos()
                mouseB = pygame.mouse.get_pressed()
                if (not mouseDown) and mouseB[2] and 700 <= mouseP[0] < 1212 and 200 <= mouseP[1] < 712:
                    palRemove = ((mouseP[0] - 700) // 32) + 16 * ((mouseP[1] - 200) // 32)
                    if palRemove < len(solidPal):
                        solidPal.pop(palRemove)
                        for i in range(len(solids) - 1, -1, -1):
                            if solids[i][2] == palRemove:
                                solids.pop(i)
                            elif solids[i][2] > palRemove:
                                solids[i][2] -= 1
                mouseDown = True
            elif event.type == pygame.MOUSEBUTTONUP:
                mouseDown = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_g:
                    guidelines = not guidelines
                if event.key == pygame.K_a:
                    appendSolidPalette(screen)
                if event.key == pygame.K_p:
                    for i in solids:
                        print(i)
                    print(camera)
                    print(' ')
                if event.key == pygame.K_c:
                    compile()
        mouseB = pygame.mouse.get_pressed()
        mouseP = pygame.mouse.get_pos()
        if mouseB[0]:
            if 700 <= mouseP[0] < 1212 and 200 <= mouseP[1] < 712:
                current = ((mouseP[0] - 700) // 32) + 16 * ((mouseP[1] - 200) // 32)
            elif (((mouseP[0] - 15) // 16) * 16, (22 - ((mouseP[1] - 338) // 16)) * 16) != lastTileClicked and current < len(solidPal) and 15 <= mouseP[0] < 655 and 338 <= mouseP[1] < 706:
                tileX = ((mouseP[0] - 15) // 16) * 16
                tileY = (22 - ((mouseP[1] - 338) // 16)) * 16
                lastTileClicked = (tileX, tileY)
                if camera == 'h':
                    tileX += editorPage * 640
                else:
                    tileY += editorPage * 368
                for i in range(len(solids) - 1, -1, -1):
                    if solids[i][0] == tileX and solids[i][1] == tileY:
                        solids.pop(i)
                placed = False
                for i in range(len(solids)):
                    if camera == 'h':
                        if solids[i][0] > tileX or (solids[i][0] == tileX and solids[i][1] > tileY):
                            solids.insert(i, [tileX, tileY, current])
                            placed = True
                    else:
                        if solids[i][1] > tileY or (solids[i][1] ==  tileY and solids[i][0] > tileX):
                            solids.insert(i, [tileX, tileY, current])
                            placed = True
                    if placed:
                        break
                if not placed:
                    solids.append([tileX, tileY, current])
        else:
            lastTileClicked = (-1, -1)
        if mouseB[2]:
            if 15 <= mouseP[0] < 655 and 338 <= mouseP[1] < 706:
                rmX = ((mouseP[0] - 15) // 16) * 16
                rmY = (22 - ((mouseP[1] - 338) // 16)) * 16
                if camera == 'h':
                    rmX += editorPage * 640
                else:
                    rmY += editorPage * 368
                for i in range(len(solids)):
                    if solids[i][0] == rmX and solids[i][1] == rmY:
                        solids.pop(i)
        screen.fill((0, 0, 0))
        pygame.draw.rect(screen, (255, 255, 255), [10, 332, 650, 378])
        pygame.draw.rect(screen, (0, 0, 0), [15, 338, 640, 368])
        if guidelines:
            for i in range(39):
                pygame.draw.rect(screen, (255, 255, 255), [30 + 16 * i, 338, 2, 368])
            for i in range(22):
                pygame.draw.rect(screen, (255, 255, 255), [15, 353 + 16 * i, 640, 2])
        tempSolid = SolidObject(700, 200, 0, 0, screen)
        for i in solids:
            tempSolid.spr = solidPal[i[2]][0]
            tempSolid.pal = solidPal[i[2]][1]
            tempSolid.x = i[0]
            tempSolid.y = i[1]
            tempSolid.drawOnEditor(15, 338)
        drawCheckerboard(700, 200, 4, 128, 128, screen)
        tempSolid.x = 700
        tempSolid.y = 200
        for i in solidPal:
            tempSolid.spr = i[0]
            tempSolid.pal = i[1]
            tempSolid.draw(tempSolid.x, tempSolid.y, 2)
            tempSolid.x += 32
            if tempSolid.x == 1180:
                tempSolid.x = 0
                tempSolid.y += 32
        frameCount += 1
        t = Text('{}/{}'.format(frameCount, len(solids)), 10, 10, (255, 255, 255), 5, screen)
        t.update()
        pygame.display.update()
        clock.tick(60)


def main():
    toInt = lambda x : int.from_bytes(x, 'little')
    palIndex = 0
    page = 0
    global paletteList
    global tiles
    paletteList = []
    try:
        with open('spritesheet.bin', 'rb') as file:
            numOfTiles = int.from_bytes(file.read(2), 'little')
            for i in range(numOfTiles):
                tiles.append(Tile(file.read(16), i))
            file.close()
    except:
        with open('spritesheet.bin', 'wb') as file:
            pass
            file.close()
    try:
        with open('palettes.bin', 'rb') as file:
            length = toInt(file.read(2))
            for i in range(length):
                paletteList.append([])
                for j in range(3):
                    paletteList[i].append((toInt(file.read(1)), toInt(file.read(1)), toInt(file.read(1))))
            file.close()
    except:
        paletteList = [[(255, 0, 0), (0, 255, 0), (0, 0, 255)]]
    screen = pygame.display.set_mode([1280, 720])
    clock = pygame.time.Clock()
    running = True
    pygame.init()
    obj = SolidObject(0, 0, 0, 1, screen)
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_s:
                    editSolids(screen)
        screen.fill((0, 0, 0))
        obj.draw(100, 100, 1)
        pygame.display.update()

if __name__ == "__main__":
    main()
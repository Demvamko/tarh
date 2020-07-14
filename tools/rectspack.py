import math

class Rect():
    idc = 0

    def __init__(self, x = 0, y = 0, w = 0, h = 0):
        self.x = x
        self.y = y
        self.w = w
        self.h = h
        self.id = Rect.idc + 1

        Rect.idc += 1

    def uv(self, divisor, normal):
        return [
            int((self.x / divisor) * normal),
            int((self.y / divisor) * normal),
            int(((self.x + self.w) / divisor) * normal),
            int(((self.y + self.h) / divisor) * normal)
        ]

def Pack(boxes):
    area = 0
    maxw = 0

    for box in boxes:
        area += box.w * box.h
        maxw = max(maxw, box.w)

    sorted_boxes = sorted(boxes, key = lambda x: x.h)
    sorted_boxes.reverse()

    start = max(math.ceil(math.sqrt(area / 0.95)), maxw)
    start = 1 if start == 0 else 2 ** (start - 1).bit_length()

    spaces = [Rect(0, 0, start, start)]

    for box in sorted_boxes:
        for space in spaces[::-1]:

            if box.w > space.w or box.h > space.h:
                continue

            box.x = space.x
            box.y = space.y

            if box.w == space.w and box.h == space.h:
                spaces.remove(space)

            elif box.h == space.h:
                space.x += box.w
                space.w -= box.w

            elif box.w == space.w:
                space.y += box.h
                space.h -= box.h

            else:
                new = Rect(space.x + box.w, space.y, space.w - box.w, box.h)
                spaces.insert(spaces.index(space) + 1, new)
                space.y += box.h
                space.h -= box.h

            break

    return start
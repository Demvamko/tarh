import os
import glob

def walk(dirs):
    arr = []

    for d in dirs:
        for root, _, files in os.walk(d):
            for file in files:
                arr.append(os.path.join(root, file))

    return arr

#//pack --type=image --atlas="UI" --image="./res/img/test.png" --id="RES_IMAGE_TEST"

class Command():
    def __init__(self, line):

        out = {}

        for arg in line.split(' '):
            if not arg.startswith('--'):
                continue
        
            kvp = arg.split('=')
            key = kvp[0][2:]
            val = kvp[1]

            if val.startswith('"'):
                val = val.strip()[1:-1]

            out[key] = val

        self.id = out.get('id')
        self.type = out.get('type')
        self.path = glob.glob(out.get('path')) or [ out.get('path') ]

        #ATLAS VARIABLES
        self.atlas = out.get('atlas')

        self.line = line

# def GenerateFont(name):
#     face = freetype.Face(f"./res/font/{name}.ttf")
#     face.set_char_size(48 * 64)

#     TEX_W = 512
#     TEX_H = 512

#     image = numpy.zeros((TEX_W, TEX_H), dtype=numpy.ubyte)

#     x = 0
#     y = 0
#     max_y = 0
#     line_height = 0

#     uvs = []
#     rects = []
#     advances = []

#     for c in range(127):
#         face.load_char(chr(c))

#         bitmap = face.glyph.bitmap

#         w = face.glyph.bitmap.width
#         h = face.glyph.bitmap.rows

#         if x + w > TEX_W:
#             x = 0
#             y += max_y + 5

#         buffer = numpy.array(bitmap.buffer, numpy.ubyte).reshape(h, w)
#         buffer = numpy.flip(buffer, (0, 1))
#         image[y:y + h, x:x + w] |= buffer[::-1, ::-1]

#         xpos = face.glyph.bitmap_left
#         ypos = 0 - (face.glyph.bitmap.rows - face.glyph.bitmap_top)

#         uvs += [ x / TEX_W, y / TEX_H, (x + w) / TEX_W, (y + h) / TEX_H ]
#         rects += [ xpos, ypos, xpos + w, ypos + h ]
#         advances += [ face.glyph.advance.x >> 6 ]

#         x += w + 5

#         if h > max_y:
#             max_y = h

#         if h > line_height:
#             line_height = h

#     image.tofile("./res/img/font.bin")

#     file = open("./res/bin/font.uv", "wb")

#     uvs = list(map(lambda x: int(x * 0xFFFF), uvs))

#     file.write(struct.pack('%sH' % len(uvs), *uvs))
#     file.close()
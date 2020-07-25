import matplotlib
import PIL.Image
import rectspack
import struct
import numpy
import os



def filename(path):
    return os.path.splitext(os.path.basename(path))[0]

def Pack(commands, res):
    atlases = {
        'default': []
    }

    filenames = {
        'default': []
    }

    for cmd in commands:
        if not cmd.atlas in atlases:
            atlases[cmd.atlas] = []

        if not cmd.atlas in filenames:
            filenames[cmd.atlas] = []

        for path in cmd.path:
            image = PIL.Image.open(path)

            if 'A' not in image.getbands():
                image.putalpha(255)

            atlases[cmd.atlas or 'default'].append(image)
            filenames[cmd.atlas or 'default'].append(filename(path))

    for name, atlas in atlases.items():
        if len(atlas) == 0:
            continue

        boxes = []
        
        for image in atlas:
            boxes.append(rectspack.Rect(0, 0, image.size[0] + 2, image.size[1] + 2))

        size = rectspack.Pack(boxes)

        out = numpy.zeros((size, size, 4), numpy.int8)

        for i, image in enumerate(atlas):

            x = image.size[0]
            y = image.size[1]
            data = numpy.array(image.getdata()).reshape(y, x, 4)

            box = boxes[i]

            out[box.y + 1 : box.y + box.h - 1, box.x + 1 : box.x + box.w - 1] = data[0 : y, 0 : x]

            out[box.y + 1 : box.y + box.h - 1, box.x] = data[0 : y, 0]
            out[box.y + 1 : box.y + box.h - 1, box.x + box.w - 1] = data[0 : y, x - 1]

            out[box.y, box.x + 1 : box.x + box.w - 1] = data[0, 0 : x]
            out[box.y + box.h - 1, box.x + 1 : box.x + box.w - 1] = data[y - 1, 0 : x]

        image = PIL.Image.fromarray(out, 'RGBA')
        image.save(res.bin, "PNG")
        image.save('./res/img/out_' + name + '.png')

        res.add('ATLAS_' + name.upper())
        res.string('ATLAS_COUNT_' + name.upper(), len(boxes))
        res.string('ATLAS_UV_' + name.upper(), len(boxes) * 8)

        print(f"SIZE: {size}")
        for i, box in enumerate(boxes):
            print(f'{box.x} , {box.y} : {box.w} , {box.h}')
            res.bin.write(struct.pack('HHHH', *box.uv(size, 0xFFFF)))

            fname = filenames[name][i]
            res.add(f"ATLAS_IMG_{fname}")

        res.flush()
        
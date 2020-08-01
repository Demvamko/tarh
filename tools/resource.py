import glob
import struct

class Res():
    def __init__(self, binary, head):
        self.bin = binary
        self.head = head

        self.pos = 0
        self.id = 0

        self.head.write("extern void* arh_res_bin;")

    def update(self):
        self.pos = self.bin.tell()

    def add(self, bin, name):
        length = len(bin)
        self.bin.write(struct.pack("I", length))

        start = self.bin.tell()
        self.bin.write(bin)

        self.head.write(f"#define {name} (arh_res_bin + {start})\n")

        self.id += 1

    def define(self, name, val):
        self.head.write(f'#define {name} {val} \n')

    def flush(self):
        self.bin.flush()
        self.head.flush()
        
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
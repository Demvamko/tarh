
class Res():
    def __init__(self, binary, head):
        self.bin = binary
        self.head = head

        self.pos = 0
        self.id = 0

    def update(self):
        self.pos = self.bin.tell()

    def add(self, name):
        last = self.pos
        self.pos = self.bin.tell()
        self.head.write(f"#define {name} {self.pos} \n")
        self.head.write(f"#define {name}_RANGE ((int[]){{ {last}, {self.pos} }}) \n")
        self.head.write("\n")
        self.id += 1

    def string(self, name, val):
        self.head.write(f'#define {name} {val} \n')

    def flush(self):
        self.bin.flush()
        self.head.flush()
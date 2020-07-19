from pprint import pprint
import re
import os

def rename(name, rename, source):
    regex = r'\b{0}\b'.format(name)
    return re.sub(regex, rename, source)

def has_word(word, string):
    return re.search(r'\b{0}\b'.format(word), string)

def has_assign(word, string):
    return re.search(r'\b{0}\b(?=\W*=)'.format(word), string)

def splitw(string):
    return re.split(r'\W+', string)

class Variable():
    def __init__(self, line, file):
        words = splitw(line)

        if words[0].isdigit():
            self.type = words[1]
            self.name = words[2]
            self.input = True
            self.bind = words[0]

        else:
            self.type = words[0]
            self.name = words[1]
            self.input = False
            self.bind = None

        self.rename = '_ren_' + self.name
        self.glob = self.type in ['vert', 'frag', 'struct', 'sampler2D']

        self.function = False
        self.assign_vert = False
        self.assign_frag = False
        self.vert = False
        self.frag = False

        if '(' in line:
            self.function = True
            self.input = True
            self.glob = True
            self.args = re.search(r'\(.+\)', line).group()

        if '{' in line:
            self.content = ''

            while '}' not in line:
                line = file.readline()
                self.content += line

    def input_string(self):
        if self.function:
            return f'{self.type} {self.name} {self.args} {{\n{self.content} \n'
        elif self.type == 'struct':
            return f'layout (binding = {self.bind}) uniform {self.name}_struct{{\n{self.content} \n'
        elif self.type == 'sampler2D':
            return f'layout (binding = {self.bind}) uniform {self.type} {self.name}; \n'
        else:
            return f'layout(location = {self.bind}) in {self.type} {self.name}; \n'
            

class Representation():
    def __init__(self, path):
        self.defs = []
        self.vars = []
        self.glob = []
        self.vert = None
        self.frag = None
        self.vert_code = '#version 440 \n'
        self.frag_code = '#version 440 \nout vec4 _ren_color;\n'

        self.Parse(path)

    def Parse(self, path, is_root = True):
        file = open(path, 'r')

        line = True
        while line:
            line = file.readline()

            if line.isspace() or not line:
                continue

            elif '#include' in line:
                incpath = re.search(r'<.+>', line).group()[1:-1]
                root = os.path.split(path)[0]

                print(f"include: {root}/{incpath}")
                self.Parse(f'{root}/{incpath}', False)
                continue

            elif '#define' in line:
                words = splitw(line)
                self.defs.append((words[2], words[3]))
                continue

            elif '#' in line:
                continue

            else:
                var = Variable(line, file)

                if var.type == 'vert':
                    self.vert = var
                elif var.type == 'frag':
                    self.frag = var
                elif var.glob:
                    self.glob.append(var)
                else:
                    self.vars.append(var)

        if not is_root:
            return

        for var in self.vars + self.glob:
            if has_word(var.name, self.vert.content):
                var.vert = True
                if has_assign(var.name, self.vert.content):
                    var.assign_vert = True

            if has_word(var.name, self.frag.content):
                var.frag = True
                if has_assign(var.name, self.frag.content):
                    var.assign_frag = True

    def Compile(self):
        for var in self.vars:
            if var.input:
                self.vert_code += var.input_string()

        for var in self.vars:
            if var.assign_vert:
                self.vert.content = rename(var.name, var.rename, self.vert.content)
            if var.assign_frag:
                self.frag.content = rename(var.name, var.rename, self.frag.content)

            if var.frag:
                self.vert_code += f'out {var.type} {var.rename}; \n'
                self.frag_code += f'in {var.type} {var.rename}; \n'

                self.frag.content = rename(var.name, var.rename, self.frag.content)
                
                if var.input:
                    self.vert.content = f'    {var.rename} = {var.name}; \n' + self.vert.content

            elif var.assign_vert:
                self.vert.content = f'    {var.type} {var.rename} = {var.name};\n' + self.vert.content

        for var in self.glob:
            if var.vert:
                self.vert_code += var.input_string()
            if var.frag:
                self.frag_code += var.input_string()

        self.vert.content = rename('return', 'gl_Position = ', self.vert.content)
        self.frag.content = rename('return', '_ren_color = ', self.frag.content)

        self.vert_code += f'\nvoid main(){{ \n{self.vert.content}\n\n'
        self.frag_code += f'\nvoid main(){{ \n{self.frag.content}\n\n'

        for define in self.defs:
            self.vert_code = rename(define[0], define[1], self.vert_code)
            self.frag_code = rename(define[0], define[1], self.frag_code)

        print("VERTEX:")
        for idx, line in enumerate(self.vert_code.splitlines(), start=1):
            print('{:4d}: {}'.format(idx, line))

        # print(self.vert_code)
        print("FRAGMENT:")
        for idx, line in enumerate(self.frag_code.splitlines(), start=1):
            print('{:4d}: {}'.format(idx, line))

        # print(self.frag_code)

def Pack(commands, res):

    for cmd in commands:
        for path in cmd.path:

            rep = Representation(path)
            rep.Compile()

            res.bin.write(bytearray(rep.vert_code, 'ascii'))
            res.add(cmd.id + '_VERTEX')
            res.bin.write(bytearray(rep.frag_code, 'ascii'))
            res.add(cmd.id + '_FRAGMENT')



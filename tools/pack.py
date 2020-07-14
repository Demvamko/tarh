'''
    Resource {
        short id;
        void* start;
    };

    File {
        Resource resources[];
        void* data;
    }

'''
import os
import utils
import atlas
import resource

packers = {
    'image': atlas.Pack,
}

def main():
    groups = {}

    for file in utils.walk(['./src', './inc']):
        if not file.endswith(('h', 'c')):
            continue

        file = open(file, 'r')

        for line in file.readlines():
            if not line.startswith('//pack'):
                continue

            cmd = utils.Command(line)

            if cmd.type not in groups.keys():
                groups[cmd.type] = []

            groups[cmd.type].append(cmd)

    output = open('./res/bin/pack.bin', 'wb')
    header = open('./inc/ext/pack.h', 'w')

    res = resource.Res(output, header)

    for group, cmds in groups.items():
        if group in packers.keys():
            packers[group](cmds, res)



main()

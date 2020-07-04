#!/usr/bin/python3

#    PACK LAYOUT
#
#       HEADER
#       [ VERSION : 16 ][ MAGIC : 16 ][ COUNT : 16 ][  UNUSED : 16  ]
#        ITEM
#       [                        ID : 64                            ]
#       [                       SIZE : 64                           ]
#       [                     DATA : <SIZE>                         ]
#        ITEM ...
#

import sys
import argparse
import os
import re
import plistlib
import struct

from PyTexturePacker import Packer as texpack

parser = argparse.ArgumentParser(description='''
    Scans source files for texture usage and packs those textures into atlases
    Once packed will produce exported image atlases of size 1024x1024 their UV map and a C header with sprite id's
    ''')

parser.add_argument('dirs', metavar="dirs", nargs='+', help="Input directories")
parser.add_argument('-o', '--output', help="Output directory")
parser.add_argument('-H', '--header', help="Output header directory")

args = parser.parse_args()

# GET ALL SOURCE FILES
sources = []

for input in args.dirs:
    for root, dirs, files in os.walk(input):
        sources += [os.path.join(root, x) for x in files if x.endswith(('h', 'c'))]

 
# FIND ALL ASSET FILES
regex = re.compile(r'(?P<id>[0-9]+).*PACK\s?:\s?(?P<name>\S+)')

pairs = []
assets = []

for source in sources:
    file = open(source, 'r')

    for line in file.readlines():
        match = regex.search(line)

        if match:
            assets.append(match.group('name'))
            pairs.append((os.path.basename(match.group('name')), match.group('id')))

print(assets)

# GENRATE PACKED ATLAS

output = args.output or "./res/img/atlas"
texpack.create().pack(assets, output)

# PARSE THE PLIST OUTPUT
rnums = re.compile(r'[0-9]+')

plist = plistlib.load(open(output + '.plist', 'rb'))
size = rnums.findall(plist['metadata']['size'])
size = [ float(x) for x in size ]

outfile = open(output + '.uv', 'wb')

for file, id in pairs:
    rect = rnums.findall(plist['frames'][file]['frame'])
    rect = [ float(x) for x in rect ]
    outfile.write(struct.pack("iffff",
        int(id),
        rect[0] / size[0],
        rect[1] / size[1],
        (rect[0] + rect[2]) / size[0],
        (rect[1] + rect[3]) / size[1]
    ))

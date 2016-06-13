#!/usr/bin/python
import argparse

parser = argparse.ArgumentParser(description='Fixes header guards.')
parser.add_argument('--dir', default='.',
                    help='The directory to fix header guards in.')
parser.add_argument('--recurse', type=bool, default=False,
                    help='Whether to fix in subdirectories, too.')

args = parser.parse_args()

def guard(dir, filebase):
  return (dir + '_' + filebase + '_').upper().replace('-', '_').replace('/', '_').replace('.', '_')

import os  
for dir, subdirs, filenames in os.walk(args.dir):
  for filename in filenames:
    base, ext = os.path.splitext(filename)
    path = os.path.join(dir, filename)
    if ext != '.h': continue

    g = guard(dir, filename)
    write = False
    with open(path, 'r') as f:
      lines = f.readlines()
      if lines[0].strip() != '#ifndef ' + g:
        if lines[0].strip().startswith('#ifndef'):
          print 'Wrong guard for ' + path
          continue
        lines.insert(0, '')
        lines.insert(0, '#define ' + g)
        lines.insert(0, '#ifndef ' + g)
        lines.append('')
        lines.append('#endif  // ' + g)
        write = True
      if write:
        with open(path, 'w') as f:
          for line in lines:
            f.write(line.rstrip() + '\n')
        print 'Updated ' + path


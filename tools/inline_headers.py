#!/usr/bin/python
import argparse

parser = argparse.ArgumentParser(description='Inlines lib headers.')
parser.add_argument('--infile', help='The input file.')
parser.add_argument('--outfile', help='The output file.')

args = parser.parse_args()

import os  

if args.infile == None:
  print "Specify --infile"
  exit()
if args.outfile == None:
  print "Specify --outfile"
  exit()

def guard(path):
  return (path + '_').upper().replace('-', '_').replace('/', '_').replace('.', '_')

import re
import sets

p1 = re.compile('#include <(.*)>')
p2 = re.compile('#include "(.*)"')

includes = set()
lib_includes = dict()
lines = dict()

def ParseFile(filename, include_dirs):
  found_path = None
  for include_dir in include_dirs:
    path = os.path.join(include_dir, filename);
    if os.path.isfile(path):
      found_path = path
      break
  if found_path == None:
    print "Can't find: " + filename
    exit()

  if found_path in lines: return found_path

  children = set()
  with open(found_path, 'r') as f:
    lines[found_path] = f.readlines()
    i = 0
    while i < len(lines[found_path]):
      line = lines[found_path][i]
      m1 = p1.match(line.strip())
      m2 = p2.match(line.strip())
      if m1: 
        includes.add(m1.groups()[0])
        lines[found_path].pop(i)
      elif m2:
        children.add(m2.groups()[0])
        lines[found_path].pop(i)
      else:
        i = i + 1
  lib_includes[found_path] = set()
  for lib_include in children:
    lib_includes[found_path].add(
      ParseFile(lib_include, ['/home/kalinov/kodiranje/',
                              os.path.dirname(found_path)]))
  return found_path

ParseFile(args.infile, [''])
  
def ProcessForOutput(lines, canonical_path = None):
  i = 0
  while i < len(lines):
    if canonical_path != None and guard(canonical_path) in lines[i]:
      lines.pop(i)
    elif len(lines[i].strip()) == 0:
      lines.pop(i)
    else:
      break
  i = -1
  while i > -len(lines):
    if canonical_path != None and guard(canonical_path) in lines[i]:
      lines.pop(i)
    elif len(lines[i].strip()) == 0:
      lines.pop(i)
    else:
      break

  i = 1
  while i < len(lines):
    if len(lines[i].strip()) == 0 and len(lines[i - 1].strip()) == 0:
      lines.pop(i)
    else:
      i = i + 1
  return lines

with open(args.outfile, 'w') as f:
  for include in sorted(includes):
    f.write('#include <{0}>\n'.format(include))
  f.write('\n// kalinov\'s libraries:\n')

  def OutputLib(f, path):
    if path not in lines: return
    for lib_include in lib_includes[path]:
      OutputLib(f, lib_include)
    canonical_path = path.replace('/home/kalinov/kodiranje/', '')
    f.write('\n//\n// {0}\n//\n'.format(canonical_path))

    for line in ProcessForOutput(lines.pop(path), canonical_path):
      f.write('{0}\n'.format(line.rstrip()))
  
  for lib_include in lib_includes[args.infile]:
    OutputLib(f, lib_include)

  f.write('\n//\n')
  f.write('// The real code follows:\n')
  f.write('//\n')

  for line in ProcessForOutput(lines.pop(args.infile)):
    f.write('{0}\n'.format(line.rstrip()))
  
  

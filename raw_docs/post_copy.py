import sys
import os
import distutils.dir_util

print('Number of arguments:', len(sys.argv), 'arguments.')
print('Argument List:', str(sys.argv))
#if len(sys.argv) == 3:
src = os.path.realpath(sys.argv[2])
dst = os.path.realpath(sys.argv[1])
print('copy from: ' + src)
print('copy to:   ' + dst)
distutils.dir_util.copy_tree(src, dst, update=1, verbose=1)

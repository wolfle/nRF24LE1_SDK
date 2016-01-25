#!/usr/bin/python

import sys
import os
import os.path
import glob

class MakefileOut:
    def __init__(self, filename):
        self.f = file(filename, 'w')

    def emit_dep(self, target, srcs):
        self.f.write('%s: %s\n\n' % (target, ' '.join(srcs)))

    def emit_lib(self, target, srcs):
        self.f.write('%s: %s\n' % (target, ' '.join(srcs)))
        self.f.write('\t$(SDAR) -rc $@ $^\n')
        self.f.write('\n')

    def emit_rel_dep(self, name, target, src):
        self.f.write('%s: %s\n' % (target, src))
        self.f.write('\t$(SDCC) $(DFLAGS) -I%s/include $^ > $@.tmp\n' % name)
        self.f.write('\t$(SED) \'s,\\($*\)\\.o[ :]*,\\1.o $@ : ,g\' < $@.tmp > $@\n')
        self.f.write('-include %s' % target)
        self.f.write('\n')

    def emit_rel(self, name, target, srcs):
        self.f.write('%s: %s\n' % (target, ' '.join(srcs)))
        self.f.write('\t$(SDCC) -c $(CFLAGS) -I%s/include -o $@ $<' % name)
        self.f.write('\n')

def list_target_dirs():
    dirs = glob.glob('_target*')
    return filter(lambda d: os.path.isdir(d), dirs)

def list_sources():
    files = glob.glob('src/*/src/*.c')
    return files

def calc_target_obj_raw(target, lib, src, dirname, suffix):
    src = lib + '_' + src
    tmp = src.replace('/', '_')
    tmp = tmp[:-2] + suffix
    return target + '/' + dirname + '/' + tmp

def calc_target_obj(target, lib, src):
    return calc_target_obj_raw(target, lib, src, 'obj', '.rel')

def calc_target_objs(target, lib, srcs):
    return map(lambda s: (calc_target_obj(target, lib, s), s), srcs)

def calc_target_dep(target, lib, src):
    return calc_target_obj_raw(target, lib, src, 'dep', '.d')

def emit_target(make, target, lib, srcs):
    target_obj_pairs = calc_target_objs(target, lib, srcs)
    target_lib_name = target + '/lib/' + lib + '.lib'

    make.emit_lib(target_lib_name, map(lambda pair: pair[0], target_obj_pairs))
    for pair in target_obj_pairs:
        dep_name = calc_target_dep(target, lib, pair[1])
        make.emit_rel_dep(target, dep_name, pair[1])
        make.emit_rel(target, pair[0], [pair[1], dep_name])
    return target_lib_name

def main():
    target_dirs = list_target_dirs()
    srcs = list_sources()
    uart_poll_srcs = glob.glob('uart_poll/*.c')
    uart_int_srcs = glob.glob('uart_int/*.c')

    make = MakefileOut('Makefile.gen')
    libs = []
    for target in target_dirs:
        libs.append(emit_target(make, target, 'nrf24le1', srcs))
        libs.append(emit_target(make, target, 'uart_poll', uart_poll_srcs))
        libs.append(emit_target(make, target, 'uart_int', uart_int_srcs))

    make.emit_dep('libs', libs)

if __name__ == '__main__':
    main()

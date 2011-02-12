#! /usr/bin/env python
# encoding: utf-8

APPNAME = 'bitvec64'
VERSION = '0.0.1'

from waflib.Tools import waf_unit_test

top = '.'
out = 'build'
cflags = '-O9 -Wall -W -Wformat=2 -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Wfloat-equal -Wpointer-arith'

def options(opt):
    opt.load('compiler_cxx')
    opt.load('waf_unit_test')

def configure(conf):
    conf.load('compiler_cxx')
    conf.recurse('tests')

def build(bld):
    bld.stlib(source='./src/bitvec.cpp', 
              target='bitvec64',
              includes = './src',
              cflags=cflags)
    bld.recurse('tests')
    bld.add_post_fun(waf_unit_test.summary)
    bld.options.all_tests = True

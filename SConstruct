#!/bin/python3

# !	scons SubJs (default) builds project

SetOption('num_jobs', 4)
sources = [ Glob('src/*.cpp') ];

env = Environment();
env.Append(CCFLAGS = ' -Wshadow -Wall -Werror -O2 -g')

env.Append(CCFLAGS = '-Iboost');
env.Append(LINKFLAGS = '-lboost_program_options');
env.Append(LINKFLAGS = '-lboost_system');
env.Append(LINKFLAGS = '-lboost_filesystem');

env.Append(CCFLAGS = '-Isrc');

build = env.Program(target = 'SubJs', source = sources)
Default(build)

# !	scons SubJs (default) builds project
# !	scons test builds tests

SetOption('num_jobs', 8)

# build program
env = Environment();
env.Append(CCFLAGS = ' -Wshadow -Wall -Werror -O2 -g')
env.Append(CCFLAGS = '-Iboost');
env.Append(LINKFLAGS = '-lboost_program_options');
env.Append(LINKFLAGS = '-lboost_system');
env.Append(LINKFLAGS = '-lboost_filesystem');
env.Append(CCFLAGS = '-Isrc');
env.VariantDir('_build', 'src', duplicate=0)
sources = [Glob('_build/*.cpp')]
build = env.Program(target = 'SubJs', source = sources)

# build tests
env_test = env.Clone();
env_test.Append(LINKFLAGS='-lboost_unit_test_framework');
env_test.VariantDir('_build_test', 'tests', duplicate=0)
env_test.Append(CCFLAGS='--define BOOST_TEST_DYN_LINK')
test_sources = []
test_sources.append(Glob('_build_test/*.cpp'))
# TODO: get all .o from src without main.o
test_sources.append(Glob('_build/lexer.o'))
test_sources.append(Glob('_build/atom.o'))
test_sources.append(Glob('_build/parser.o'))
test_sources.append(Glob('_build/tree.o'))
build_test = env_test.Program(target = 'test', source = test_sources);
Depends(build_test, build)

Default(build)

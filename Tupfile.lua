function join(list)
  return table.concat(list, "\x20")
end

function remove(list, entry)
  for k,v in pairs(list) do
    if v == entry then
      table.remove(list, k)
    end
  end
end

MAIN_TARGET = 'bin/island'
TEST_TARGET = 'bin/tests'

MOC = join {
  'moc', '--no-notes',
  '-o %o %f',
}

CXX = join {
  'c++',
  '-Wall -Wextra -pipe -fPIC',
  '-fdiagnostics-color',
  '-g -O2 -std=c++11',
  '`pkg-config --cflags Qt5WebEngineWidgets`',
}

CCFLAGS = '-c %f -o %o'
LDFLAGS  = join {
  '%f',
  '`pkg-config --libs Qt5WebEngineWidgets`',
  '-o %o',
}

-- building the main binary
tup.foreach_rule('src/*.hpp', MOC, 'build/moc_%B.cpp')
tup.foreach_rule(
  {'src/*.cpp', 'build/moc_*.cpp'},
  join{CXX, CCFLAGS},
  'build/%B.o'
)
tup.rule('build/*.o', join{CXX, LDFLAGS}, MAIN_TARGET)

-- build the test suite
tup.foreach_rule('test/*.cpp', join{CXX, '-Isrc', CCFLAGS}, 'build/test/%B.o')

local objects = tup.glob('build/*.o')
objects += tup.glob('build/test/*.o')
remove(objects, 'build/main.o')

tup.rule(objects, join{CXX, LDFLAGS}, TEST_TARGET)

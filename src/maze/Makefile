.PHONY: test gcov_report valgrind clang clean clean_build rebuild dvi install uninstall build_maze

HEADERS = backend/model.h \
		backend/genMaze.h \
		backend/genCave.h \
		backend/solveMaze.h \
		backend/agent.h

SRC = backend/genMaze.cpp \
	  backend/genCave.cpp \
	  backend/solveMaze.cpp \
	  backend/agent.cpp

FLAGS = g++ -Wall -Wextra -Wunused -Warray-bounds -Werror -std=c++20

LIBS = -lgtest -lpthread -lstdc++

all: clean clean_build install test gcov_report

install: build_maze
	mkdir ../app
	cp ../build/maze ../app
	cd ../app && ./maze

uninstall: clean_build
	rm -r ../app

build_maze:
	mkdir ../build
	qmake frontend/maze.pro -o ../build/Makefile
	cd ../build; make && make clean

test: clean
	$(FLAGS) test/* $(HEADERS) $(SRC) $(LIBS) -o test.out
	./test.out

gcov_report: clean
	$(FLAGS) test/* $(HEADERS) $(SRC) --coverage $(LIBS) -o test.out
	./test.out
	lcov -t "./test.out" -exclude="*test*" -exclude="*.h" -exclude="*11*" -o report.info -c -d .
	genhtml -o report/ report.info
	open ./report/index.html

valgrind:
	valgrind ./test.out --leak-check=full -s

dvi:
	texi2dvi "../documentation/Mazes&Caves.tex"
	@echo -e "\nManual was converted to dvi.\n"	

clang:
	clang-format-18 -i backend/*.cpp backend/*.h frontend/*.cpp frontend/*.h test/*.cpp

clean:
	rm -rf *.aux *.dvi *.log *.toc *.tar *.gz *.txt 
	rm -rf *.o *.a *.gcno *.gcda *.info *.out *.gcov *.gch *.dSYM report
	@echo -e "\nAll clean!\n"

style_check:
	clang-format -n -style=file:../../materials/linters/.clang-format $(SRC) $(HEADERS)

style:
	clang-format -i -style=file:../../materials/linters/.clang-format $(HEADERS) $(SRC)

clean_build:
	rm -rf ../build

rebuild: clean clean_build all

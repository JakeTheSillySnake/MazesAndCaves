.PHONY: test gcov_report valgrind clang clean clean_build rebuild dvi

HEADERS = backend/model.h backend/genMaze.h backend/genCave.h backend/solveMaze.h
SRC = backend/model.cpp backend/genMaze.cpp backend/genCave.cpp backend/solveMaze.cpp
FLAGS = g++ -Wall -Wextra -Werror -std=c++17

all: clean test gcov_report

test: clean
	$(FLAGS) test/* $(HEADERS) $(SRC) -lgtest -lpthread -o test.out
	./test.out

gcov_report: clean
	$(FLAGS) test/* $(HEADERS) $(SRC) --coverage -lgtest -lpthread -lstdc++ -o test.out
	./test.out
	lcov -t "./test.out" -exclude="*test*" -exclude="*.h" -exclude="*model*" -exclude="*11*" -o report.info -c -d .
	genhtml -o report/ report.info
	open ./report/index.html

valgrind:
	valgrind ./test.out --leak-check=full -s

dvi:
	texi2dvi "../documentation/Mazes&Caves.tex"
	@echo "\nManual was converted to dvi.\n"	

clang:
	clang-format -i backend/*.cpp backend/*.h frontend/*.cpp frontend/*.h test/*.cpp

clean: clean_build
	rm -rf *.aux *.dvi *.log *.toc *.tar *.gz *.txt 
	rm -rf *.o *.a *.gcno *.gcda *.info *.out *.gcov *.gch *.dSYM report
	@echo "\nAll clean!\n"

clean_build:
	cd ../build; rm -rf *obj moc* Makefile* .qmake.stash *cpp *h object* debug/ release/ *o

rebuild: clean all

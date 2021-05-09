
BookList: BookList.hpp BookList.cpp main.cpp
	clang++ --std=c++17 -Wall -g BookList.cpp main.cpp -o BookList

test: BookList data.csv
	./BookList

clean:
	rm -f BookList

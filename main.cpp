#include <iostream>
#include <sstream>
#include <vector>

typedef std::vector<std::pair<int,int> > column;
typedef std::vector<column> matrix;
matrix parsematrix(char const *str) {
	matrix ret;
	column col;
	int acc;
	for (int i = 0; str[i]; i++) {
		if (isdigit(str[i])) {
			acc *= 10;
			acc += str[i] - '0';
		} else if (str[i] == ',' || str[i] == ')') {
			std::pair<int,int> entry;
			entry.first = acc;
			if (ret.size()) {
				int parent;
				if (col.size()) {
					//parent = ret.back().back().second;
					parent = col.back().second;
				} else {
					parent = ret.size() - 1;
				}
				while (parent >= 0 && ret[parent][col.size()].first >= acc) {
					if (col.size()) {
						parent = ret[parent][col.size() - 1].second;
					} else {
						parent--;
					}
				}
				entry.second = parent;
			} else {
				entry.second = -1;
			}
			col.push_back(entry);
			acc = 0;
			if (str[i] == ')') {
				ret.push_back(col);
				col = *new std::vector<std::pair<int,int> >;
			}
		}
	}
	return ret;
}
void printmatrix(matrix mat) {
	for (int i = 0; i < mat.size(); i++) {
		for (int j = 0; j < mat[0].size(); j++) {
			std::cout << mat[i][j].first << mat[i][j].second << ' ';
		}
		std::cout << std::endl;
	}
}
void stepmatrix(matrix &mat, int maxlen) {
	column cut = mat.back();
	mat.pop_back();
	if (cut.size() == 0 || cut[0].first == 0) {
		return;
	}
	int critrow;
	for (critrow = cut.size() - 1; cut[critrow].first == 0; critrow--) {}
	int badroot = cut[critrow].second;
	int badpartlen = mat.size() - badroot;
	int limit = mat.size();
	while (limit + badpartlen < maxlen) {limit += badpartlen;}
	for (int i = badroot; mat.size() < limit; i++) {
		column newcol;
		for (int j = 0; j < cut.size(); j++) {
			std::pair<int, int> entry = mat[i][j];
			if (j < critrow) {
				int ancestor = i;
				while (ancestor > badroot) {
					ancestor = mat[ancestor][j].second;
				}
				if (ancestor == badroot) {
					entry.first++;
					if (entry.second > 0) {
						entry.second += badpartlen;
					} else {
						entry.second = mat.size() - badpartlen;
					}
				}
			}
			newcol.push_back(entry);
		}
		mat.push_back(newcol);
	}
}
int main(int argc, char const *argv[]) {
	if (argc < 3) {
		std::cout << "Insufficient arguments" << std::endl;
		return 0;
	}
	matrix mat = parsematrix(argv[1]);
	matrix target = parsematrix(argv[2]);
	int limit = 10;
	if (argc > 3) {
		std::istringstream ss(argv[3]);
		ss >> limit;
	}
	int steps;
	for (int i = 0; i < target.size(); i++) {
		for (int j = 0; j < target[i].size(); j++) {
			while (mat[i][j] != target[i][j]) {
				stepmatrix(mat, limit);
				steps++;
			}
		}
	}
	while (mat.size() > target.size()) {
		stepmatrix(mat, limit);
		steps++;
	}
	std::cout << "Steps: " << steps << std::endl;
	return 0;
}

#include <iostream>
#include <sstream>
#include <vector>

typedef std::vector<std::pair<int,int> > column;
typedef std::vector<column> matrix;
matrix parsematrix(char const *str) {
	matrix ret;
	column col;
	int acc = 0;
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
	for (unsigned int i = 0; i < mat.size(); i++) {
		for (unsigned int j = 0; j < mat[0].size(); j++) {
			if (mat[i][j].second >= 0) {
				std::cout << mat[i][j].first << mat[i][j].second << ' ';
			} else {
				std::cout << mat[i][j].first << ' ' << ' ';
			}
		}
		std::cout << std::endl;
	}
}
void printmatrixshort(matrix mat) {
	for (unsigned int i = 0; i < mat.size(); i++) {
		std::cout << '(';
		for (unsigned int j = 0; j < mat[0].size(); j++) {
			if (j > 0) {
				std::cout << ',';
			}
			std::cout << mat[i][j].first;
		}
		std::cout << ')';
	}
	std::cout << std::endl;
}
void checkubi(matrix &mat) {
	for (unsigned int i = 0; i < mat.size(); i++) {
		for (unsigned int j = 0; j < mat[i].size(); j++) {
			int parent;
			if (i > 0) {
				if (j > 0) {
					parent = mat[i][j - 1].second;
				} else {
					parent = i - 1;
				}
				while (parent >= 0 && mat[parent][j].first >= mat[i][j].first) {
					if (j > 0) {
						parent = mat[parent][j - 1].second;
					} else {
						parent--;
					}
				}
			} else {
				parent = -1;
			}
			if (parent != mat[i][j].second) {
				std::cout << "UBI violation!" << ' ' << parent << ' ' << mat[i][j].second << std::endl;
				printmatrixshort(mat);
				mat[i][j].second = parent;
			}
		}
	}
}
int stepmatrix(matrix &mat, unsigned int maxlen) {
	column cut = mat.back();
	mat.pop_back();
	if (cut.size() == 0 || cut[0].first == 0) {
		return 1;
	}
	unsigned int critrow;
	for (critrow = cut.size() - 1; cut[critrow].first == 0; critrow--) {}
	int badroot = cut[critrow].second;
	int badpartlen = mat.size() - badroot;
	unsigned int limit = mat.size();
	#ifdef BM2D3
	limit = maxlen;
	#else
	while (limit + badpartlen <= maxlen) {limit += badpartlen;}
	#endif
	for (unsigned int i = badroot; mat.size() < limit; i++) {
		column newcol;
		newcol.reserve(cut.size());
		for (unsigned int j = 0; j < cut.size(); j++) {
			std::pair<int, int> entry = mat[i][j];
			if (j < critrow) {
				int ancestor = i;
				while (ancestor > badroot) {
					ancestor = mat[ancestor][j].second;
				}
				if (ancestor == badroot) {
					entry.first += cut[j].first - mat[badroot][j].first;
				}
			}
			int parent;
			if (j > 0) {
				parent = newcol.back().second;
			} else {
				parent = mat.size() - 1;
			}
			while (parent >= 0 && mat[parent][j].first >= entry.first) {
				if (j > 0) {
					parent = mat[parent][j - 1].second;
				} else {
					parent--;
				}
			}
			entry.second = parent;
			newcol.push_back(entry);
		}
		mat.push_back(newcol);
	}
	#ifdef SHOW_STEPS
	printmatrixshort(mat);
	#endif
	#ifdef CHECK_UBI
	checkubi(mat);
	#endif
	return 0;
}
int main(int argc, char const *argv[]) {
	if (argc < 3) {
		std::cout << "Insufficient arguments" << std::endl;
		return 0;
	}
	matrix mat = parsematrix(argv[1]);
	#ifdef SINGLE_STEP
	printmatrix(mat);
	stepmatrix(mat, 10);
	std::cout << std::endl;
	printmatrix(mat);
	#else
	matrix target = parsematrix(argv[2]);
	int limit = 10;
	if (argc > 3) {
		std::istringstream ss(argv[3]);
		ss >> limit;
	}
	int steps = 0;
	int counter = 0;
	for (unsigned int i = 0; i < target.size(); i++) {
		for (unsigned int j = 0; j < target[i].size(); j++) {
			while (mat[i][j] != target[i][j]) {
				counter += stepmatrix(mat, limit);
				steps++;
			}
		}
	}
	while (mat.size() > target.size()) {
		counter += stepmatrix(mat, limit);
		steps++;
	}
	std::cout << "Steps: " << steps << std::endl;
	std::cout << "Counter: " << counter << std::endl;
	#endif
	return 0;
}

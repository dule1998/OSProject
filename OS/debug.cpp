#include "debug.h"
#include <iostream.h>

const int cnt_max = 10;
int cnts[cnt_max] = {0};
int cnts_printed = 4;

void debugPrint() {
	cout << endl;
	for (int i = 0; i < cnts_printed - 1; ++i) {
		cout << "c[" << i << "] = " << cnts[i] << " | ";
	}
	cout << "c[" << (cnts_printed - 1) << "] = " << cnts[cnts_printed - 1];
	cout << endl;
}


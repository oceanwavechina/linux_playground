/*
 * rand_test.cpp
 *
 *  Created on: Mar 13, 2017
 *      Author: liuyanan
 */


#include <cstdlib>


//  测试产生随机数的性能
int gen_rand_num() {

	for(int i=0;i<10;i++){
		int rand_num = rand() % 6;
		return rand_num;
	}
}

int main() {

	srand(1);

	for (int i=0; i<10000; ++i) {
		gen_rand_num();
	}

	return 0;
}

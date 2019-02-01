/*
 * mmap_copy_files.cpp
 *
 *  Created on: Nov 19, 2016
 *      Author: liuyanan
 */




#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>

using namespace std;

int main(int argc, char* argv[])
{
	int fd_src = -1;
	void *sm , *dm= NULL;

	if (argc !=2 ) {
		cerr << "Usage: " << argv[0];
		exit(EXIT_FAILURE);
	}


	if ((fd_src = open(argv[1], O_RDONLY)) < 0) {
		cerr << "open file error !";
		exit(EXIT_FAILURE);
	}

	struct stat statbuf;
	stat(argv[1], &statbuf);

	sm = mmap(NULL, (size_t)statbuf.st_size, PROT_READ, MAP_PRIVATE, fd_src, 0);


	write(STDOUT_FILENO, sm, statbuf.st_size);

	return EXIT_SUCCESS;
}

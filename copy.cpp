#include <iostream>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;

int main() {
	int fd_in, fd_out;
	char inFile[] = "success.txt";
	char outFile[] = "sucess_copy.txt";
	off64_t len = 7, ret;
	char buf[8] = "1234567";
	pid_t pid;
	char *argv[] = {"/usr/lib/firefox-esr/firefox-esr", "www.fairmontstate.edu",/* "-display", ":0",*/ NULL};
	
	// open input file
	fd_in = open(inFile, O_RDONLY);
	if(fd_in == -1) {
		cout << "inFile Error" << endl;
		exit(EXIT_FAILURE);
	}
	
	// open output file
	fd_out = open(outFile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if(fd_out == -1) {
		cout << "outFile Error" << endl;
		exit(EXIT_FAILURE);
	}
	
	// copy loop
	do {
		ret = copy_file_range(fd_in, NULL, fd_out, NULL, len, 0);
		if(ret == -1) {
			cout << "Copy Error" << endl;
			exit(EXIT_FAILURE);
		}
		
		len -= ret;
	}
	while(len > 0 && ret > 0);
	
	// return to beginning of file
	close(fd_in);
	fd_in = open(inFile, O_RDONLY);
	if(fd_in == -1) {
		cout << "inFile Error" << endl;
		exit(EXIT_FAILURE);
	}
	
	// read file
	ret = read(fd_in, buf, 7);
	if(ret == -1) {
		cout << "Read Error" << endl;
		exit(EXIT_FAILURE);
	}
	
	// print the contents to screen
	for(int n = 0; n < 7; n++)
		cout << buf[n];	
		
	cout << endl;
	
	close(fd_in);
	close(fd_out);
	
	// remove duplicate file
	ret = remove(outFile);
	if(ret == -1) {
		cout << "Remove Error" << endl;
		exit(EXIT_FAILURE);
	}

	// fork
	pid = fork();
	if(pid == -1) {
		cout << "Fork Error" << endl;
		exit(EXIT_FAILURE);
	}
	else if(pid == 0) {
		int websiteOpened = execv(argv[0], &argv[0]);
		if(websiteOpened == -1) {
			cout << "Website Error" << endl;
			exit(EXIT_FAILURE);
		}
	}
	else {
		cout << "Firefox started!" << endl;
		exit(0);
	}
	
	exit(EXIT_SUCCESS);
}


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <errno.h>


#define FILE_SIZE 1024*1024*16

int create_file_set_size(char * file_name, unsigned int file_size) {
	int fd = open(file_name,
				  O_CREAT | O_RDWR,
				  S_IRUSR | S_IWUSR // l'utente proprietario del file avrà i permessi di lettura e scrittura sul nuovo file
				 );
	if (fd == -1) {
    perror("open()");
    return -1;
  }

	int res = ftruncate(fd, file_size);
	if (res == -1) {
		perror("ftruncate()");
		return -1;
	}

	return fd;
}


int main(int argc, char * argv[]) {
  char * file_name = "output.txt";
	unsigned int file_size = FILE_SIZE;

	int res;
	int fd;

	char * addr;

  /*
	if (argc > 1) {
		file_name = argv[1];

		if (argc > 2) {
			unsigned int temp;
			res = sscanf(argv[2], "%u", &temp);
			if (res == 1)
				file_size = temp;
		}
	}
  */

	fd = create_file_set_size(file_name, file_size);
	if (fd == -1) {
		exit(EXIT_FAILURE);
	}

  addr = mmap(NULL, // NULL: è il kernel a scegliere l'indirizzo
			file_size, // dimensione della memory map
			PROT_READ | PROT_WRITE, // memory map leggibile e scrivibile
			MAP_SHARED, // memory map condivisibile con altri processi
			fd,
			0); // offset nel file
	if (addr == MAP_FAILED) {
		perror("mmap()");
		exit(EXIT_FAILURE);
	}

	close(fd);

  memset(addr, 97, file_size);  //impostiamo tutti i byte del file al valore 'a'

  return 0;
}
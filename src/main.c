#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>

#include "common.h"
#include "file.h"
#include "parse.h"

void print_usage(char *argv[]) {
	printf("Usage: %s -n -f <database file>", argv[0]);
	printf("\t -n  -  create new database file\n");
	printf("\t -f  -  (required) path to database file\n");
	return;
}

int main(int argc, char *argv[]) {
	char *filepath = NULL;
	bool newfile = false;
	int c;

	int dbfd = -1;
	struct dbhdr_t *dbhdr = NULL;

	while ((c = getopt(argc, argv, "nf:")) != -1) {
		switch (c) {
			case 'n':
				newfile = true;
				break;
			case 'f':
				filepath = optarg;
				break;
			case '?':
				printf("Unknown option -%c\n", c);
				break;
			default:
				return -1;
		}
	}

	if (filepath == NULL) {
		printf("Filepath is a required argument\n");
		print_usage(argv);
		return 0;
	}

	if (newfile) {
		dbfd = create_db_file(filepath);
		if (dbfd == STATUS_ERROR) {
			printf("Unable to create the database file\n");
			return -1;
		}

		if (create_db_header(dbfd, &dbhdr) == STATUS_ERROR) {
			printf("Failed to created database header\n");
			return -1;
		}

	} else {
		dbfd = open_db_file(filepath);
		if (dbfd == STATUS_ERROR) {
			printf("Unable to open the database file\n");
			return -1;
		}

		if (validate_db_header(dbfd, &dbhdr) == STATUS_ERROR) {
			printf("Failed to validate database header\n");
			return -1;
		}
	}

	// printf("Nefile: %d\n", newfile);
	// printf("Filepath: %s\n", filepath);

	output_file(dbfd, dbhdr, NULL);
	return 0;
}

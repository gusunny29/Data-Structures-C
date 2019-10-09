#include <stdio.h>
#include <string.h>

#include "svec.h"

void
sort(svec* xs)
{
    // TODO: sort input vector by string length
    // see "man 3 strlen"
    int position = 0;
    while (position < xs->size) { 
	if (position == 0 || strlen(xs->data[position]) >= strlen(xs->data[position-1])) {
		position = position + 1;
	} else {
		svec_swap(xs, position, position-1);
		position = position-1;
	}
    }
}

void
chomp(char* text)
{
    // TODO: Modify input string to remove trailing newline ('\n')
    text[strlen(text)-1] = 0;
}

int
main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage:\n  %s input-file\n", argv[0]);
        return 1;
    }

    FILE* fh = fopen(argv[1], "r");
    if (!fh) {
        perror("open failed");
        return 1;
    }

    svec* xs = make_svec();
    char temp[128];

    while (1) {
        char* line = fgets(temp, 128, fh);
        if (!line) {
            break;
        }
        chomp(line);
        svec_push_back(xs, line);
    }

    fclose(fh);

    sort(xs);

    for (int ii = 0; ii < xs->size; ++ii) {
        char* line = svec_get(xs, ii);
      	 printf("%s\n", line);
    }

    free_svec(xs);
    return 0;
}

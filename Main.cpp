#include <bits/stdc++.h>
#include <getopt.h>
#include <glob.h>

using namespace std;

void help() {
    printf("need help?\n");
}

int main(int argc, char **argv) {

    static struct option long_options[] = {
        {"help"   , no_argument      , 0, 'h'},
        {"count"  , no_argument      , 0, 'c'},
        {"edit"   , required_argument, 0, 'e'},
        {"pattern", required_argument, 0, 'p'},
        {0, 0, 0, 0}
    };

    int op;
    int hasHelp = 0, hasEdit = 0, hasCount = 0, hasPatternFile = 0;
    string patternFileName;
    int maxError;

    while ((op = getopt_long(argc, argv, "hce:p:", long_options, NULL)) != -1) {
        if (op == 'h') {
            help();
        } else if (op == 'e') {
            maxError = atoi(optarg);
            hasEdit = 1;
        } else if (op == 'c') {
            hasCount = 1;
        } else if (op == 'p') {
            hasPatternFile = 1;
            patternFileName = string(optarg);
        }
    }

    if (hasCount) {
        printf("only the number of occurrences\n");
    }
    if (hasEdit) {
        printf("max error: %d\n", maxError);
    }
    if (hasPatternFile) {
        printf("there is a pattern file: %s\n", patternFileName.c_str());
    } else {
        printf("the pattern to be sought for is: %s\n", argv[optind]);
        ++optind;
    }
    while (optind < argc) {
        glob_t *vector_ptr = new glob_t;
        glob(argv[optind], GLOB_NOSORT|GLOB_MARK, NULL, vector_ptr);
        for (int i = 0; i < vector_ptr->gl_pathc; i++) {
            printf("there is a textfile with name: %s\n", vector_ptr->gl_pathv[i]);
        }
        ++optind;
    }

    return 0;
}
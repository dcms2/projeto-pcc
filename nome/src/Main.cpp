#include <bits/stdc++.h>
#include <getopt.h>
#include <glob.h>

using namespace std;

inline void help() {
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
    char* patternFileName;
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
            patternFileName = optarg;
        }
    }

    if (hasEdit) {
        printf("max error: %d\n", maxError);
    } else {
        vector<string> patterns;
        if (hasPatternFile) {
            ifstream fileReader(patternFileName);
            string pattern;
            while (getline(fileReader, pattern)) {
                patterns.push_back(pattern);
            }
        } else {
            patterns.push_back(argv[optind]);
            ++optind;
        }

        if (hasCount) {
            printf("only the number of occurrences\n");
        }
        while (optind < argc) {
            glob_t *vector_ptr = new glob_t;
            glob(argv[optind], GLOB_NOSORT|GLOB_MARK, NULL, vector_ptr);
            for (int i = 0; i < vector_ptr->gl_pathc; i++) {
                printf("there is a textfile with name: %s\n", vector_ptr->gl_pathv[i]);
            }
            ++optind;
        }
    }

    return 0;
}

#include <bits/stdc++.h>
#include <getopt.h>
#include <glob.h>
#include "Aho_Corasick.h"
#include "Ukkonen.h"

using namespace std;

inline void help() {
    ifstream fileReader("../doc/help.txt");
    string line;
    while (getline(fileReader, line)) {
        printf("%s\n", line.c_str());
    }
}

inline void exactMatch(const vector<string>& fileNames, const vector<string>& patterns, const bool hasCount) {
    AhoCorasick aho = AhoCorasick(patterns);
    string line;
    for (int i = 0; i < fileNames.size(); ++i) {
        ifstream fileReader(fileNames[i]);
        long long total = 0;
        while (getline(fileReader, line)) {
            if (hasCount) {
                total += aho.numTimes(line, false);
            } else {
                if (aho.numTimes(line, true)) {
                    printf("%s:%s\n", fileNames[i].c_str(), line.c_str());
                }
            }
        }
        if (hasCount) {
            printf("%s:%lld\n", fileNames[i].c_str(), total);
        }
    }
}

inline void approximateMatch(const vector<string>& fileNames, const vector<string>& patterns,  const bool hasCount, const int maxError) {
    string line;
    Ukkonen* ukk = new Ukkonen[patterns.size()];
    for (int i = 0; i < patterns.size(); ++i) {
        ukk[i] = Ukkonen(patterns[i], maxError);
    }
    for (int i = 0; i < fileNames.size(); ++i) {
        ifstream fileReader(fileNames[i]);
        long long total = 0;
        while (getline(fileReader, line)) {
            if (hasCount) {
                for (int j = 0; j < patterns.size(); ++j) {
                    total += ukk[j].numTimes(line, false);
                }
            } else {
                for (int j = 0; j < patterns.size(); ++j) {
                    if (ukk[j].numTimes(line, true)) {
                        printf("%s:%s\n", fileNames[i].c_str(), line.c_str());
                        break;
                    }
                }
            }
        }
        if (hasCount) {
            printf("%s:%lld\n", fileNames[i].c_str(), total);
        }
    }
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
            hasHelp = 1;
        } else if (op == 'e') {
            maxError = atoi(optarg);
            hasEdit = 1;
        } else if (op == 'c') {
            hasCount = 1;
        } else if (op == 'p') {
            patternFileName = optarg;
            hasPatternFile = 1;
        }
    }

    if (hasHelp) {
        help();
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

        vector<string> fileNames;

        glob_t *vector_ptr = new glob_t;

        while (optind < argc) {
            glob(argv[optind], GLOB_NOSORT|GLOB_MARK, NULL, vector_ptr);

            for (int i = 0; i < vector_ptr->gl_pathc; i++) {
                fileNames.push_back(vector_ptr->gl_pathv[i]);
            }

            optind++;
        }

        if (hasEdit) {
            approximateMatch(fileNames, patterns, hasCount, maxError);
        } else {
            exactMatch(fileNames, patterns, hasCount);
        }
    }

    return 0;
}

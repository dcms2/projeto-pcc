#ifndef BITSET
#define BITSET

#include <bits/stdc++.h>

using namespace std;

class Bitset {
private:
    long long *array;
    int length; //length of the array
    int numBits; //number of bits used

public:

    Bitset() {}
    
    Bitset(int size, long long allOne = 0) : length((size+63)/64), numBits(size) {
        array = new long long[length];
        for (int i = 0; i < length; ++i) {
            array[i] = -allOne;
        }
        int last = size % 64;
        if (last > 0) {
            array[length-1] &= (1LL << last) - 1;
        }
    }

    Bitset(const Bitset &other) {
        length = other.length;
        array = new long long[other.length];
        numBits = other.numBits;
        memcpy(array, other.array, sizeof(long long) * length);
    }

    void update_array(const Bitset &other) {
        memcpy(array, other.array, sizeof(long long) * length);
    }

    void delete_array() {
        delete[] array;
        array = NULL;
    }

    long long& operator [] (int i) const {
        return array[i];
    }

    void andd(const Bitset &other) const {
        for (int i = 0; i < length; ++i) {
            array[i] &= other[i];
        }
    }

    void orr(const Bitset &other) {
        if (length >= other.length) {
            for (int i = 0; i < length; ++i) {
                array[i] |= other[i];
            }
        } else {
            long long* aux = new long long[other.length];
            for (int i = 0; i < length; ++i) {
                aux[i] = array[i] | other[i];
            }
            for (int i = length; i < other.length; ++i) {
                aux[i] = other[i];
            }
            delete[] array;
            array = aux;
        }
    }

    bool operator < (const Bitset &other) const {
        if (length == other.length) {
            for (int i = length-1; i >= 0; --i) {
                if (static_cast<unsigned long long>(array[i]) < other[i]) return true;
            }
            return false;
        } else if (length > other.length) {
            for (int i = length-1; i >= other.length; --i) {
                if (array[i] != 0) return false;
            }
            for (int i = other.length-1; i >= 0; --i) {
                if (static_cast<unsigned long long>(array[i]) < other[i]) return true;
            }
            return false;
        } else {
            for (int i = other.length-1; i >= length; --i) {
                if (other[i] != 0) return false;
            }
            for (int i = length-1; i >= 0; --i) {
                if (static_cast<unsigned long long>(array[i]) < other[i]) return true;
            }
            return false;
        }
    }

    void negate() {
        for (int i = 0; i < length; ++i) {
            array[i] = ~array[i];
        }
        int last = numBits % 64;
        if (last > 0) {
            array[length-1] &= (1LL << last) - 1;
        }
    }

    void shift() {
        for (int i = length-1; i > 0; --i) {
            array[i] <<= 1;
            array[i] |= (array[i-1] >> 63) & 1;
        }
        array[0] <<= 1;
        int last = numBits % 64;
        if (last > 0) {
            array[length-1] &= (1LL << last) - 1;
        }
    }

    int get(int x) {
        int box = x/64;
        int pos = x%64;
        return (array[box] >> pos) & 1;
    }

    void set(int x) {
        int box = x/64;
        int pos = x%64;
        array[box] |= 1LL << pos;
    }

    void clear(int x) {
        int box = x/64;
        int pos = x%64;
        array[box] &= ~(1LL << pos);
    }

    void print() {
        if (length == 1) {
            for (int j = numBits-1; j >= 0; --j) {
                printf("%c", int((array[0] >> j) & 1) + '0');
            }
        } else {
            for (int j = numBits%64; j > 0; --j) {
                printf("%c", int((array[length-1] >> (j-1)) & 1) + '0');
            }
            for (int i = length-1-(numBits%64 != 0); i >= 0; --i) {
                for (int j = 63; j >= 0; --j) {
                    printf("%c", int((array[i] >> j) & 1) + '0');
                }
            }
        }
        puts("");
    }

};

#endif
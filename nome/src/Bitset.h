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
    
    Bitset(int size, long long allOne = 0) : length((size+63)/64), numBits(size) {
        array = new long long[length];
        for (int i = 0; i < length; ++i) {
            array[i] = -allOne;
        }
    }

    long long& operator [] (int i) const {
        return array[i];
    }

    Bitset operator & (const Bitset &other) const {
        Bitset ret = Bitset(max(numBits, other.numBits));
        for (int i = 0, end = min(length, other.length); i < end; ++i) {
            ret[i] = array[i] & other[i];
        }
        
        long long* aux = (length > other.length) ? array : other.array;

        for (int i = min(length, other.length); i < ret.length; ++i) {
            ret[i] = aux[i];
        }
        return ret;
    }

    Bitset operator | (const Bitset &other) const {
        Bitset ret = Bitset(max(numBits, other.numBits));
        for (int i = 0, end = min(length, other.length); i < end; ++i) {
            ret[i] = array[i] | other[i];
        }
        
        long long* aux = (length > other.length) ? array : other.array;

        for (int i = min(length, other.length); i < ret.length; ++i) {
            ret[i] = aux[i];
        }
        return ret;
    }

    Bitset operator << (int x) const {
        if (x != 1) assert(false);
        Bitset ret = Bitset(numBits);
        for (int i = 0; i < length; ++i) {
            ret[i] = array[i];
        }
        for (int i = length-1; i > 0; --i) {
            ret[i] <<= 1;
            ret[i] |= (ret[i-1] >> 63) & 1;
        }
        ret[0] <<= 1;
        return ret;
    }

    Bitset operator ~ () const {
        Bitset ret = Bitset(numBits);
        for (int i = 0; i < length; ++i) {
            ret[i] = ~array[i];
        }
        return ret;
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
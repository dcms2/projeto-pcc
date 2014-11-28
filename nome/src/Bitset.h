#ifndef BITSET
#define BITSET

#include <bits/stdc++.h>

using namespace std;

class Bitset {
private:
    long long* array;
    int length; //length of the array
    int numBits; //number of bits used

public:

    Bitset() {}
    
    Bitset(int size, int allOne = 0) : length((size+63)/64), numBits(size) {
        array = new long long[length];
        memset(array, -allOne, sizeof(long long) * length);
        int last = size % 64;
        if (last > 0) {
            array[length-1] &= (1LL << last) - 1;
        }
    }

    Bitset(const Bitset &other) {
        length = other.length;
        numBits = other.numBits;
        array = new long long[length];
        memcpy(array, other.array, length * sizeof(long long));
    }

    /*
        Update this array with the data from other.array.
    */
    void update_array(const Bitset &other) {
        memcpy(array, other.array, length * sizeof(long long));
    }

    /*
        Delete the array.
    */
    void delete_array() {
        delete[] array;
        array = NULL;
    }

    /*
        Bitwise and.
    */
    void andd(const Bitset &other) {
        for (int i = 0; i < length; ++i) {
            array[i] &= other.array[i];
        }
    }

    /*
        Bitwise or.
    */
    void orr(const Bitset &other) {
        for (int i = 0; i < length; ++i) {
            array[i] |= other.array[i];
        }
    }

    /*
        Returns true if and only if the number represented by this array is smaller than other.array.
    */
    bool operator < (const Bitset &other) const {
        for (int i = length-1; i >= 0; --i) {
            if (static_cast<unsigned long long>(array[i]) < other.array[i]) return true;
        }
        return false;
    }

    /*
        Bitwise not.
    */
    void negate() {
        for (int i = 0; i < length; ++i) {
            array[i] = ~array[i];
        }
        int last = numBits % 64;
        if (last > 0) {
            array[length-1] &= (1LL << last) - 1;
        }
    }

    /*
        Shift the elements once from the least significant bit to the most one.
    */
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

    /*
        Get the xth-bit.
    */
    int get(int x) {
        int box = x/64;
        int pos = x%64;
        return (array[box] >> pos) & 1;
    }

    /*
        Set the x-th bit.
    */
    void set(int x) {
        int box = x/64;
        int pos = x%64;
        array[box] |= 1LL << pos;
    }


    /*
        Clear the x-th bit.
    */
    void clear(int x) {
        int box = x/64;
        int pos = x%64;
        array[box] &= ~(1LL << pos);
    }

    /*
        Print the bits.
    */
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
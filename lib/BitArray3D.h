#pragma once

#include <algorithm>
#include <cstdint>
#include <iostream>

const int kPower16 = 65536;
const int kPower17 = 131072;

class BitArray {
private:
    int32_t xSize = 0;
    int32_t ySize = 0;
    int32_t zSize = 0;
    int32_t xIterator = -1;
    int32_t yIterator = -1;
    int32_t zIterator = -1;
    uint16_t* data16;
    uint8_t*  data8;

public:
    BitArray(int32_t x, int32_t y, int32_t z);
    //explicit BitArray();
    static BitArray makeArray(int32_t x, int32_t y, int32_t z);

//    BitArray makeArr(int32_t x, int32_t y, int32_t z){
//        return BitArray(x, y, z);
//    }
    int CalculateBitPosition(int32_t x, int32_t y, int32_t z);
    void CalculateIndex(int32_t x, int32_t y, int32_t z, int32_t value);
    void InstallValue(int32_t itIndex, int32_t value);

    BitArray operator+(BitArray& rhs);
    BitArray operator-(BitArray& rhs);
    BitArray operator*(int32_t value);
    BitArray& operator=(int value);
    BitArray& operator[](const int itIndex);
    explicit operator int();

    friend std::ostream& operator<<(std::ostream& stream, BitArray& arr);
    friend std::istream& operator>>(std::istream& stream, BitArray& arr);

    ~BitArray();
};

#include "BitArray3D.h"

BitArray::BitArray(int32_t x, int32_t y, int32_t z) : xSize(x), ySize(y), zSize(z), xIterator(-1), yIterator(-1), zIterator(-1) {
    int32_t size = xSize * ySize * zSize;
    int32_t num8BitWords = size / 8 + 1;

    data16 = new uint16_t[size]{0};
    data8 = new uint8_t[num8BitWords]{0};
}

BitArray BitArray::makeArray(int32_t x, int32_t y, int32_t z) {
    return BitArray(x, y, z);
}

int BitArray::CalculateBitPosition(int32_t x, int32_t y, int32_t z) {
    int32_t itIndex = x * ySize * zSize + y * zSize + z;

    int32_t additional_bit_first = itIndex / 8;
    int8_t additional_bit_second = itIndex % 8;
    int32_t position = ((data8[additional_bit_first] >> additional_bit_second) & 1) * kPower16;

    return data16[itIndex] + position;
}

void BitArray::CalculateIndex(int32_t x, int32_t y, int32_t z, int32_t value) {
    InstallValue(x * ySize * zSize + y * zSize + z, value);
}

void BitArray::InstallValue(int32_t itIndex, int32_t value) {
    int32_t additional_bit_first = itIndex / 8;
    int8_t additional_bit_second = itIndex % 8;
    value %= kPower17;
    if ((value >> 16) & 1) {
        data8[additional_bit_first] |= (1 << additional_bit_second);
    } else {
        data8[additional_bit_first] &= ~(1 << additional_bit_second);
    }
    data16[itIndex] = value % kPower16;
}

BitArray BitArray::operator+(BitArray& rhs) {
    if (xSize != rhs.xSize || ySize != rhs.ySize || zSize != rhs.zSize) {
        std::cerr << "Error: Index out of range!";
        exit(1);
    }
    BitArray ans = makeArray(xSize, ySize, zSize);
    int32_t siz = xSize * ySize * zSize;
    for (int i = 0; i < siz; ++i) {
        int32_t additional_bit_first = i / 8;
        int8_t additional_bit_second = i % 8;

        size_t valueSum = data16[i] + ((data8[additional_bit_first] >> additional_bit_second) & 1) * kPower17 +
                  rhs.data16[i] + ((rhs.data8[additional_bit_first] >> additional_bit_second) & 1) * kPower17;

        ans.InstallValue(i, valueSum);
    }

    return ans;
}

BitArray BitArray::operator-(BitArray& rhs) {
    if (xSize != rhs.xSize || ySize != rhs.ySize || zSize != rhs.zSize) {
        std::cerr << "Error: Index out of range!";
        exit(1);
    }
    BitArray ans = makeArray(xSize, ySize, zSize);
    int32_t siz = xSize * ySize * zSize;
    for (int i = 0; i < siz; ++i) {
        int32_t additional_bit_first = i / 8;
        int8_t additional_bit_second = i % 8;
        int valueDifference = data16[i] - ((data8[additional_bit_first] >> additional_bit_second) & 1) * kPower17 -
                  rhs.data16[i] - ((rhs.data8[additional_bit_first] >> additional_bit_second) & 1) * kPower17;
        ans.InstallValue(i, std::max(0, valueDifference));
    }

    return ans;
}

BitArray BitArray::operator*(int32_t value) {
    if(value < 0) {
        std::cerr << "Error: Index out of range!";
        exit(1);
    }
    BitArray ans = makeArray(xSize, ySize, zSize);
    int32_t siz = xSize * ySize * zSize;
    for (int i = 0; i < siz; ++i) {
        int32_t additional_bit_first_lhs = i / 8;
        int8_t additional_bit_second_lhs = i % 8;
        size_t valueMult = data16[i] + ((data8[additional_bit_first_lhs] >> additional_bit_second_lhs) & 1) * kPower17;
        ans.InstallValue(i, valueMult * value);
    }

    return ans;
}

BitArray& BitArray::operator=(int value) {
    if(value < 0) {
        std::cerr << "Error: Index out of range!";
        exit(1);
    }
    if (xIterator != -1 && yIterator != -1 && zIterator != -1) {
        CalculateIndex(xIterator, yIterator, zIterator, value);
        xIterator = yIterator = zIterator = -1;
    } else {
        std::cerr << "Error: Index out of range!";
        exit(1);
    }

    return (*this);
}

BitArray& BitArray::operator[](const int itIndex) {
    if (xIterator == -1) {
        xIterator = itIndex;
    } else if (yIterator == -1) {
        yIterator = itIndex;
    } else if (zIterator == -1) {
        zIterator = itIndex;
    }

    return (*this);
}

BitArray::operator int() {
    return CalculateBitPosition(xIterator, yIterator, zIterator);
}

std::ostream& operator<<(std::ostream& stream, BitArray& f) {
    if (f.xIterator != -1 && f.yIterator != -1 && f.zIterator != -1) {
        std::cout << f.CalculateBitPosition(f.xIterator, f.yIterator, f.zIterator);
        f.xIterator = f.yIterator = f.zIterator = -1;
        return stream;
    }
    for (int i = 0; i < f.xSize; ++i) {
        for (int j = 0; j < f.ySize; ++j) {
            for (int k = 0; k < f.zSize; ++k) {
                std::cout << f.CalculateBitPosition(i, j, k) << ' ';
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    return stream;
}

std::istream& operator>>(std::istream& stream, BitArray& f) {
    int32_t siz = f.xSize * f.ySize * f.zSize;
    for (int i = 0; i < siz; ++i) {
        int32_t value;
        stream >> value;
        f.InstallValue(i, value);
    }

    return stream;
}

BitArray::~BitArray() {
    delete[] data16;
    delete[] data8;
}
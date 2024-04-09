#include <iostream>
#include "C:\Users\Ирина\Desktop\labwork51-ereminair-main\lib\BitArray3D.cpp"

int main() {

    BitArray bitArray = BitArray::makeArray(3, 3, 3);

    bitArray[0][0][0] = 2;
    bitArray[1][1][1] = 3;

    int multiplier = -1;

    BitArray result = bitArray * multiplier;

    std::cout << result;

    return 0;
}

#include "iRequestHandler.h"
#include <stdlib.h>
#include <vector>
#include <string>

//void iRequestHandler::setBit(char &src,char index, bool val){
//    if (val)
//        src |= (1<<index);
//    else
//        src &= ~(1<<index);
//}

//bool iRequestHandler::getBit(char src, char index){
//    return src &= (1<<index);
//}

//std::string iRequestHandler::fromBoolVectorToSting(const std::vector<bool>& vec){
//    const int charBitSize = 7;
//    size_t cStrSize = vec.size() / charBitSize;
//    char cStr[cStrSize];
//    memset(cStr, 0, cStrSize);

//    int bitCount = 0;
//    for(size_t i = 0; i < cStrSize; ++i){
//        for(size_t bitNum = 0; bitNum < charBitSize; ++bitNum){
//            setBit(cStr[i], bitNum, vec[bitCount]);
//            ++bitCount;
//        }
//    }
//    std::string str(cStr);
//    return str;
//}

//std::vector<bool> iRequestHandler::fromStringToBoolVector(const std::string& str){
//    const int charBitSize = 7;
//    std::vector<bool> vec((str.length() + 1) * charBitSize);
//    int bitCount = 0;
//    for(size_t i = 0; i < str.length() + 1; ++i){
//        for(size_t bitNum = 0; bitNum < charBitSize; ++bitNum){
//            vec[bitCount] = getBit(str[i], bitNum);
//            ++bitCount;
//        }
//    }
//    return vec;
//}

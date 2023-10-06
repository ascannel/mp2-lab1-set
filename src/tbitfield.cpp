// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
// Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
// Переработано в соответсвии с C++11 (частично) Пановым А.А. 2021
// Битовое поле

#include <limits>
#include <iostream>
#include "tbitfield.h"

TBitField::TBitField(size_t len)
{
    bitLen = len;
    if (len % (sizeof(uint) * 8) != 0)
        memLen = len / (sizeof(uint) * 8) + 1;
    else
        memLen = len / (sizeof(uint) * 8);
    pMem = new uint[memLen];
    for (int i = 0; i < memLen; i++) pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    bitLen = bf.bitLen;
    memLen = bf.memLen;
    pMem = new uint[memLen];
    for (size_t j = 0; j < memLen; j++) {
        pMem[j] = bf.pMem[j];
    }
}

size_t TBitField::getIndex(const size_t n) const  // индекс в pМем для бита n
{
    if (n < bitLen) {
        return n % (sizeof(uint) * 8);
    }
    else
        throw - 1;
}

uint TBitField::getMask(const size_t n) const // битовая маска для бита n
{
    uint mask = (uint)1 << n;
    return mask;
}

// доступ к битам битового поля
uint TBitField::getLength() const // получить длину (к-во битов)
{
    return (uint)bitLen;
}

size_t TBitField::getNumBytes() const // получить количество байт выделенной памяти
{
    return memLen * sizeof(uint);
}

void TBitField::setBit(const size_t n) // установить бит
{
    size_t indexSet = n / (sizeof(uint) * 8);
    uint mask = getMask(getIndex(n));
    if (n < bitLen) {
        pMem[indexSet] = pMem[indexSet] | mask;
    }
    else
        throw - 1;
}

void TBitField::clrBit(const size_t n) // очистить бит
{
    size_t indexClr = n / (sizeof(uint) * 8);
    uint mask = ~(getMask(getIndex(n)));
    if (n < bitLen) {
        pMem[indexClr] = pMem[indexClr] & mask;
    }
    else
        throw - 1;
}

bool TBitField::getBit(const size_t n) const // получить значение бита
{
    if (n > bitLen)
        throw - 1;
    if ((pMem[n / (sizeof(uint) * 8)]) & (getMask(getIndex(n))))
        return true;
    return false;
}

// битовые операции
TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (*this == bf)
        return *this;
    else
    {
        memLen = bf.memLen;
        bitLen = bf.bitLen;

        pMem = new uint[memLen];

        for (int i = 0; i < memLen; i++)
        {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

bool TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (bitLen != bf.bitLen) return false;
    int i = 0;
    for (; i < bitLen; i++)
    {
        if (this->getBit(i) != bf.getBit(i)) return false;
    }
    return true;
}

bool TBitField::operator!=(const TBitField& bf) const // сравнение
{
    if (bitLen != bf.bitLen) return true;
    for (int i = 0; i < memLen; i++) {
        if (pMem[i] != bf.pMem[i]) return true;
    }
    return false;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    TBitField max(0), min(0);
    if (bitLen >= bf.getLength())
    {
        min = bf;
        max = *this;
    }
    else if (bitLen < bf.getLength())
    {
        max = bf;
        min = *this;
    }
    for (size_t i = 0; i < min.getLength(); ++i)
    {
        if (min.getBit(i))
            max.setBit(i);
    }
    return max;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    if (bitLen < bf.bitLen) {
        TBitField a(bf.bitLen);
        for (int i = 0; i < memLen; i++) {
            a.pMem[i] = pMem[i] & bf.pMem[i];
        }
        return a;
    }
    else {
        TBitField a(bitLen);
        for (int i = 0; i < bf.memLen; i++) {
            a.pMem[i] = pMem[i] & bf.pMem[i];
        }
        return a;
    }

}

TBitField TBitField::operator~() // отрицание
{
    TBitField res(this->getLength());
    for (size_t i = 0; i < bitLen; ++i)
    {
        if (!getBit(i))
            res.setBit(i);
    }
    return res;
}

TBitField::~TBitField()
{
    delete[] pMem;
    bitLen = 0;
    memLen = 0;
}

// ввод/вывод
std::istream& operator>>(std::istream& istr, TBitField& bf) // ввод
{
    for (int i = 0; i < bf.memLen; ++i)
    {
        istr >> bf.pMem[i];
    }
    return istr;
}

std::ostream& operator<<(std::ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.bitLen; i++)
    {
        if (bf.getBit(i))
        {
            ostr << '1';
        }
        else
        {
            ostr << '0';
        }
    }
    return ostr;
}

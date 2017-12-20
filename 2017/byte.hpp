#pragma once
#ifndef _BYTE_HPP
#define _BYTE_HPP


enum class byte : unsigned char
{
};

#pragma region arithmetic
inline int operator-(const byte &left, const int &right) {
	return (int)left - right;
}
#pragma endregion


#pragma region binary operators
// XOR
inline byte operator^(const byte &left, const byte &right)
{
	return (byte)(((int)left ^ (int)right) & 0xFF);
}
inline void operator^=(byte &left, const byte &right)
{
	left = left ^ right;
}
// bit-shift
inline byte operator>>(const byte &left, const int &right)
{
	return (byte)((int)left >> right);
}
inline byte operator<<(const byte &left, const int &right)
{
	return (byte)((int)left << right);
}
#pragma endregion


#pragma region comparisons
// less than
inline bool operator<(const byte &left, const byte &right) {
	return (int)left < (int)right;
}
inline bool operator<(const byte &left, const int &right) {
	return (int)left < right;
}
inline bool operator<(const int &left, const byte &right) {
	return left < (int)right;
}
// greater than
inline bool operator>(const byte &left, const byte &right) {
	return (int)left > (int)right;
}
inline bool operator>(const byte &left, const int &right) {
	return (int)left > right;
}
inline bool operator>(const int &left, const byte &right) {
	return left > (int)right;
}
#pragma endregion


inline byte operator "" _b(unsigned long long value) {
	return (byte)value;
}

#endif

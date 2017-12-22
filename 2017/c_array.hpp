#pragma once
#ifndef _C_ARRAY_HPP
#define _C_ARRAY_HPP

template <typename _Ty>
class c_array
{
  private:
	_Ty *_d;
	size_t _l;

  private:
	c_array(const c_array &o)
	{
	}

  public:
	const size_t &length;
	c_array(c_array &&other) : _l(other._l), _d(other._d), length(_l)
	{
		other._d = nullptr;
		other._l = 0;
	}
	c_array(_Ty *arr, size_t length) : _l(length), _d(arr), length(_l)
	{
	}
	~c_array()
	{
		if (_d != nullptr)
			delete[] _d;
		_d = nullptr;
		_l = 0;
	}
	inline _Ty &operator[](size_t index)
	{
		return _d[index];
	}
	inline _Ty *begin() { return _d; }
	inline _Ty *end() { return _d + length; }
};

#endif

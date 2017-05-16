#ifndef G_EXCEPTION_H
#define G_EXCEPTION_H

#pragma once
#include <exception>

struct GEmptyPoolException : virtual public std::exception
{
	const char* what() const throw()
	{
		return "There are no components of this type!";
	}
};

struct GPoolCastException : virtual public std::exception
{
	const char* what() const throw()
	{
		return "ERROR with casting!";
	}
};


#endif // G_EXTENSIONS_H


#pragma once
#ifndef HASHABLE_H_INCLUDED
#define HASHABLE_H_INCLUDED
#include <Windows.h>
#include <openssl/sha.h>
#include "general.h"

class Hashable
{
public:
	virtual std::vector<unsigned char> hash() = 0;
};

#endif // HASHABLE_H_INCLUDED

#ifndef UTIL_H
#define UTIL_H

#include<string>
#include<sstream>
#include<sys/time.h>

#define IS_32BIT() (sizeof(int *)==4)

class util
{
public:
	util();
	~util();
	static std::string getTime();
};

#endif
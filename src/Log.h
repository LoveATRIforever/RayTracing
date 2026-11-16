#pragma once
#include<iostream>
#include<cassert>

#define LOG(x) std::cout << (x) << std::endl;
#define ASSERT(x,info) if(!(x)) { std::cout << "Assertion Failed: " << (info) << std::endl; assert(false); }

// cpp_premain.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
using namespace std;
using std::cout;

int func()
{
	cout<<"func() called before main()"<<endl;
	return 100;
}
class A
{
public:
	A()
	{
		cout<<"A() constructor called"<<endl;
	}
	~A()
	{
		cout<<"~A() destructor called"<<endl;
	}
};

A a;

int g_iValue = func();


int _tmain(int argc, _TCHAR* argv[])
{
	cout<<"main() called"<<endl;
	return 0;
}


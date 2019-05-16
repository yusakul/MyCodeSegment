// c_premain.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>

int before_main1()  
{  
	printf("before_main1()\n");  

	return 0;  
}  
int before_main2()  
{  
	printf("before_main2()\n");  

	return 0;  
}  
int after_main()
{
	printf("after_main()\n");
	return 0;
}
/*
__CRTInit����һЩ��ʼ��������
����C�⡢C�ĳ�ʼ��������C++�⡢C++�ĳ�ʼ�������ȡ�
C��C++�ֱ���һ�ű��������ʼ������ָ�룬
ÿ������ʹ��2��ָ������ȷ��Χ��
__CRTInit�����ε�����2�����еĺ�����
C��ʼ��������[ __xi_a, __xi_z]
C++��ʼ��������: [ __xc_a, __xc_z]
���ڶ��մ���ע�ͣ��ͻ����������Ƕδ�������á�
ͨ������Ķ����ơ�.CRT$XIU������.CRT$XCU����
���������before1����ڡ�C��ʼ���������У���������
[__xi_a, ..., before1(xiu), ..., __xi_z].
ͬ��before2��ᱻ���������ڡ�C++��ʼ���������У�������
[__xc_a, ..., before2(xcu), ..., __xc_z]��
*/

typedef int func();  

#pragma data_seg(".CRT$XIU")  
static func * before1[] = { before_main1 };  

#pragma data_seg(".CRT$XCU")  
static func * before2[] = { before_main2 };  

#pragma data_seg() 

int _tmain(int argc, _TCHAR* argv[])
{
	_onexit(after_main);
	printf("hello world\n");

	return 0;
}

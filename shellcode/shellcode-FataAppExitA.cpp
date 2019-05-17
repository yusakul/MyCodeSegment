

char shellcode[] = "\x31\xd2\xb2\x30\x64\x8b\x12\x8b\x52\x0c\x8b\x52\x1c\x8b\x42"
"\x08\x8b\x72\x20\x8b\x12\x80\x7e\x0c\x33\x75\xf2\x89\xc7\x03"
"\x78\x3c\x8b\x57\x78\x01\xc2\x8b\x7a\x20\x01\xc7\x31\xed\x8b"
"\x34\xaf\x01\xc6\x45\x81\x3e\x46\x61\x74\x61\x75\xf2\x81\x7e"
"\x08\x45\x78\x69\x74\x75\xe9\x8b\x7a\x24\x01\xc7\x66\x8b\x2c"
"\x6f\x8b\x7a\x1c\x01\xc7\x8b\x7c\xaf\xfc\x01\xc7\x68\x79\x74"
"\x65\x01\x68\x6b\x65\x6e\x42\x68\x40\x42\x72\x6f\x89\xe1\xfe"
"\x49\x0b\x31\xc0\x51\x50\xff\xd7";


int main(int argc, char **argv) 
{ 
	int(*f)(); 
	f = (int(*)())shellcode; 
	(int)(*f)(); 
}



void ShellCodeFunction()
{
	typedef void(__stdcall *Pfn_FatalAppExit)(UINT, LPCSTR);
	Pfn_FatalAppExit pfnFatalAppExit = NULL;

	PCHAR pBuf = NULL;
	_asm
	{
		mov eax, fs:[0x30];//找到PEB
		mov eax, [eax + 0x0C];//找到了LDR
		mov eax, [eax + 0x0C];//找到了第一个节点
		mov eax, [eax];       //找到了ntdll
		mov eax, [eax];       //找到了kernel32.dll
		mov ebx, dword ptr ds : [eax + 0x18];	//在0x18偏移处取出基址(DllBase)
		mov pBuf, ebx;
	}

	//kernel32.dll 解析
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)pBuf;	//kernel32.dll的dos头
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(pDos->e_lfanew + pBuf);

	PIMAGE_DATA_DIRECTORY pExportDir =
		(pNt->OptionalHeader.DataDirectory + 0);

	PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)
		(pExportDir->VirtualAddress + pBuf);
	//后面的步骤

	//1  找到三个表：名称，地址，序号
	PDWORD pAddress = (PDWORD)(pExport->AddressOfFunctions + pBuf);
	PDWORD pName = (PDWORD)(pExport->AddressOfNames + pBuf);
	PWORD  pId = (PWORD)(pExport->AddressOfNameOrdinals + pBuf);
	PVOID Address = 0;

	//2  在名称表中去遍历GetProcAddress这个字符串
	for (size_t i = 0; i < pExport->NumberOfNames; i++)
	{
		char* Name = (pName[i] + pBuf);
		if (strcmp(Name, "FatalAppExitA") == 0)
		{
			Address = pAddress[pId[i]] + pBuf;
		}
	}
	pfnFatalAppExit = (Pfn_FatalAppExit)Address;

	pfnFatalAppExit(0, "by Yusakul");
}

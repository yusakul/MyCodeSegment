#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>
#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)
void HandleError(const char *s);

int main()
{
	//--------------------------------------------------------------------
	// 申明、初始化变量

	HCRYPTPROV hCryptProv;        // CSP句柄 

	HCRYPTKEY hKey;               // 公私密钥对句柄
	CHAR szUserName[100];         // 密钥容器名称 

	DWORD dwUserNameLen = 100;    // 密钥容器名称长度
	LPCWSTR UserName = (LPCWSTR)("ticai");        // 用户名，作为密钥容器名

	//--------------------------------------------------------------------
	// 创建默认密钥容器。若需要创建一个密钥容器则替换CryptAcquireContext函数中的“NULL”

	if (CryptAcquireContext(
		&hCryptProv,               // CSP句柄
		UserName,                  // 密钥容器名 
		NULL,                      // 使用默认CSP
		PROV_RSA_FULL,             // CSP类型
		0))                        // 标志位
	{
		printf("一个名为 %s 的密钥容器的句柄 \n", UserName);
		printf("已经获取.\n\n");
	}
	else
	{
		//--------------------------------------------------------------------
		// 如果获取CSP出现错误，则创建一个新的默认密钥容器

		if (CryptAcquireContext(
			&hCryptProv,
			UserName,
			NULL,
			PROV_RSA_FULL,
			CRYPT_NEWKEYSET)) //创建密钥容器
		{
			printf("一个新的密钥容器已经被创建.\n");
		}
		else
		{
			HandleError("不能创建新的密钥容器.\n");
		}
	} // End of else

	//--------------------------------------------------------------------
	// 已获取包含密钥容器的CSP，获取其密钥容器名。

	if (CryptGetProvParam(
		hCryptProv,               // CSP句柄
		PP_CONTAINER,             // 获取参数类型，密钥容器名
		(BYTE *)szUserName,       // 密钥容器名指针
		&dwUserNameLen,           // 密钥容器名长度，设为100
		0))
	{
		printf("一个已经包含密钥容器的CSP句柄已被获取并且 \n");
		printf("所包含的密钥容器的名称为 %s.\n\n", szUserName);
	}
	else
	{
		// 获取其密钥容器名出错
		HandleError("一个CSP句柄已经获取或创建, 但是\
		  在获取密钥容器名称的时候发生了错误.\n");
	}

	//--------------------------------------------------------------------
	// 密钥容器已存在

	if (CryptGetUserKey(
		hCryptProv,                     // CSP句柄
		AT_SIGNATURE,                   // 密钥类型，签名密钥
		&hKey))                         // 密钥句柄
	{
		printf("签名密钥已经存在.\n");
	}
	else
	{
		printf("没有签名密钥存在.\n");
		if (GetLastError() == NTE_NO_KEY)
		{
			//----------------------------------------------------------------
			// 当错误为：密钥不存在时，重新创建签名密钥对。

			printf("签名密钥已经不存在.\n");
			printf("创建一个签名密钥对.\n");
			if (CryptGenKey(
				hCryptProv,
				AT_SIGNATURE,
				0,
				&hKey))
			{
				printf("已经创建了一个签名密钥对.\n");
			}
			else
			{
				HandleError("创建签名密钥出错.\n");
			}
		}
		else
		{
			HandleError("在获得签名密钥时发生了一个不同于 NTE_NO_KEY 的错误.\n");
		}
	} // End of if

	printf("一个签名密钥对已经存在或被创建.\n\n");

	CryptDestroyKey(hKey);

	// 检查交换密钥
	if (CryptGetUserKey(
		hCryptProv,
		AT_KEYEXCHANGE,
		&hKey))
	{
		printf("有一对交换密钥存在. \n");
	}
	else
	{
		printf("没有交换密钥存在.\n");
		// 检查交换密钥是否存在
		if (GetLastError() == NTE_NO_KEY)
		{
			// 创建交换密钥
			printf("交换密钥不存在.\n");
			printf("正在试图创建一个交换密钥对.\n");
			if (CryptGenKey(
				hCryptProv,
				AT_KEYEXCHANGE,
				0,
				&hKey))
			{
				printf("交换密钥对已经存在.\n");
			}
			else
			{
				HandleError("在试图创建交换密钥时发生错误.\n");
			}
		}
		else
		{
			HandleError("发生了一个不同于 NTE_NO_KEY 的错误.\n");
		}
	}

	printf("一个交换密钥对已经存在或被创建.\n\n");

	CryptDestroyKey(hKey); //销毁密钥句柄

	CryptReleaseContext(hCryptProv, 0); //释放CSP句柄

	printf("一切就绪.\n");
	printf("在 %s 密钥容器中存在\n", szUserName);
	printf("一个签名密钥对和一个交换密钥对.\n");
	system("pause");
} // End of main

//  HandleError：错误处理函数，打印错误信息，并退出程序
void HandleError(const char *s)
{
	printf("程序执行发生错误!\n");
	printf("%s\n", s);
	printf("错误代码为: %x.\n", GetLastError());
	printf("程序终止执行!\n");
	system("pause");
	exit(1);
}

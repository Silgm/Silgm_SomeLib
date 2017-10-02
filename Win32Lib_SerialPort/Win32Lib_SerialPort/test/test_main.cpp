#include "test_main.h"

#include <cstdio>
#include <cstdlib>

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG //жидиnew  
#define new  new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

int main(int argc, char *argv[]) {
	int returnVal = test_main(argc, argv);
	int MemleaksNums = _CrtDumpMemoryLeaks();
	printf("Leaks:%d\n", MemleaksNums);
	system("pause");
	return returnVal;
}


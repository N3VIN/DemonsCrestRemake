#include "pch.h"
#include "Core.h"
#include <ctime>

void StartHeapControl();
void DumpMemoryLeaks();

int main( int argc, char *argv[] )
{
	srand(static_cast<unsigned int>(time(nullptr)));

	StartHeapControl();
	// have to change the window resoulution later.
	Core* pCore{ new Core{ Window{ "Demon's Crest - Amarendranath, Nevin - 1DAE11", 512.f , 448.f } } };

	//Core* pCore{ new Core{ Window{ "Demon's Crest - Amarendranath, Nevin - 1DAE11", 2048.f , 255.f } } };
	pCore->Run();
	delete pCore;

	DumpMemoryLeaks();
	return 0;
}


void StartHeapControl()
{
#if defined(DEBUG) | defined(_DEBUG)
	// Notify user if heap is corrupt
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	// Report detected leaks when the program exits
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Set a breakpoint on the specified object allocation order number
	//_CrtSetBreakAlloc( 453 );
#endif
}

void DumpMemoryLeaks()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif
}



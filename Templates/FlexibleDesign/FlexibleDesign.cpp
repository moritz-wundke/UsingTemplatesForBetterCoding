#include <iostream>
#include "Types.h"

int main()
{

	SimpleObjectManager Manager;
	Manager.CreateOrNull()->SayHi();

	SimpleExtendedObjectManager ExtendedManager;
	ExtendedManager.CreateOrNull()->SayHi();

	SimpleObjectManagerFunc OtherManager;
	OtherManager.CreateOrNull<ExtendedObject>()->SayHi();

    return 0;
}


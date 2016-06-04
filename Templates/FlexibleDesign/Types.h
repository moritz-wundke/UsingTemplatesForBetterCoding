#pragma once

#include <iostream>
#include "CreationPolicies.h"

class BaseObject
{
public:
	BaseObject() { }

	virtual void SayHi() { std::cout << "Hi from BaseObject" << std::endl; }
};

class ExtendedObject : public BaseObject
{
public:
	ExtendedObject() : BaseObject() { }

	virtual void SayHi() override { std::cout << "Hi from ExtendedObject" << std::endl; }
};

template <class T, template <class> class CreationPolicy>
class ObjectManager : public CreationPolicy<T>
{

};

// Predefine our object managers for each type and CreationPOlicy
typedef ObjectManager< BaseObject, NewCreator > SimpleObjectManager;
typedef ObjectManager< ExtendedObject, NewCreator > SimpleExtendedObjectManager;

class SimpleObjectManagerFunc : public NewCreatorFunc
{

};

#pragma once
#include <iostream>
#include <cassert>

//
// Types and helpers
//

class TypeA
{
public:
	std::string SomeData;
};

class TypeB : TypeA
{
public:
	std::string MoreData;
};

class BaseController
{

};

class ControllerA : BaseController
{

};

class ControllerB : BaseController
{

};

class BaseContainer
{

};

class ContainerA : BaseContainer
{

};

class ContainerB : BaseContainer
{

};

template<class ContainerArg>
class ContainerWithArg : BaseContainer
{

};

typedef unsigned char      uint8;    // 8 -bit unsigned


//
// Compile time assertions
//

template <class Destination, class Source>
Destination ReinterpCastChecked_Runtime(Source source)
{
	// We ensure that the destination type can hold all bits from the source type
	assert(sizeof(Source) <= sizeof(Destination));
	return reinterpret_cast<Destination>(source);
}


// STATIC_CHECK License START

////////////////////////////////////////////////////////////////////////////////
// The Loki Library
// Copyright (c) 2001 by Andrei Alexandrescu
// This code accompanies the book:
// Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design 
//     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// The author or Addison-Welsey Longman make no representations about the 
//     suitability of this software for any purpose. It is provided "as is" 
//     without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

template<bool> struct ctCheck;
template<> struct ctCheck<true> {};

#define STATIC_CHECK(expr, msg) \
    { ctCheck<((expr) != 0)> ERROR_##msg; (void)ERROR_##msg; } 

// STATIC_CHECK License END

template <class Destination, class Source>
Destination ReinterpCastChecked(Source source)
{
	STATIC_CHECK(sizeof(Source) <= sizeof(Destination), Destination_is_smaller_than_source)
	return reinterpret_cast<Destination>(source);
}


//
// Partial Specialization
//

template<class Container, class Controller>
class Widget
{
public:
	void SomeMethod()
	{
		std::cout << "Widget Generic" << std::endl;
	}
};

template<>
class Widget<ContainerA, ControllerA>
{
public:
	void SomeMethod()
	{
		std::cout << "Widget <ContainerA, ControllerA> Specialization" << std::endl;
	}
};

template<class Container>
class Widget<Container, ControllerA>
{
public:
	void SomeMethod()
	{
		std::cout << "Widget <" << typeid(Container).name() << ", ControllerA> Partial Specialization" << std::endl;
	}
};

template<class ContainerArg>
class Widget<ContainerWithArg<ContainerArg>, ControllerA>
{
public:
	void SomeMethod()
	{
		std::cout << "Widget <" << typeid(ContainerWithArg<ContainerArg>).name()
				  << ", ControllerA> Partial Specialization" << std::endl;
	}
};

//
// Adapter Pattern
//

class IShootingInterface
{
public:
	virtual void Shoot() = 0;
};

template<class T, class P>
IShootingInterface* MakeShootingAdapter(const T& obj, const P& arg)
{
	class AdapterImpl : public IShootingInterface
	{
	public:
		AdapterImpl(const T& obj, const P& arg)
			: _obj(obj), _arg(arg) { }

		virtual void Shoot() override
		{
			_obj.Call(_arg);
		};
	private:
		T _obj;
		P _arg;
	};
	return new AdapterImpl(obj, arg);
}

class Soldier
{
public:
	void ShootRifle()
	{
		std::cout << "Shooting Rifle" << std::endl;
	}
};

class Tank
{
public:
	void ShootCannon()
	{
		std::cout << "Shooting Cannon" << std::endl;
	}
};

class SoldierShootAdapter
{
public:
	void Call(Soldier& soldier)
	{
		soldier.ShootRifle();
	}
};

class TankShootAdapter
{
public:
	void Call(Tank& tank)
	{
		tank.ShootCannon();
	}
};

//
// Switch Template
//

class MethodNotDefinedExcpetion : public std::exception
{
	virtual const char* what() const throw()
	{
		return "Method not defined!";
	}
};

template<int Method>
class MultiMethodSimple
{
private:
	void MethodImplementation0()
	{
		std::cout << "MethodImplementation0" << std::endl;
	}

	void MethodImplementation1()
	{
		std::cout << "MethodImplementation1" << std::endl;
	}
public:
	void DoMethod()
	{
		switch (Method)
		{
		case 0:
			MethodImplementation0();
			break;
		case 1:
			MethodImplementation1();
			break;
		default:
			throw "Method not defined!";
		}
	}
};

template<int V>
struct SwitchType
{
	enum { value = V };
};

template<int Method>
class MultiMethodClass
{
private:
	void MethodImplementation(SwitchType<0>)
	{
		std::cout << "MethodImplementation(SwitchType<0>)" << std::endl;
	}

	void MethodImplementation(SwitchType<1>)
	{
		std::cout << "MethodImplementation(SwitchType<1>)" << std::endl;
	}
public:
	void DoMethod()
	{
		MethodImplementation(SwitchType<Method>());
	}
};

template<typename T, int Method>
class MultiMethodClassStatic
{
private:
	static void MethodImplementation(T* Instance, SwitchType<0>)
	{

	}
	static void MethodImplementation(T* Instance, SwitchType<1>)
	{

	}
public:
	static void DoMethod(T* Instance)
	{
		MethodImplementation(Instance, SwitchType<Method>);
	}
};

//
// Type lists
//

class NullType {};
struct EmptyType {};

typedef AbstractFactory<TYPELIST_2<Soldier, Tank>> Factory;

Factory MyFactory = Factory();
Soldier* soldier = Factory->Create<Soldier>();
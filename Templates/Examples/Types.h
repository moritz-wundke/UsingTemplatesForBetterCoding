#pragma once
#include <iostream>
#include <cassert>

// Examples are based on the loki library by Andrei Alexandrescu

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

class BaseType
{

};

class Soldier : BaseType
{
public:
	void ShootRifle()
	{
		std::cout << "Shooting Rifle" << std::endl;
	}
};

class Tank : BaseType
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

template <typename T>
struct Type2Type
{
	typedef T OriginalType;
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

class NullType {};  // Defines our null values for types, used like a mark for the end of a type list

template <class T, class U = NullType>
struct Typelist
{
	typedef T Head;
	typedef U Tail;
};

// Typelist defining all available char types, note the NullType at the end?
typedef Typelist<char, Typelist<signed char, Typelist<unsigned char, NullType>>> CharTypelist;

#define TYPE_LIST_1(T1) Typelist<T1, NullType>
#define TYPE_LIST_2(T1, T2) Typelist<T1, TYPE_LIST_1(T2)>
#define TYPE_LIST_3(T1, T2, T3) Typelist<T1, TYPE_LIST_2(T2, T3)>

typedef TYPE_LIST_3(char, signed char, unsigned char) CharacterTypes;

// Find the length of a type list
template <class TList> struct Length;
template <> struct Length<NullType>
{
	enum { value = 0 };
};
template <class T, class U>
struct Length <Typelist<T, U>>
{
	enum { value = 1 + Length<U>::value };
};

// Get an element using an index
template <class TList, unsigned int index> struct TypeAt;

template <class Head, class Tail>
struct TypeAt<Typelist<Head, Tail>, 0>
{
	typedef Head result;
};

template <class Head, class Tail, unsigned int i>
struct TypeAt<Typelist<Head, Tail>, i>
{
	typedef typename TypeAt<Tail, i - 1>::result result;
};

// Append a type list

template <class TList, class T> struct Append;

template <> struct Append<NullType, NullType>
{
	typedef NullType result;
};

template <class T> struct Append<NullType, T>
{
	typedef TYPE_LIST_1(T) result;
};

template <class Head, class Tail>
struct Append<NullType, Typelist<Head, Tail> >
{
	typedef Typelist<Head, Tail> result;
};

template <class Head, class Tail, class T>
struct Append<Typelist<Head, Tail>, T>
{
	typedef Typelist<Head,
		typename Append<Tail, T>::result>
		result;
};

// Reverse a type list

template <class TList> struct Reverse;

template <class T>
struct Reverse< TYPE_LIST_1(T) >
{
	typedef TYPE_LIST_1(T) result;
};

template <class Head, class Tail>
struct Reverse< Typelist<Head, Tail> >
{
	typedef typename Append<
		typename Reverse<Tail>::result, Head>::result result;
};

//
// Simple abstract factory
//

template<class... Args>
struct BuildTypelist;

template <class Head>
struct BuildTypelist<Head>
{
	typedef Typelist<Head> result;
};

template <class Head, class... Args>
struct BuildTypelist<Head, Args...>
{
	typedef Typelist<Head, typename BuildTypelist<Args...>::result > result;
};

// Generate a scattered hierarchy

template <class TList, template <class> class Unit>
class GenScatterHierarchy;

template <class T1, class T2, template <class> class Unit>
class GenScatterHierarchy<Typelist<T1, T2>, Unit>
	: public GenScatterHierarchy<T1, Unit>
	, public GenScatterHierarchy<T2, Unit>
{
public:
	typedef Typelist<T1, T2> TList;
	typedef GenScatterHierarchy<T1, Unit> LeftBase;
	typedef GenScatterHierarchy<T2, Unit> RightBase;
	template <typename T> struct Rebind
	{
		typedef Unit<T> Result;
	};
};

template <class AtomicType, template <class> class Unit>
class GenScatterHierarchy : public Unit<AtomicType>
{
	typedef Unit<AtomicType> LeftBase;
	template <typename T> struct Rebind
	{
		typedef Unit<T> Result;
	};
};

template <template <class> class Unit>
class GenScatterHierarchy<NullType, Unit>
{
	template <typename T> struct Rebind
	{
		typedef Unit<T> Result;
	};
};

// Generate a linear hierarchy

template
<
	class TList,
	template <class AtomicType, class Base> class Unit,
	class Root = EmptyType
>
class GenLinearHierarchy;

template
<
	class T1,
	class T2,
	template <class, class> class Unit,
	class Root
>
class GenLinearHierarchy<Typelist<T1, T2>, Unit, Root>
	: public Unit< T1, GenLinearHierarchy<T2, Unit, Root> >
{
};

template
<
	class T,
	template <class, class> class Unit,
	class Root
>
class GenLinearHierarchy<Typelist<T, NullType>, Unit, Root>
	: public Unit<T, Root>
{
};


// The AbstractFactory

template <class T>
class AbstarctProducer
{
public:
	virtual T* DoCreate(Type2Type<T>) = 0;
	virtual ~AbstarctProducer() {}
};

template
<
	class TList,
	template <class> class Producer = AbstarctProducer
>
class AbstractFactory : public GenScatterHierarchy<TList, Producer>
{
public:
	typedef TList ClassList;

	template <class T> T* Create()
	{
		Producer<T>& unit = *this;
		return unit.DoCreate(Type2Type<T>());
	}
};

// A implementation of our interface

template <class NewClass, class Base>
class NewCreator : public Base
{
	typedef typename Base::ClassList BaseClassList;

protected:
	typedef typename BaseClassList::Tail ClassList;

public:
	typedef typename BaseClassList::Head AbstractClass;
	NewClass* DoCreate(Type2Type<AbstractClass>)
	{
		return new NewClass;
	}
};

template
<
	class AbstractFact,
	template <class, class> class Creator = NewCreator,
	class TList = typename AbstractFact::ClassList
>
class AbstractFactoryImpl
	: public GenLinearHierarchy<
		typename Reverse<TList>::result, Creator, AbstractFact>
{
public:
	typedef typename AbstractFact::ClassList ClassList;
	typedef TList ConcreteClassList;
};
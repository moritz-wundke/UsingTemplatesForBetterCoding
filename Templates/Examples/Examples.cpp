#include <iostream>
#include "Types.h"

// Examples
#define ENABLE_COMPILE_TIME_CHECKS 0
#define ENABLE_PARTIAL_SPECIALIZATION 1
#define ENABLE_ADAPTER_PATTERN 2
#define ENABLE_SWITCH_TYPE 3
#define ENABLE_TYPE_LISTS 4
#define ENABLE_ABSTRACT_FACTORY 5

// Set define to use the examples you wish
#define ENABLE_CODE ENABLE_ABSTRACT_FACTORY

int main()
{
#if ENABLE_CODE == ENABLE_COMPILE_TIME_CHECKS

	TypeA* a = new TypeA();
	char c = ReinterpCastChecked_Runtime<char>(a);
	char c2 = ReinterpCastChecked<char>(a);

	// Different platforms will have different sizes, we can test it in compile time now
	STATIC_CHECK(sizeof(uint8) == 1, uint8_wrong_size)

#elif ENABLE_CODE == ENABLE_PARTIAL_SPECIALIZATION

	auto Widget1 = Widget<BaseContainer, BaseController>();
	Widget1.SomeMethod();

	auto Widget2 = Widget<ContainerA, ControllerA>();
	Widget2.SomeMethod();

	auto Widget3 = Widget<int, ControllerA>();
	Widget3.SomeMethod();

	auto Widget4 = Widget<ContainerWithArg<int>, ControllerA>();
	Widget4.SomeMethod();

#elif ENABLE_CODE == ENABLE_ADAPTER_PATTERN

	Soldier soldier = Soldier();
	SoldierShootAdapter soldierAdapter = SoldierShootAdapter();

	Tank tank = Tank();
	TankShootAdapter tankAdapter = TankShootAdapter();

	// Now we create our interface adapters
	IShootingInterface* soldierShooter = MakeShootingAdapter(soldierAdapter, soldier);
	IShootingInterface* tankShooter = MakeShootingAdapter(tankAdapter, tank);

	soldierShooter->Shoot();
	tankShooter->Shoot();

#elif ENABLE_CODE == ENABLE_SWITCH_TYPE

	auto MultiInstanceSimple0 = MultiMethodSimple<0>();
	MultiInstanceSimple0.DoMethod();

	auto MultiInstanceSimple1 = MultiMethodSimple<1>();
	MultiInstanceSimple1.DoMethod();

	auto MultiInstanceSimple2 = MultiMethodSimple<2>();
	MultiInstanceSimple2.DoMethod();

	auto MultiInstance0 = MultiMethodClass<0>();
	MultiInstance0.DoMethod();

	auto MultiInstance1 = MultiMethodClass<1>();
	MultiInstance1.DoMethod();
#elif ENABLE_CODE == ENABLE_TYPE_LISTS

	std::cout << "Number of character types: " << Length<CharacterTypes>::value << std::endl;
	std::cout << "Type at index 1: " << typeid(TypeAt<CharacterTypes, 1>::result).name() << std::endl;

#elif ENABLE_CODE == ENABLE_ABSTRACT_FACTORY

	std::cout << "Number of Enemy types: " << Length<BuildTypelist<Soldier, Tank>::result>::value << std::endl;

	typedef AbstractFactory<BuildTypelist<Soldier, Tank>::result> EnemyFactory;
	AbstractFactoryImpl<EnemyFactory> factory = AbstractFactoryImpl<EnemyFactory>();
	auto soldier = factory.Create<Soldier>();

	std::cout << "We just built a: " << typeid(soldier).name() << std::endl;


#endif
	return 0;
}

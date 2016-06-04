#pragma once

//
// Creation Policies
//

template <class T>
struct NewCreator
{
	static T* CreateOrNull()
	{
		return new T;
	}
protected:
	~NewCreator() {}
};


struct NewCreatorFunc
{
	template <class T>
	static T* CreateOrNull()
	{
		return new T;
	}
protected:
	~NewCreatorFunc() {}
};


template <class T>
struct MallocCreator
{
	static T* CreateOrNull()
	{
		void* alloc = std::malloc(sizeof(T));
		return alloc ? return new(alloc) T : nullptr;
	}
protected:
	~MallocCreator() {}
};

template <class T>
struct CloneCreator
{
	CloneCreator(T* pObj = nullptr) : pBlueprint(pObj)
	{

	}

	T* CreateOrNull()
	{
		return pBlueprint ? pBlueprint->Clone() : nullptr;
	}

	T* GetClonable()
	{
		return pBlueprint;
	}
	void SetCloneable(T* pObj)
	{
		pBlueprint = pObj;
	}
protected:
	~CloneCreator() {}
private:
	T* pBlueprint;
};
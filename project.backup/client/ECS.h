#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <bitset>
#include <iterator>
#include <algorithm>



using ComponentID = std::size_t;
using Group = std::size_t;

constexpr int maxComp = 32;
constexpr std::size_t maxGrups = 32;

using GroupBitset = std::bitset<maxGrups>;

class Component;
class Entity;
class manager;

inline ComponentID getnewcompid()
{
	static ComponentID id = 0;
	return id++;
}
template <typename T> inline ComponentID getcompid() {
	static ComponentID compid = getnewcompid();
	return compid;
}

class Component {
public:
	Entity* ent;
	Component() { };
	~Component() {};
	virtual void init(){}
	virtual void Update() {}
	virtual void Draw(){}
};

class Entity {
private:
	manager& Manager;
	std::array<Component*, maxComp> componentArray;
	std::vector<char> bitArray = std::vector<char>(maxComp, 0);;
	std::vector<Component*> components;
	GroupBitset groupBitset;
	bool state = true;
public:
	Entity(manager& mManager) : Manager(mManager) { }
	bool isActive() { return state; }
	void Update()
	{
		for (auto& c : components) c->Update();
	}
	void Draw() {
		for (auto& c : components) c->Draw();
	}
	void Destroy() { state = false; }
	bool hasGroup(Group mGroup) {
		return groupBitset[mGroup];
	}
	void addGroup(Group mGroup);
	void delGroup(Group mGroup) 
	{
		groupBitset[mGroup] = false;
	}
	template <typename T>bool hasComponent() {
		if (bitArray[getcompid<T>()])
			return 1;
		else return 0;
	}
	template <typename T,typename... Targs> void addComponent(Targs&&... margs ) {
		T* c(new T(std::forward<Targs>(margs)...));
		c->ent = this;
		c->init();
		components.push_back(c);
		bitArray[getcompid<T>()] = 1;
		componentArray[getcompid<T>()]=c;
	}
	template <typename T> T& getComponent() {
		int x = getcompid<T>();
		auto ptr(componentArray[x]);
		return *static_cast<T*>(ptr);
	}

};

class manager {
private : 
	std::vector<Entity*> entities;
	std::array<std::vector<Entity*>, maxGrups> groupedEntities;
public:
	Entity* createEntity() {
		Entity* ent = new Entity(*this);
		entities.push_back(ent);
		return ent;
	}
	void Update(){
		for (auto& c : entities) c->Update();
	}
	void Draw() {
		for (auto& c : entities) c->Draw();
	}
	void refresh(){

		for (auto i(0u); i < maxGrups; i++)
		{
			auto& v(groupedEntities[i]);
			v.erase(
				std::remove_if(std::begin(v), std::end(v)
					, [i](Entity* mEntity)
			{
						return !mEntity->isActive() || !mEntity->hasGroup(i);
					}),
				std::end(v));
		}
		int i = 0;
		for (auto& c : entities)
		{
			if (!c->isActive())
			{	
				entities.erase(entities.begin()+i);
				if(c)
					delete c;
			}
			i++;
		}
	}
	
	void AddToGroup(Entity* mEntity, Group mGroup)
	{
		groupedEntities[mGroup].emplace_back(mEntity);
	}
	std::vector<Entity*>& getGroup(Group mGroup)
	{
		return groupedEntities[mGroup];
	}
};

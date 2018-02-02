#ifndef _ITEMS_H_
#define _ITEMS_H_

#include <string>
#include "living.h"

// ================ Base Class Item and Subclasses ================
class Item {
private:
	std::string name;
	unsigned int price;
	unsigned short minLevel;
	unsigned int weight;
public:
	Item(std::string, unsigned int, unsigned short, unsigned int);
	virtual ~Item() {};

	virtual void showInfo() const = 0;

	std::string getName() const				{ return name; }
	unsigned int getPrice() const			{ return price; }
	unsigned short getMinLevel() const		{ return minLevel; }
	unsigned int getWeight() const			{ return weight; }

	virtual bool isWeapon() const			{ return false; }
	virtual bool isArmor() const			{ return false; }
	virtual bool isPotion() const 			{ return false; }
	virtual bool isHealthPotion() const		{ return false; }
	virtual bool isManaPotion() const		{ return false; }
	virtual bool isStrengthPotion() const	{ return false; }
	virtual bool isDexterityPotion() const	{ return false; }
	virtual bool isAgilityPotion() const	{ return false; }
};

// ================ Class Weapon ================
class Weapon : public Item {
private:
	int damage;
	bool twoHanded;
public:
	Weapon(std::string, unsigned int, unsigned short, unsigned int, int, bool);

	void showInfo() const;

	int getDamage() const			{ return damage; }
	bool isTwoHanded() const		{ return twoHanded; }
	bool isWeapon() const			{ return true; }
};

// ================ Class Armor ================
class Armor : public Item {
private:
	int armorClass;
public:
	Armor(std::string, unsigned int, unsigned short, unsigned int, int);

	void showInfo() const;

	int getArmorClass() const		{ return armorClass; }
	bool isArmor()	const			{ return true; }
};

// ================ Class Potion and Subclasses ================
class Potion : public Item {
protected:
	int amount;
public:
	Potion(std::string, unsigned int, unsigned short, unsigned int, int);
	~Potion();

	int getAmount() const			{ return amount; }
	bool isPotion() const			{ return true; }

	virtual void setStat(Hero*) const = 0;
};

class HealthPotion : public Potion {
private:
public:
	HealthPotion(std::string, unsigned int, unsigned short, unsigned int, int);
	void showInfo() const;
	void setStat(Hero* hero) const	{ hero->setCurHealth(amount); }
	bool isHealthPotion() const		{ return true; }
};

class ManaPotion : public Potion {
private:
public:
	ManaPotion(std::string, unsigned int, unsigned short, unsigned int, int);
	void showInfo() const;
	void setStat(Hero* hero) const	{ hero->setCurMagicPower(amount); }
	bool isManaPotion()	const		{ return true; }
};

class StrengthPotion : public Potion {
private:
public:
	StrengthPotion(std::string, unsigned int, unsigned short, unsigned int, int);
	void showInfo() const;
	void setStat(Hero* hero) const	{ hero->setStrength(amount); }
	bool isStrengthPotion()	const	{ return true; }
};

class DexterityPotion : public Potion {
private:
public:
	DexterityPotion(std::string, unsigned int, unsigned short, unsigned int, int);
	void showInfo() const;
	void setStat(Hero* hero) const	{ hero->setDexterity(amount); }
	bool isDexterityPotion() const	{ return true; }
};

class AgilityPotion : public Potion {
private:
public:
	AgilityPotion(std::string, unsigned int, unsigned short, unsigned int, int);
	void showInfo() const;
	void setStat(Hero* hero) const	{ hero->setAgility(amount); }
	bool isAgilityPotion() const	{ return true; }
};

#endif /* _ITEMS_H_ */

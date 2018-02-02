#ifndef _SPELLS_H_
#define _SPELLS_H_

#include <string>
#include "living.h"

class Spell {
private:
	std::string name;
	unsigned int price;
	unsigned short minLevel;
	unsigned int minDamage;
	unsigned int maxDamage;
	unsigned int mana;
protected:
	unsigned int rounds;
public:
	Spell(std::string, unsigned int, unsigned short, unsigned int, unsigned int,
			unsigned int, unsigned int);

	std::string getName()			{ return name; }
	unsigned int getPrice() 		{ return price; }
	unsigned short getMinLevel()	{ return minLevel; }
	unsigned int getMinDamage()		{ return minDamage; }
	unsigned int getMaxDamage()		{ return maxDamage; }
	unsigned int getMana()			{ return mana; }

	virtual bool isIceSpell() const			{ return false; }
	virtual bool isFireSpell() const		{ return false; }
	virtual bool isLightingSpell() const	{ return false; }

	virtual void reduceStat(Monster&) const = 0;
	virtual void showInfo() const = 0;
};

class IceSpell : public Spell {
private:
	int modifier;
public:
	IceSpell(std::string, unsigned int, unsigned short, unsigned int, unsigned int,
			unsigned int, unsigned int, int);

	bool isIceSpell() const					{ return true; }
	void reduceStat(Monster&) const;
	void showInfo() const;
};

class FireSpell : public Spell {
private:
	int modifier;
public:
	FireSpell(std::string, unsigned int, unsigned short, unsigned int, unsigned int,
			unsigned int, unsigned int, int);

	bool isFireSpell() const				{ return true; }
	void reduceStat(Monster&) const;
	void showInfo() const;
};

class LightingSpell : public Spell {
private:
	int modifier;
public:
	LightingSpell(std::string, unsigned int, unsigned short, unsigned int, unsigned int,
			unsigned int, unsigned int, int);

	bool isLightingSpell() const			{ return true; }
	void reduceStat(Monster&) const;
	void showInfo() const;
};

#endif /* _SPELLS_H_ */

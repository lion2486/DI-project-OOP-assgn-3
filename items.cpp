#include <iostream>
#include "items.h"
using namespace std;

// ================ Class Item ================
Item::Item(string nam, unsigned int pr, unsigned short minL, unsigned int w)
	: name(nam), price(pr), minLevel(minL), weight(w)
{}

void Item::showInfo(void) const
{
	cout << "Name: " << name << '\n'
		 << "\tSell Price: " << price / 2 << " gold, "
		 << "Minimum level required: " << minLevel << endl;
}

// ================ Class Weapon ================
Weapon::Weapon(string nam, unsigned int pr, unsigned short minL, unsigned int w, int dam, bool bothH)
	: Item(nam, pr, minL, w), damage(dam), twoHanded(bothH)
{}

void Weapon::showInfo(void) const
{
	Item::showInfo();
	cout << "\tDamage: " << damage << ", "
		 << ((twoHanded == true) ? "One-handed" : "Two-handed") << endl;
}

// ================ Class Armor ================
Armor::Armor(string nam, unsigned int pr, unsigned short minL, unsigned int w, int armor)
	: Item(nam, pr, minL, w), armorClass(armor)
{}

void Armor::showInfo(void) const
{
	Item::showInfo();
	cout << "\tArmor Class: " << armorClass << endl;
}

// ================ Class Potion and Subclasses================
Potion::Potion(string nam, unsigned int pr, unsigned short minL, unsigned int w, int am)
	: Item(nam, pr, minL, w), amount(am)
{}

Potion::~Potion()
{
	cout << this->getName() << " was used." << endl;
}

// ================ Class HealthPotion ================
HealthPotion::HealthPotion(string nam, unsigned int pr, unsigned short minL, unsigned int w, int am)
	: Potion(nam, pr, minL, w, am)
{}

void HealthPotion::showInfo(void) const
{
	Potion::showInfo();
	cout << "\tHealth boost by " << amount << " points." << endl;
}

// ================ Class ManaPotion ================
ManaPotion::ManaPotion(string nam, unsigned int pr, unsigned short minL, unsigned int w, int am)
	: Potion(nam, pr, minL, w, am)
{}

void ManaPotion::showInfo(void) const
{
	Potion::showInfo();
	cout << "\tMagic Power boost by " << amount << " points." << endl;
}

// ================ Class StrengthPotion ================
StrengthPotion::StrengthPotion(string nam, unsigned int pr, unsigned short minL, unsigned int w, int am)
	: Potion(nam, pr, minL, w, am)
{}

void StrengthPotion::showInfo(void) const
{
	Potion::showInfo();
	cout << "\tStrength boost by " << amount << " points." << endl;
}

// ================ Class DexterityPotion ================
DexterityPotion::DexterityPotion(string nam, unsigned int pr, unsigned short minL, unsigned int w, int am)
	: Potion(nam, pr, minL, w, am)
{}

void DexterityPotion::showInfo(void) const
{
	Potion::showInfo();
	cout << "\tDexterity boost by " << amount << " points." << endl;
}

// ================ Class AgilityPotion ================
AgilityPotion::AgilityPotion(string nam, unsigned int pr, unsigned short minL, unsigned int w, int am)
	: Potion(nam, pr, minL, w, am)
{}

void AgilityPotion::showInfo(void) const
{
	Potion::showInfo();
	cout << "\tAgility boost by " << amount << " points." << endl;
}

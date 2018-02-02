#include <iostream>
#include "spells.h"
#include "living.h"
using namespace std;

// ================ Class Spell ================
Spell::Spell(string nam, unsigned int pr, unsigned short minL,
		unsigned int minD, unsigned int maxD, unsigned int m, unsigned int r)
	: name(nam), price(pr), minLevel(minL), minDamage(minD), maxDamage(maxD),
	  mana(m), rounds(r)
{}

// Show Spell's Info
void Spell::showInfo(void) const
{
	cout << "Name: " << name << "\n"
		 << "\tPrice: " << price << " gold, "
		 << "Required level: " << minLevel << "\n"
		 << "\tDamage ratio: " << minDamage << "~" << maxDamage << ", "
		 << "Mana cost: " << mana << ", "
		 << "Rounds in effect: " << rounds << endl;
}

// ================ Class IceSpell ================
IceSpell::IceSpell(string nam, unsigned int pr, unsigned short minL,
		unsigned int minD, unsigned int maxD, unsigned int m, unsigned int r,
		int modif)
	: Spell(nam, pr, minL, minD, maxD, m, r), modifier(modif)
{}

// Reduce monster's damage ratio
void IceSpell::reduceStat(Monster& monster) const
{
	unsigned int minDamage = monster.getCurMinDamage();
	unsigned int maxDamage = monster.getCurMaxDamage();

	if (maxDamage - modifier >= minDamage - modifier / 3) {
		monster.setCurMinDamage( - modifier / 3);
		monster.setCurMaxDamage( - modifier);
	}
	else {
		monster.setCurMinDamage( - modifier / 3);
		monster.setCurMaxDamage( - (maxDamage - minDamage));
	}
	cout << monster.getName() << "'s damage ratio was reduced to "
		 << monster.getCurMinDamage() << "~" << monster.getCurMaxDamage() << "." << endl;
	monster.setDebuff(new DamageDebuff(rounds, modifier, modifier / 3));
}

void IceSpell::showInfo(void) const
{
	Spell::showInfo();
	cout << "\tReduces damage ratio by " << modifier << " (min damage) and "
		 << modifier / 3 << " (max damage) points." << endl;
}

// ================ Class FireSpell ================
FireSpell::FireSpell(string nam, unsigned int pr, unsigned short minL,
		unsigned int minD, unsigned int maxD, unsigned int m, unsigned int r,
		int modif)
	: Spell(nam, pr, minL, minD, maxD, m, r), modifier(modif)
{}

// Reduce monster's defence
void FireSpell::reduceStat(Monster& monster) const
{
	monster.setCurDefence( - modifier);
	cout << monster.getName() << "'s defence was reduced by "
		 << modifier << " points." << endl;
	monster.setDebuff(new DefenceDebuff(rounds, modifier));
}

void FireSpell::showInfo(void) const
{
	Spell::showInfo();
	cout << "\tReduces defence by " << modifier << " points." << endl;
}

// ================ Class LightingSpell ================
LightingSpell::LightingSpell(string nam, unsigned int pr, unsigned short minL,
		unsigned int minD, unsigned int maxD, unsigned int m, unsigned int r,
		int modif)
	: Spell(nam, pr, minL, minD, maxD, m, r), modifier(modif)
{}

// Reduce monster's dodge chance
void LightingSpell::reduceStat(Monster& monster) const
{
	monster.setCurDodgeChance( - modifier);
	cout << monster.getName() << "'s dodge chance was reduced by "
		 << modifier << " points." << endl;
	monster.setDebuff(new DodgeChDebuff(rounds, modifier));
}

void LightingSpell::showInfo(void) const
{
	Spell::showInfo();
	cout << "\tReduces enemy's dodge chance by " << modifier << " points." << endl;
}

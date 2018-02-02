/*
 * living.cpp
 */

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <iomanip>
#include "living.h"
#include "items.h"
#include "spells.h"
using namespace std;

// ================ Parent Class Living ================
Living::Living(string nam, unsigned short lev, int health, int maxHealth)
	: name(nam), level(lev), curHealthP(health), healthPower(maxHealth)
{}

void Living::displayStats(void) const
{
	cout << "======== Stats ========" << '\n'
		 << setw(15) << left << "Name" << ":" << name << "\n"
		 << setw(15) << left << "Level" << ":" << level << "\n"
		 << setw(15) << left << "Health" << ":"
		 << curHealthP << "/" << healthPower << endl;
}

void Living::setCurHealth(int modifier)
{
	if (curHealthP + modifier > healthPower) {
		if (curHealthP == healthPower) return;
		cout << name << "'s health was replenished by "
			 << healthPower - curHealthP << " points." << endl;
		curHealthP = healthPower;
	}
	else if (curHealthP + modifier < 0) {
		cout << name << "'s health was reduced by " << curHealthP << " points." << endl;
		curHealthP = 0;
	}
	else {
		curHealthP += modifier;
		cout << name << "'s health was " << ((modifier > 0) ? "replenished" : "reduced")
			 << " by " << modifier << " points" << endl;
	}
}

// ================ Classes members of Hero ================
Equipment::Equipment(Weapon* lW = NULL, Weapon* rW = NULL, Armor* ar = NULL)
	: leftWeapon(lW), rightWeapon(rW), armor(ar)
{}

Inventory::Inventory(unsigned int mon, unsigned int weight)
	: money(mon), carriedWeight(weight)
{}

// Insert a weapon in the inventory
void Inventory::insertItem(Weapon* weapon)
{
	string name = weapon->getName();
	vector<Weapon*>::iterator it, itEnd;
	if (weapons.empty())
		weapons.push_back(weapon);
	else
		for (it = weapons.begin(), itEnd = weapons.end(); it < itEnd; it++)
			if (name <= (*it)->getName())
				weapons.insert(it, weapon);
}

// Insert armor in the inventory
void Inventory::insertItem(Armor* arm)
{
	string name = arm->getName();
	vector<Armor*>::iterator it, itEnd;
	if (armor.empty())
		armor.push_back(arm);
	else
		for (it = armor.begin(), itEnd = armor.end(); it < itEnd; it++)
			if (name <= (*it)->getName())
				armor.insert(it, arm);
}

// Insert a potion in the inventory
void Inventory::insertItem(Potion* potion)
{
	string name = potion->getName();
	vector<Potion*>::iterator it, itEnd;
	if (potions.empty())
		potions.push_back(potion);
	else
		for (it = potions.begin(), itEnd = potions.end(); it < itEnd; it++)
			if (name <= (*it)->getName())
				potions.insert(it, potion);
}

// Remove a weapon from the inventory
void Inventory::removeWeapon(int num)
{
	// unequip if equipped
	if (weapons[num] == equipment.getLeftWeapon())
		equipment.setLeftWeapon(NULL);
	else if (weapons[num] == equipment.getRightWeapon())
		equipment.setRightWeapon(NULL);
	money += weapons[num]->getPrice() / 2;
	carriedWeight -= weapons[num]->getWeight();
	delete weapons[num];
	weapons[num] = NULL;
	weapons.erase(weapons.begin() + num);
}

// Remove armor from the inventory
void Inventory::removeArmor(int num)
{
	// unequip if equipped
	if (armor[num] == equipment.getArmor())
		equipment.setArmor(NULL);
	money += armor[num]->getPrice() / 2;
	carriedWeight -= armor[num]->getWeight();
	delete armor[num];
	armor[num] = NULL;
	armor.erase(armor.begin() + num);
}

// Remove a potion from the inventory
void Inventory::removePotion(int num)
{
	money += potions[num]->getPrice() / 2;
	carriedWeight -= potions[num]->getWeight();
	delete potions[num];
	potions.erase(potions.begin() + num);
}

/*
 * function 0 -> Print all the items; Weapons first, Armor second, Potions last
 * function 1 -> Print Weapons and Armor
 * function 2 -> Print only Potions
 */
int Inventory::checkInventory(int function)
{
	vector<Weapon*>::iterator itW, itWEnd;
	vector<Armor*>::iterator itA, itAEnd;
	vector<Potion*>::iterator itP, itPEnd;
	unsigned int counter = 1;

	cout << "\nGold: " << money << '\n'
		 << "Carried Weight: " << carriedWeight << endl;
	if (function == 0)
		cout << "#####################################" << '\n'
			 << "############  Inventory  ############" << '\n'
			 << "#####################################" << endl;
	if (weapons.empty() && armor.empty() && potions.empty()) {
		cout << "You have nothing in your inventory." << endl;
		return -1;
	}
	if (function == 0 || function == 1) {
		if (function == 1 && weapons.empty() && armor.empty()) {
			cout << "You have no weapons and armor." << endl;
			return -1;
		}
		if (!weapons.empty())
			cout << "======== Weapons ========" << endl;
		for (itW = weapons.begin(), itWEnd = weapons.end(); itW < itWEnd; itW++) {
			cout << counter++ << ". ";
			(*itW)->showInfo();
		}
		if (!armor.empty())
			cout << "========= Armor =========" << endl;
		for (itA = armor.begin(), itAEnd = armor.end(); itA < itAEnd; itA++) {
			cout << counter++ << ". ";
			(*itA)->showInfo();
		}
	}
	if (function == 0 || function == 2) {
		if (function == 2 && potions.empty()) {
			cout << "You don't have any potions." << endl;
			return -1;
		}
		cout << "======== Potions ========" << endl;
		for (itP = potions.begin(), itPEnd = potions.end(); itP < itPEnd; itP++) {
			cout << counter++ << ". ";
			(*itP)->showInfo();
		}
	}
	cout << endl;
	return counter;
}


// ================ Class Hero and Subclasses ================
Hero::Hero(string nam, unsigned short lev, int hp,  int maxHP,
		int mp, int curMP, int stren, int dext, int agil,
		unsigned int maxW, unsigned int mon, unsigned int exp)
	: Living(nam, lev, maxHP, hp),
	  magicPower(mp), curMagicPower(mp), strength(stren), dexterity(dext), agility(agil),
	  maxWeight(maxW), experience(exp), inventory(mon, 0)
{}

/*
 * Mutators for class Hero
 */
void Hero::setCurMagicPower(int amount)
{
	if (curMagicPower + amount > magicPower) {
		if (curMagicPower == magicPower) return;
		cout << name << "'s magic power was replenished by "
			 << magicPower - curMagicPower << " points." << endl;
		curMagicPower = magicPower;
	}
	else if (curMagicPower + amount < 0) {
		cout << name << "'s magic power was reduced by "
			 << curMagicPower << " points." << endl;
		curMagicPower = 0;
	}
	else {
		curMagicPower += magicPower;
		cout << name << "'s magic power was "
			 << ((amount > 0) ? "replenished" : "reduced")
			 << " by " << amount << " points" << endl;
	}
}

void Hero::setStrength(int amount)
{
	strength += amount;
	cout << "Strength was increased by " << amount << " points." << endl;
}

void Hero::setDexterity(int amount)
{
	dexterity += amount;
	cout << "Dexterity was increased by " << amount << " points." << endl;
}

void Hero::setAgility(int amount)
{
	agility += amount;
	cout << "Agility was increased by " << amount << " points." << endl;
}

void Hero::setExperience(unsigned int amount)
{
	experience += amount;
	while (experience >= (unsigned) level * level * 500)
		this->levelUp();
}

//Raise Hero's stats when leveled up
void Hero::levelUp(void)
{
	cout << "~~~~~~~~~~ Level Up! ~~~~~~~~~~" << endl;
	level++;
	healthPower += 100;
	curHealthP += 100;
}

// Buy an item and place it in the inventory
void Hero::buy(Item* item)
{
	int price = item->getPrice();
	unsigned int weight = item->getWeight();

	if (inventory.getMoney() >= price)
		if (inventory.getCarriedWeight() + weight <= maxWeight) {
			inventory.setMoney( - price);
			inventory.setCarriedWeight(weight);
			if (item->isWeapon())
				inventory.insertItem(new Weapon(*dynamic_cast<Weapon*>(item)));
			else if (item->isArmor())
				inventory.insertItem(new Armor(*dynamic_cast<Armor*>(item)));
			else {
				if (item->isHealthPotion())
					inventory.insertItem(new HealthPotion(*dynamic_cast<HealthPotion*>(item)));
				else if (item->isManaPotion())
					inventory.insertItem(new ManaPotion(*dynamic_cast<ManaPotion*>(item)));
				else if (item->isStrengthPotion())
					inventory.insertItem(new StrengthPotion(*dynamic_cast<StrengthPotion*>(item)));
				else if (item->isDexterityPotion())
					inventory.insertItem(new DexterityPotion(*dynamic_cast<DexterityPotion*>(item)));
				else
					inventory.insertItem(new AgilityPotion(*dynamic_cast<AgilityPotion*>(item)));
			}
			cout << "A(n) " << item->getName() << " was bought." << endl;
		}
		else
			cout << "You can't carry any more items!" << endl;
	else
		cout << "You don't have enough money for this item!" << endl;
}

// Buy a spell and place it in the spellbook
void Hero::buy(Spell* spell)
{
	int price = spell->getPrice();

	if (inventory.getMoney() >= price) {
		inventory.setMoney( - price);
		if (spell->isIceSpell())
			spellbook.push_back(new IceSpell(*dynamic_cast<IceSpell*>(spell)));
		else if (spell->isFireSpell())
			spellbook.push_back(new FireSpell(*dynamic_cast<FireSpell*>(spell)));
		else
			spellbook.push_back(new LightingSpell(*dynamic_cast<LightingSpell*>(spell)));
		cout << "A(n) " << spell->getName() << " (spell) was bought." << endl;
	}
	else
		cout << "You don't have enough money for this spell!" << endl;
}

// Sell an item in the inventory or a spell in the spellbook
void Hero::sell(int num)
{
	int weaponNum = inventory.getWeaponNum();
	int armorNum = inventory.getArmorNum();
	int potionNum = inventory.getPotionNum();

	// in case of error
	if (num < 1 || num > weaponNum + armorNum + potionNum + (int) spellbook.size())
		cout << "There is no item or spell with number " << num << "!" << endl;
	// if it is a weapon
	else if (num <= weaponNum)
		inventory.removeWeapon(num - 1);
	// if it is armor
	else if (num <= weaponNum + armorNum)
		inventory.removeArmor(num - weaponNum - 1);
	// if it is a potion
	else if (num <= weaponNum + armorNum + potionNum)
		inventory.removePotion(num - (weaponNum + armorNum) - 1);
	// if it is a spell
	else {
		int pos = num - (weaponNum + armorNum + potionNum) - 1;
		setMoney(spellbook[pos]->getPrice() / 2);
		delete spellbook[pos];
		spellbook[pos] = NULL;
		spellbook.erase(spellbook.begin() + pos);
	}
}

/*
 * Check the items in the inventory
 * function 0 -> all
 * function 1 -> weapons and armor
 * function 2 -> potions only
 */
int Hero::checkInventory(int function)
{
	return inventory.checkInventory(function);
}

// Show info of all the spells in the spellbook
bool Hero::showSpellbook(int counter)
{
	vector<Spell*>::iterator it, itEnd;

	cout << "#####################################" << '\n'
		 << "############  Spellbook  ############" << '\n'
		 << "#####################################" << endl;
	if (spellbook.empty()) {
		cout << "You don't have any spells in your spellbook." << '\n' << endl;
		return false;
	}
	for (it = spellbook.begin(), itEnd = spellbook.end(); it < itEnd; it++) {
		cout << counter++ << ". ";
		(*it)->showInfo();
	}
	cout << '\n' << endl;
	return true;
}

/*
 * Equip a weapon: 0 -> left hand, 1 -> right hand (input by user)
 */
bool Hero::equip(int num)
{
	int weaponNum = inventory.getWeaponNum();
	int armorNum = inventory.getArmorNum();
	int potionNum = inventory.getPotionNum();
	Weapon *leftWeapon, *rightWeapon;

	// in case of error
	if (num < 1 || num > weaponNum + armorNum + potionNum) {
		cout << "There is no item with number " << num << "!" << endl;
		return false;
	}
	// if it is a weapon
	if (num <= weaponNum) {
		Weapon* weapon = inventory.getWeapon(num - 1);
		// if the hero does not meet the required level
		if (weapon->getMinLevel() > this->level) {
			cout << "You cannot use this item, yet!" << endl;
			return false;
		}
		// in case of a two-handed weapon
		if (weapon->isTwoHanded()) {
			// "unequip" the left hand if not free
			if ((leftWeapon = inventory.getLeftWeapon()) != NULL)
				cout << leftWeapon->getName() << " was unequipped." << endl;
			// "unequip" the right hand if not free
			if ((rightWeapon = inventory.getRightWeapon()) != NULL)
				cout << rightWeapon->getName() << " was unequipped." << endl;
			// equip the weapon
			inventory.setLeftWeapon(weapon);
			inventory.setRightWeapon(NULL);
		}
		// in case of one-handed weapon
		else {
			int hand;
			string line;
			cout << "Choose the hand you want to equip (1 -> left, 2 -> right)." << endl;
			cin >> hand;
			while ((hand != 1 && hand != 2) || cin.fail()) {
				cout << "Wrong option given! Please choose again." << endl;
				cin.clear();
				cin >> line >> hand;
			}
			if (hand == 2) {
				// "unequip" the right hand if not free
				if ((rightWeapon = inventory.getRightWeapon()) != NULL)
					cout << rightWeapon->getName() << " was unequipped." << endl;
				inventory.setRightWeapon(weapon);
			}
			else {
				// "unequip" the left hand if not free
				if ((leftWeapon = inventory.getLeftWeapon()) != NULL)
					cout << leftWeapon->getName() << " was unequipped." << endl;
				inventory.setLeftWeapon(weapon);
			}
		}
		cout << "A(n)" << weapon->getName() << " was equipped." << endl;
		return true;
	}
	// if it is armor
	if (num <= weaponNum + armorNum) {
		Armor* armor = inventory.getArmor(num - weaponNum - 1);
		// if the hero does not meet the required level
		if (armor->getMinLevel() > this->level) {
			cout << "You cannot use this item, yet!" << endl;
			return false;
		}
		if (inventory.getArmor() != NULL)
			cout << inventory.getArmor()->getName() << " was unequipped." << endl;
		inventory.setArmor(armor);
		cout << armor->getName() << " was equipped." << endl;
		return true;
	}
	// if it is a potion
	cout << "You cannot equip potions!" << endl;
	return false;
}


// Use a potion
bool Hero::use(int num)
{
	int potionNum = inventory.getPotionNum();

	if (num < 1 || num > potionNum) {
		cout << "There is no potion with number " << num << " !" << endl;
		return false;
	}
	inventory.getPotion(num - 1)->setStat(this);
	inventory.removePotion(num - 1);
	return true;
}

// Attack an monster
void Hero::attack(Monster& monster)
{
	int damage = 0;
	Weapon* weapon;

	cout << this->getName() << " attacks monster " << monster.getName() << "..." << '\n';
	if ((weapon = inventory.getLeftWeapon()) != NULL)
		damage += weapon->getDamage();
	if ((weapon = inventory.getRightWeapon()) != NULL)
		damage += weapon->getDamage();
	// if monster did NOT dodge the attack
	if (rand() % 101 > monster.getCurDodgeChance()) {
		if (monster.getCurDefence() - (this->strength + damage) < 0) {
			monster.setCurHealth(monster.getCurDefence() - (this->strength + damage));
			cout << "Monster " << monster.getName() << " lost "
				 << this->strength + damage << " life points." << endl;
		}
		else
			cout << "Monster " << monster.getName() << " lost 0 life points." << endl;
	}
	else
		cout << "Hero " << this->getName() << " missed!" << endl;
}

// Cast a spell on an enemy and return the number of rounds in effect if successful
bool Hero::castSpell(int num, Monster& monster)
{
	if (num < (int) spellbook.size()) {
		cout << "There is no spell with number " << num << "!" << endl;
		return false;
	}
	// if the hero does not meet the required level
	if (spellbook[num - 1]->getMinLevel() > this->level) {
		cout << "You cannot use this spell, yet!" << endl;
		return false;
	}
	// if there is not enough mana
	if ((signed) spellbook[num - 1]->getMana() > curMagicPower) {
		cout << "You don't have enough mana for this spell!" << endl;
		return false;
	}
	monster.setCurHealth( - (int) (spellbook[num - 1]->getMinDamage() +
		((spellbook[num - 1]->getMaxDamage() - spellbook[num - 1]->getMinDamage()) * (double) dexterity/100 )));
	// apply the spell's side effect
	spellbook[num - 1]->reduceStat(monster);
	// reduce magic power
	curMagicPower -= spellbook[num - 1]->getMana();
	return true;
}

// Display the Hero statistics
void Hero::displayStats(void) const
{
	Living::displayStats();
	cout << setw(15) << left << "Magic Power"	<< ":"
			<< curMagicPower << "/" << magicPower << '\n'
		 << setw(15) << left << "Strength"		<< ":" << strength << '\n'
		 << setw(15) << left << "Dexterity"		<< ":" << dexterity << '\n'
		 << setw(15) << left << "Agility"		<< ":" << agility << '\n'
		 << setw(15) << left << "Max Weight"	<< ":" << maxWeight << '\n'
		 << setw(15) << left << "Experience"	<< ":" << experience << '\n' << endl;
}

// ================ Class Warrior ================
Warrior::Warrior(string nam, unsigned short lev, int hp, int maxHP,
		int maxMP, int mp, int stren, int dext, int agil,
		unsigned int maxW, unsigned int mon, unsigned int exp)
	: Hero(nam, lev, maxHP, hp, maxMP, mp, stren, dext, agil, maxW, mon, exp)
{
	cout << "A warrior joined the party." << endl;
}

void Warrior::levelUp(void)
{
	Hero::levelUp();
	magicPower += 15;
	curMagicPower += 15;
	strength += 5;
	dexterity += 2;
	agility += 5;
	maxWeight += 400 * level;
}

// ================ Class Sorcerer ================
Sorcerer::Sorcerer(string nam, unsigned short lev, int hp, int maxHP,
		int maxMP, int mp, int stren, int dext, int agil,
		unsigned int maxW, unsigned int mon, unsigned int exp)
	: Hero(nam, lev, maxHP, hp, maxMP, mp, stren, dext, agil, maxW, mon, exp)
{
	cout << "A sorcerer joined the party." << endl;
}

void Sorcerer::levelUp(void)
{
	Hero::levelUp();
	magicPower += 50;
	curMagicPower += 50;
	strength += 2;
	dexterity += 5;
	agility += 5;
	maxWeight += 100 * level;
}

// ================ Class Paladin ================
Paladin::Paladin(string nam, unsigned short lev, int hp, int maxHP,
		int maxMP, int mp, int stren, int dext, int agil,
		unsigned int maxW, unsigned int mon, unsigned int exp)
	: Hero(nam, lev, maxHP, hp, maxMP, mp, stren, dext, agil, maxW, mon, exp)
{
	cout << "A paladin joined the party." << endl;
}

void Paladin::levelUp(void)
{
	Hero::levelUp();
	magicPower += 30;
	curMagicPower += 30;
	strength += 5;
	dexterity += 5;
	agility += 2;
	maxWeight += 200 * level;
}

// ================ Class Debuff and Subclasses ================
Debuff::Debuff(int r, int modif)
	: rounds(r), modifier(modif)
{}

DamageDebuff::DamageDebuff(int rounds, int modifier, int damageModif)
	: Debuff(rounds, modifier), minDamageMod(damageModif)
{}

void DamageDebuff::removeDebuff(Monster* monster)
{
	monster->setCurMaxDamage(modifier);
	monster->setCurMinDamage(minDamageMod);
}

DefenceDebuff::DefenceDebuff(int rounds, int modifier)
	: Debuff(rounds, modifier)
{}

void DefenceDebuff::removeDebuff(Monster* monster)
{
	monster->setCurDefence(modifier);
}

DodgeChDebuff::DodgeChDebuff(int rounds, int modifier)
	: Debuff(rounds, modifier)
{}

void DodgeChDebuff::removeDebuff(Monster* monster)
{
	monster->setCurDodgeChance(modifier);
}


// ================ Class Monster ================
Monster::Monster(string nam, unsigned short lev, int health, int maxHealth,
		unsigned int minD, unsigned int maxD, int def, int dodge)
	: Living(nam, lev, health, maxHealth), minDamage(minD), curMinDamage(minD),
	  maxDamage(maxD), curMaxDamage(maxD), defence(def), curDefence(def),
	  dodgeChance(dodge), curDodgeChance(dodge)
{}

// just to avoid undefined references
Monster::~Monster() {}

// Display all the statistics of a monster
void Monster::displayStats(void) const
{
	Living::displayStats();
	cout << setw(15) << left << "Damage ratio" << ":" << minDamage << "~" << maxDamage << '\n';
	if (curMaxDamage != maxDamage)
		cout << "Current Max Damage: " << curMaxDamage << '\n';
	cout << setw(15) << left << "Defence" << ":" << defence << '\n';
	if (curDefence != defence)
		cout << "Current Defence: " << curDefence << '\n';
	cout << setw(15) << left << "Dodge Chance" << ":" << dodgeChance << '\n';
	if (curDodgeChance != dodgeChance)
		cout << "Current Dodge Chance: " << curDodgeChance << '\n';
	cout << endl;
}

// Attack a hero
void Monster::attack(Hero& hero)
{
	int damage = 0, armorClass = 0;

	cout << this->getName() << " attacks hero " << hero.getName() << "..." << '\n';
	// if hero did NOT dodge the attack
	if (rand() % 101 > hero.getAgility()) {
		damage = this->getCurMinDamage();
		damage += (int) ((this->getCurMaxDamage() - damage) * (double) (rand() % 101) / 100);
		if (hero.getArmor() != NULL)
			armorClass = hero.getArmor()->getArmorClass();
		if (armorClass - damage < 0) {
			hero.setCurHealth(armorClass - damage);
			cout << "Hero " << hero.getName() << " lost " << damage << " life points." << endl;
		}
		else
			cout << "Hero " << hero.getName() << " lost 0 life points." << endl;
	}
	else
		cout << "Monster " << this->getName() << " missed!" << endl;
}

// Check if a debuff is still in effect, else remove it
void Monster::checkDebuff(void)
{
	for (unsigned int i = 0; i < debuffs.size(); i++) {
		debuffs[i]->reduceRounds();
		if (debuffs[i]->fadesOut()) {
			debuffs[i]->removeDebuff(this);
			delete debuffs[i];
			debuffs.erase(debuffs.begin() + i);
		}
	}
}

// ================ Class Dragon ================
Dragon::Dragon(string nam, unsigned short lev, int health, int maxHealth,
		unsigned int minD, unsigned int maxD, int def, int dodge)
	: Monster(nam, lev, health, maxHealth, minD, maxD, def, dodge)
{}

Dragon::~Dragon()
{

}

// ================ Class Exoskeleton ================
Exoskeleton::Exoskeleton(string nam, unsigned short lev, int health, int maxHealth,
		unsigned int minD, unsigned int maxD, int def, int dodge)
	: Monster(nam, lev, health, maxHealth, minD, maxD, def, dodge)
{}

Exoskeleton::~Exoskeleton()
{

}

// ================ Class Spirit ================
Spirit::Spirit(string nam, unsigned short lev, int health, int maxHealth,
		unsigned int minD, unsigned int maxD, int def, int dodge)
	: Monster(nam, lev, health, maxHealth, minD, maxD, def, dodge)
{}

Spirit::~Spirit()
{

}

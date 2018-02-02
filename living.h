#ifndef _LIVING_H_
#define _LIVING_H_

#include <string>
#include <vector>

// ================ Base Class Living ================
class Living {
protected:
	const std::string name;
	unsigned short level;
	int curHealthP;
	int healthPower;
public:
	Living(std::string, unsigned short, int, int);
	virtual void displayStats() const = 0;

	std::string getName()			{ return name; }
	unsigned short getLevel()		{ return level; }
	int getHealthP()				{ return healthPower; }
	int getCurHealthP()				{ return curHealthP; }

	void setCurHealth(int);
};

// ====== Class forwarding ======
class Item;
class Weapon;
class Armor;
class Potion;
class HealthPotion;
class ManaPotion;
class StrengthPotion;
class DexterityPotion;
class AgilityPotion;
class Spell;
class Monster;

// ================ Class Hero and Subclasses ================
class Equipment {
private:
	Weapon* leftWeapon;
	Weapon* rightWeapon;
	Armor* armor;
public:
	Equipment(Weapon*, Weapon*, Armor*);

	Weapon* getLeftWeapon()			{ return leftWeapon; }
	Weapon* getRightWeapon()		{ return rightWeapon; }
	Armor* getArmor()				{ return armor; }

	void setLeftWeapon(Weapon* lW)	{ leftWeapon = lW; }
	void setRightWeapon(Weapon* rW)	{ rightWeapon = rW; }
	void setArmor(Armor* ar)		{ armor = ar; }
};

class Inventory {
private:
	Equipment equipment;
	std::vector<Weapon*> weapons;
	std::vector<Armor*> armor;
	std::vector<Potion*> potions;

	int money;
	unsigned int carriedWeight;
public:
	Inventory(unsigned int, unsigned int);

	Weapon* getLeftWeapon()				{ return equipment.getLeftWeapon(); }
	Weapon* getRightWeapon()			{ return equipment.getRightWeapon(); }
	Armor* getArmor()					{ return equipment.getArmor(); }
	Weapon* getWeapon(int num)			{ return weapons[num]; }
	Armor* getArmor(int num)			{ return armor[num]; }
	Potion* getPotion(int num)			{ return potions[num]; }
	int getWeaponNum()					{ return weapons.size(); }
	int getArmorNum()					{ return armor.size(); }
	int getPotionNum()					{ return potions.size(); }
	int getMoney()						{ return money; }
	unsigned int getCarriedWeight()		{ return carriedWeight; }

	void setLeftWeapon(Weapon* weapon)	{ equipment.setLeftWeapon(weapon); }
	void setRightWeapon(Weapon* weapon)	{ equipment.setRightWeapon(weapon); }
	void setArmor(Armor* arm)			{ equipment.setArmor(arm); }
	void setMoney(int amount)			{ money += amount; }
	void setCarriedWeight(int amount)	{ carriedWeight += amount; }
	void insertItem(Weapon*);
	void insertItem(Armor*);
	void insertItem(Potion*);
	void removeWeapon(int);
	void removeArmor(int);
	void removePotion(int);

	int checkInventory(int);
};

class Hero : public Living {
protected:
	int magicPower;
	int curMagicPower;
	int strength;
	int dexterity;
	int agility;

	unsigned int maxWeight;
private:
	unsigned int experience;

	Inventory inventory;
	std::vector<Spell*> spellbook;
public:
	Hero(std::string, unsigned short, int, int,
			int, int, int, int, int,
			unsigned int, unsigned int, unsigned int);

	unsigned int getCurMagicPower() const	{ return curMagicPower; }
	int getAgility() const					{ return agility; }
	unsigned int getExperience() const		{ return experience; }
	int getMoney()							{ return inventory.getMoney(); }
	Weapon* getLeftWeapon()					{ return inventory.getLeftWeapon(); }
	Weapon* getRightWeapon()				{ return inventory.getRightWeapon(); }
	Armor* getArmor()						{ return inventory.getArmor(); }

	void setCurMagicPower(int);
	void setStrength(int);
	void setDexterity(int);
	void setAgility(int);
	void setExperience(unsigned int);
	void setLeftWeapon(Weapon* weapon)		{ inventory.setLeftWeapon(weapon); }
	void setRightWeapon(Weapon* weapon)		{ inventory.setRightWeapon(weapon); }
	void setArmor(Armor* armor)				{ inventory.setArmor(armor); }
	void setMoney(int amount)				{ inventory.setMoney(amount); }

	virtual void levelUp() = 0;

	void buy(Item*);
	void buy(Spell*);
	void sell(int);
	int checkInventory(int = 0);
	bool showSpellbook(int = 1);
	bool equip(int);
	bool use(int);
	void attack(Monster&);
	bool castSpell(int, Monster&);
	virtual void displayStats() const;
};

class Warrior : public Hero {
public:
	Warrior(std::string, unsigned short, int, int,
			int, int, int, int, int,
			unsigned int, unsigned int, unsigned int);

	virtual void levelUp();
};

class Sorcerer : public Hero {
public:
	Sorcerer(std::string, unsigned short, int, int,
			int, int, int, int, int,
			unsigned int, unsigned int, unsigned int);

	virtual void levelUp();
};

class Paladin : public Hero {
public:
	Paladin(std::string, unsigned short, int, int,
			int, int, int, int, int,
			unsigned int, unsigned int, unsigned int);

	virtual void levelUp();
};

// ================ Class Monster and Subclasses ================
class Debuff {
private:
	int rounds;
protected:
	int modifier;
public:
	Debuff(int, int);
	bool fadesOut()	const			{ return rounds == 0; }
	void reduceRounds()				{ rounds--; }
	virtual void removeDebuff(Monster*) = 0;
};

class DamageDebuff : public Debuff {
private:
	int minDamageMod;
public:
	DamageDebuff(int, int, int);
	void removeDebuff(Monster*);
};

class DefenceDebuff : public Debuff {
public:
	DefenceDebuff(int, int);
	void removeDebuff(Monster*);
};

class DodgeChDebuff : public Debuff {
public:
	DodgeChDebuff(int, int);
	void removeDebuff(Monster*);
};

class Monster : public Living {
private:
	unsigned int minDamage;
	unsigned int curMinDamage;
	unsigned int maxDamage;
	unsigned int curMaxDamage;
	int defence;
	int curDefence;
	int dodgeChance;
	int curDodgeChance;
	std::vector<Debuff*> debuffs;
public:
	Monster(std::string, unsigned short, int, int,
			unsigned int, unsigned int, int, int);
	virtual ~Monster() = 0;
	virtual void displayStats() const;

	int getCurMinDamage() const				{ return curMinDamage; }
	int getCurMaxDamage() const				{ return curMaxDamage; }
	int getCurDefence()	const				{ return curDefence; }
	int getCurDodgeChance()	const			{ return curDodgeChance; }

	void setCurMinDamage(int modifier)		{ curMinDamage		+= modifier; }
	void setCurMaxDamage(int modifier)		{ curMaxDamage		+= modifier; }
	void setCurDefence(int modifier)		{ curDefence		+= modifier; }
	void setCurDodgeChance(int modifier)	{ curDodgeChance	+= modifier; }

	void attack(Hero&);
	void checkDebuff();
	void setDebuff(Debuff* debuff)			{ debuffs.push_back(debuff); }
};

class Dragon : public Monster {
public:
	Dragon(std::string, unsigned short, int, int,
			unsigned int, unsigned int, int, int);
	~Dragon();
};

class Exoskeleton : public Monster {
public:
	Exoskeleton(std::string, unsigned short, int, int,
			unsigned int, unsigned int, int, int);
	~Exoskeleton();
};

class Spirit : public Monster {
public:
	Spirit(std::string, unsigned short, int, int,
			unsigned int, unsigned int, int, int);
	~Spirit();
};

#endif /* _LIVING_H_ */

#ifndef _WORLD_H_
#define _WORLD_H_

#include <iostream>
#include <cstdlib>
#include <list>
#include <vector>
#include "items.h"
#include "spells.h"
#include "living.h"
#include "world.h"

// functions that read info from files
namespace load{
	std::vector<Item*>* readItems();
	std::vector<Spell*>* readSpells();
	std::vector<std::string>* readMonsters();
}

// ================ Class Party ================
class Party {
private:
	std::list<Hero *> party;
public:
	// Return the number of the heroes
	int size() {	return party.size(); }
	void createHero(std::string, int);
	Hero * get(int pos);
	unsigned short min_lvl();
	bool isAlive();
};

// ================ Class Square and Subclasses ================
class Square {
public:
	virtual ~Square() {
	}
	virtual bool isNonAccessible() const {
		return false;
	}
	virtual bool hasMarket() const {
		return false;
	}
	virtual bool isCommon() const {
		return false;
	}
	void print_square();
	virtual bool access(Party party) {
		return false;
	}
};

class NonAccesible: public Square {
public:
	bool isNonAccessible() const {
		return true;
	}
};

class Market: public Square {
private:
	std::vector<Item *> * item_list;
	std::vector<Spell *> * spell_list;
public:
	bool hasMarket() const {
		return true;
	}
	Market(std::vector<Item *> * it, std::vector<Spell *> * sp);
	void Shop();
	std::vector<Item *> * getItemList() {
		return item_list;
	}
	std::vector<Spell *> * getSpellList() {
		return spell_list;
	}
};

class Common: public Square {
private:
	std::vector<Monster*> monster;
	std::vector<std::string> * monster_list;
	static const int RATE = 40;
	std::string rand_name() {
		int i = rand() % (monster_list->size());
		return (*(monster_list))[i];
	}
public:
	bool isCommon() const {
		return true;
	}
	Common(std::vector<std::string> * tmp) :
		monster_list(tmp) {
	}
	bool access(Party party);
	std::vector<Monster *> * getMonsters() {
		return &monster;
	}
	bool monsterIsAlive();
	Monster * next();
};

typedef struct Position {
	unsigned int x;
	unsigned int y;
} Position;

// ================ Class Grid ================
class Grid {
private:
	unsigned int dimensionX;
	unsigned int dimensionY;
	Party party;
	Position position;
	Square *map[10][20];
public:
	Grid();
	~Grid();
	void createHero(std::string, int);
	Square *randSquare(std::vector<std::string> * mon,
			std::vector<Item *> * it, std::vector<Spell *> * sp);
	void move(char direction);
	void trade();
	void battle();
	void displayStats();
	void displayMap(void);
	Hero * selectHero();
	void battleHelp();
	void checkInventory();
	void equip();
	void use();
	bool quitGame();
};

#endif /* _WORLD_H_ */

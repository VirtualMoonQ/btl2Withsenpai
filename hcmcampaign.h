/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 2
 * Programming Fundamentals Spring 2025
 * Date: 02.02.2025
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef H_HCM_CAMPAIGN_H
#define H_HCM_CAMPAIGN_H

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class Unit;
class UnitList;
class Army;
class TerrainElement;

class Vehicle;
class Infantry;

class LiberationArmy;
class ARVN;

class Position;

class Road;
class Mountain;
class River;
class Urban;
class Fortification;
class SpecialZone;

class BattleField;

class HCMCampaign;
class Configuration;

enum VehicleType
{
    TRUCK,
    MORTAR,
    ANTIAIRCRAFT,
    ARMOREDCAR,
    APC,
    ARTILLERY,
    TANK
};
enum InfantryType
{
    SNIPER,
    ANTIAIRCRAFTSQUAD,
    MORTARSQUAD,
    ENGINEER,
    SPECIALFORCES,
    REGULARINFANTRY
};

//3.4 Army Class
class Army
{
protected:
    int LF, EXP;
    string name;
    UnitList *unitList;
    BattleField *battleField;

public:
    Army();
    Army(Unit **unitArray, int size, string name, BattleField *battleField);
    virtual void fight(Army *enemy, bool defense = false) = 0;
    virtual string str() const = 0;
    
    //Extra functions
    bool sNum(int a);
    bool isInt(double n);
    int getLF();
    int getEXP();
    UnitList* getUnitList();
    void reCal();
    bool war = 0;
};
//3.4.1 LibeartionArmy Class
class LiberationArmy : public Army{
public:

    LiberationArmy ( Unit ** unitArray , int size , string name , BattleField * battleField );
    void fight(Army* enemy, bool defense = false) override;
    string str() const override;
    
    void deleteList(vector<Unit*> v);
    int nextFib(int n);
};
//3.4.2
class ARVN : public Army{
public:
    ARVN( Unit ** unitArray , int size , string name , BattleField * battleField );
    void fight(Army* enemy, bool defense = false) override;
    string str() const override;
};

//3.6 Position Class
class Position
{
private:
    int r, c;

public:
    Position(int r = 0, int c = 0) : r(r), c(c) {}

    Position(const string &str_pos)
    {
        sscanf(str_pos.c_str(), "(%d,%d)", &r, &c);
    }

    int getRow() const { return r; }
    int getCol() const { return c; }
    void setRow(int r) { this->r = r; }
    void setCol(int c) { this->c = c; }

    string str() const
    {
        ostringstream oss;
        oss << "(" << r << "," << c << ")";
        return oss.str();
    }
    
    bool equals(const Position& other) const {
            return this->c == other.c && this->r == other.r;
        }
};

class Unit
{
protected:
    int quantity, weight;
    Position pos;

public:
    Unit(int quantity, int weight, Position pos);
    virtual ~Unit();
    virtual int getAttackScore() = 0;
    Position getCurrentPosition() const;
    virtual string str() const = 0;
    
    //Extra functions
    int getQuantity(){
        return quantity;
    }
    void setQuantity(int q){
        quantity = q;
    }
    int getW(){ return weight;}
    void setW(int w){weight = w;}
    virtual bool isVehicle() const = 0;
    virtual bool isInfantry() const = 0;
    virtual VehicleType getVT() const = 0;
    virtual InfantryType getIT() const = 0;
    virtual Unit* dup() = 0;
    //End of extra functions
};

//3.5 UnitList Class
struct Node{
    Unit* data;
    Node* next;
    Node(Unit* unit, Node* next = nullptr) : data(unit), next(next) {}
};

class UnitList
{
private:
    int capacity;
    Node* head;
public:
    ~UnitList();
    UnitList();
    UnitList(int capacity);
    bool insert(Unit *unit);                   // return true if insert successfully
    bool isContain(VehicleType vehicleType);   // return true if it exists
    bool isContain(InfantryType infantryType); // return true if it exists
    string str() const;
    // TODO
    
    //--Extra functions
    int listSize(); 
    int getCapacity();
    Node* getHead();
    void setCapacity(int c);
    void setHead(Node* newHead);
    bool exist(Unit* u);
};

class TerrainElement
{
public:
    TerrainElement();
    ~TerrainElement();
    virtual void getEffect(Army *army) = 0;
};

class BattleField
{
private:
    int n_rows, n_cols;
    // TODO
public:
    BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                vector<Position *> arrayUrban, vector<Position *> arraySpecialZone);
    ~BattleField();
};

class HCMCampaign
{
private:
    Configuration *config;
    BattleField *battleField;
    LiberationArmy *liberationArmy;
    ARVN *arvn;

public:
    HCMCampaign(const string &config_file_path);
    void run();
    string printResult();
};


//3.2 Vehicle Class
class Vehicle : public Unit{
private:
    VehicleType vehicleType;
public:
    Vehicle( int quantity , int weight , const Position pos , VehicleType vehicleType );
    int getAttackScore() override;
    string str() const override;
    
    //--Extra funtions
    bool isInfantry() const override;
    bool isVehicle() const override;
    VehicleType getVT() const override {return vehicleType;}
    InfantryType getIT() const override {throw std::logic_error("");};
    Unit* dup() override;
    //--End of extra functions
};

//3.3 Infantry Class
class Infantry : public Unit{
private:
    InfantryType infantryType;
public:
    Infantry ( int quantity , int weight , const Position pos , InfantryType infantryType );
    int getAttackScore() override;
    string str() const override;
    
    
    //--Extra functions
    int computePersonalNumber(int score, int year = 1975);
    int digitSum(int n);
    bool isInfantry() const override;
    bool isVehicle() const override;
    InfantryType getIT() const override {return infantryType;}
    VehicleType getVT() const override {throw std::logic_error("");};
    Unit* dup() override;
    //--End of extra functions
};

#endif
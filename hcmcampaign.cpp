#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////

//3.1 Unit Class
Unit::Unit(int quantity, int weight, Position pos):quantity(quantity), weight(weight), pos(pos){}

Unit :: ~Unit(){}


///////////////////////////////////////////////////////
//3.2 Vehicle Class
//Constructor
Vehicle::Vehicle( int quantity , int weight , const Position pos , VehicleType vehicleType ): Unit(quantity, weight, pos), vehicleType(vehicleType){}

//getAttackScore Method
int Vehicle::getAttackScore(){
    return ceil(1.0 * ((int)vehicleType * 304 + quantity * weight ) / 30);
}

//str Method
string Vehicle::str() const{
    ostringstream oss;
    oss << "Vehicle[vehicleType=" << static_cast<int>(vehicleType)
        << ", quantity=" << quantity
        << ", weight=" << weight
        << ", pos=" << pos.str() << "]";
    return oss.str();
}



///////////////////////////////////////////////////////
//3.3 Infatnry Class

//Constructor
Infantry::Infantry( int quantity , int weight , const Position pos , InfantryType infantryType ): Unit(quantity, weight, pos), infantryType(infantryType){}

//getAttackScore Method
//--Extra functions
int Infantry::digitSum(int n){
    while (n >= 10) {
        int sum = 0;
        while (n > 0) {
            sum += n % 10;
            n /= 10;
        }
        n = sum;
    }
    return n;
}  
int Infantry::computePersonalNumber(int score, int year){
    int scoreDigitSum = 0, yDigitSum = 0;

    int s = score;
    while (s > 0) {
        scoreDigitSum += s % 10;
        s /= 10;
    }

    int y = year;
    while (y > 0) {
        yDigitSum += y % 10;
        y /= 10;
    }

    int total = scoreDigitSum + yDigitSum;
    return digitSum(total);  // Reduce to 1 digit
}
//--End of extra functions

int Infantry::getAttackScore(){
    int extra = 0;
    int score = 0;
    int q = getQuantity();

    if (infantryType == SPECIALFORCES && sqrt((double)weight) == (int)sqrt((double)weight)){
        extra = 75;
    }
    score = infantryType * 56 + quantity * weight + extra;
    if (computePersonalNumber(score) < 3){
        q -= q/10;
    } 
    else if (computePersonalNumber(score) > 7){
        q += q/5;
    }
    else{
        return score;
    }
    setQuantity(q);
    return getAttackScore();
}

//str Method
string Infantry::str() const {
    ostringstream oss;
    oss << "Infantry[infantryType=" << static_cast<int>(infantryType)
        << ", quantity=" << quantity
        << ", weight=" << weight
        << ", pos=" << pos.str() << "]";
    return oss.str();
}


///////////////////////////////////////////////////////
//3.4 Army Class
Army::Army() : unitList(nullptr), battleField(nullptr), name(""), LF(0), EXP(0) {}
Army::Army(Unit **unitArray, int size, string name, BattleField *battleField):name(name), battleField(battleField){
    
} 

///////////////////////////////////////////////////////
//3.5 UnitList Class
//Constructor UnitList() and UnitList(int) and destructor
UnitList::UnitList(){
    this -> head = nullptr;
}
UnitList::UnitList(int capacity): capacity(capacity){
    this -> head = nullptr;
}
UnitList::~UnitList(){
    Node* current = head;
    while(current){
        Node* next = current -> next;
        delete[] current -> data;
        delete[] current;
        current = next;
    }
}

//insert Method
//--Extra functions
bool Infantry :: isInfantry() const{
    return true;
}
bool Infantry :: isVehicle() const{
    return false;
}
bool Vehicle :: isInfantry() const{
    return false;
}
bool Vehicle :: isVehicle() const{
    return true;
}
int UnitList::listSize(){
    Node* current = head;
    int count;
    while(current != nullptr) count++;
    return count;
}
int UnitList::getCapacity(){
    return capacity;
}
//--End of extra functions
bool UnitList::insert(Unit* unit){
    cout << "got unit\n";
    if(unit == nullptr) return false;
    if(unit -> isVehicle()){
        cout << "is vehicle\n";
        VehicleType type = unit -> getVT();
        if(isContain(type)){
            cout << "entered here-1\n";
            Node* current = head;
            while(current != nullptr){
                if(current -> data -> isVehicle() && current -> data -> getVT() == type){
                    current -> data -> setQuantity(current -> data -> getQuantity() + unit -> getQuantity());
                    current -> data -> setW(max(current -> data -> getW(), unit -> getW()));
                    return true;
                }
                current = current -> next;
            }
        }
        else{
            cout << "entered here-2\n";
            if(this -> listSize() >= this -> getCapacity()) return false;
            Node* newNode = new Node(unit);
            if(head == nullptr) {
                head = newNode;
                return true;
            } else {
                Node* current = head;
                while(current -> next != nullptr) current = current -> next;
                newNode = current -> next;
                current -> next = newNode;
                return true;
            }

        }
    }
    else{
        cout << "is infantry\n";
        auto type = unit -> getIT();
        if(isContain(type)){
            cout << "entered here-1\n";
            Node* current = head;
            while(current != nullptr){
                if(current -> data -> isInfantry() && current -> data -> getIT() == type ){
                    current -> data -> setQuantity(current -> data -> getQuantity() + unit -> getQuantity());
                    current -> data -> setW(max(current -> data -> getW(), unit -> getW()));
                    return true;
                }
                current = current -> next;
            }
        }else{
            cout << "entered here-2\n";
            if(this -> listSize() >= this -> getCapacity()) return false;
            Node* newNode = new Node(unit);
            newNode -> next = head;
            head = newNode;
            return true;
        }
    }
    return false;
}

//isContain Method
bool UnitList::isContain(VehicleType vehicleType){
    Node* current = head;
    while(current != nullptr){
        if(current -> data -> isVehicle()){
            if(current -> data-> getVT() == vehicleType){
                return true;
            }
        }
        current = current -> next;
    }
    return false;
}
bool UnitList::isContain(InfantryType infantryType){
    Node* current = head;
    while(current != nullptr){
        if(current -> data -> isInfantry()){
            if(current -> data -> getIT() == infantryType){
                return true;
            }
        }
    current = current -> next;
    }
    return false;
}

//str() Method
string UnitList::str() const{
    int countV = 0, countI = 0, i = 0;
    Node* current = head;
    ostringstream unitlist;
    cout << "got here!\n";
    while(current != nullptr){
        if(current -> data -> isVehicle()) countV++;
        else countI++;
        if(current -> next != nullptr) unitlist << current -> data -> str() << ",";
        else unitlist << current -> data -> str();
        cout << i++;
        current = current -> next;
    }

    ostringstream oss;
    oss << "UnitList[count_vehicle=" << countV
        << ";count_infantry=" << countI
        << ";" << unitlist.str();
        
    return oss.str();
}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
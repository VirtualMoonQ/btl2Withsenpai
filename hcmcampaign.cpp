#include "hcmcampaign.h"
#include <unistd.h>
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
    string vt;
    switch(vehicleType){
        case TRUCK:
            vt = "TRUCK";
            break;
        case MORTAR:
            vt = "MORTAR";
            break;
        case ANTIAIRCRAFT:
            vt = "ANTIAIRCRAFT";
            break;
        case ARMOREDCAR:
            vt = "ARMOREDCAR";
            break;
        case APC:
            vt = "APC";
            break;
        case ARTILLERY:
            vt = "ARTILLERY";
            break;
        case TANK:
            vt = "TANK";
            break;
        default: break;
    }
    
    ostringstream oss;
    oss << "Vehicle[vehicleType=" << vt
        << ",quantity=" << quantity
        << ",weight=" << weight
        << ",position=" << pos.str() << "]";
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
    int q = this -> getQuantity();
    if (infantryType == SPECIALFORCES && sqrt((double)weight) == (int)sqrt((double)weight)){
        extra = 75;
    }
    score = infantryType * 56 + quantity * weight + extra;
    // cout << "Personal Score: " << computePersonalNumber(score) << endl;
    if (computePersonalNumber(score) < 3){
        q += ceil(q/10.0);
    } 
    else if (computePersonalNumber(score) > 7){
        q += ceil(q/5.0);
    }
    else{
        return score;
    }
    this -> setQuantity(q);
    return infantryType * 56 + this -> getQuantity() * weight + extra;
}


//str Method
string Infantry::str() const {
    string it;
    switch(infantryType){
        case SNIPER: 
            it = "SNIPER";
            break;
        case ANTIAIRCRAFTSQUAD:
            it = "ANTIAIRCRAFTSQUAD";
            break;
        case MORTARSQUAD:
            it = "MORTARSQUAD";
            break;
        case ENGINEER:
            it = "ENGINEER";
            break;
        case SPECIALFORCES:
            it = "SPECIALFORCES";
            break;
        case REGULARINFANTRY:
            it = "REGULARINFANTRY";
            break;
        default: break;
    }
    
    ostringstream oss;
    oss << "Infantry[infantryType=" << it
        << ",quantity=" << quantity
        << ",weight=" << weight
        << ",position=" << pos.str() << "]";
    return oss.str();
}


///////////////////////////////////////////////////////
//3.4 Army Class
//Constructor and Destructor
//--Extra functions
Node* UnitList::getHead(){
    return head;
}
void UnitList::setCapacity(int c){
    capacity = c;
}
bool Army::isInt(double n){
    return floor(n) == ceil(n);
}
bool Army :: sNum(int n){
    int primeUnder10[4] = {2, 3, 5, 7};
    for (int x : primeUnder10){
        for (int a = 0; pow(x, a) < sqrt(n); a++){
            int remain = n - pow(x, a);
            if (isInt(log(remain)/log(x))) return true;
        }
    }
    return false;
}    
Unit* Vehicle::dup(){
    return new Vehicle(*this);
}
Unit* Infantry::dup(){ 
    return new Infantry(*this);
}
//--End of extra functions
Army::Army() : unitList(nullptr), battleField(nullptr), name(""), LF(0), EXP(0) {}
Army::Army(Unit **unitArray, int size, string name, BattleField *battleField): LF(0), EXP(0), unitList(new UnitList(size)), name(name), battleField(battleField){
    int tempLF = 0, tempEXP = 0;
    for (int i = 0; i < size; i++){
        auto unit = unitArray[i];
        if(unit->isVehicle()){
            tempLF += unit->getAttackScore();
        }else{
            tempEXP += unit->getAttackScore();
        }
    }
    LF = min(tempLF,1000);
    EXP = min(tempEXP, 500);
    for(int i = 0; i < size; i++){
        Unit* duplicate = unitArray[i] -> dup();
        if(duplicate != NULL) this -> unitList -> insert(duplicate);
    }
    
}

//3.4.1 LiberationArmy Class
//Constructor
LiberationArmy::LiberationArmy(Unit **unitArray , int size , string name , BattleField * battleField ) : Army(unitArray, size, name , battleField){}

//fight Method
//--Extra Class
class ArmyHelper {
public:
    vector<Unit*> bestCombo;
    int bestScore = 2147483647;   // minimal score > target

    void dfs(const vector<Unit*>& units,
             size_t               idx,
             int                       currentScore,
             vector<Unit*>&       currentCombo,
             int                       targetScore)
    {
        /* perfect hit – cannot improve further */
        if (currentScore > targetScore && currentScore < bestScore)
        {
            // cout << endl << currentScore << " > " << targetScore << endl
            bestScore  = currentScore;
            bestCombo  = currentCombo;
            return;
        }

        if (idx == units.size())
        {
            /* crossed the target but better than what we had */
            if (currentScore > targetScore && currentScore < bestScore)
            {
                bestScore = currentScore;
                bestCombo = currentCombo;
            }
            return;
        }

        /* ---------- include units[idx] ---------- */
        currentCombo.push_back(units[idx]);
        dfs(units,
            idx + 1,
            currentScore + units[idx]->getAttackScore(),
            currentCombo,
            targetScore);
        currentCombo.pop_back();                      // back-track

        /* ---------- skip units[idx] -------------- */
        dfs(units, idx + 1, currentScore, currentCombo, targetScore);
    }

    vector<Unit*> findBestCombo(const vector<Unit*>& units,
                                     int                       targetScore)
    {
        bestCombo.clear();
        bestScore = 2147483647;

        vector<Unit*> currentCombo;
        dfs(units, 0, 0, currentCombo, targetScore);
        return bestCombo;      // empty ⇒ no way to exceed target
    }
};

//--End of extra class
//--Extra functions
int Army::getEXP(){return EXP;}
int Army::getLF(){return LF;}

void UnitList::setHead(Node* newHead){
    this -> head = newHead;
}

void LiberationArmy::deleteList(vector<Unit*> v){ //checked
    if(v.empty()) return;
    Node* current = this -> unitList -> getHead();
    Node* prev = nullptr;
    for(Unit* target : v){
        while(current != nullptr){
            if(current -> data -> str() == target -> str()){
                Node* delTarget = current;
                if(prev == nullptr){
                    this -> unitList -> setHead(current->next);
                    current = current->next;
                }
                else{
                    prev = current -> next;
                    current = current -> next;
                }
            delete delTarget;
            break;
            }
            else{
                prev = current;
                current = current->next;
            }
        }
    }
}

UnitList* Army::getUnitList(){
    return this -> unitList;
}


void Army :: reCal(){ //checked
    if (!unitList || !unitList->getHead()) {
        LF = 0;
        EXP = 0;
        return;
    }
    Node* current = this->unitList->getHead();
    int tempEXP = 0, tempLF = 0;
    while (current != NULL){
        if (current->data->isInfantry()){
            tempEXP += current->data->getAttackScore();
        }
        else {
            tempLF += current->data->getAttackScore();
        }
        current = current->next;
    }
    LF = min(tempLF, 1000);
    EXP = min(tempEXP, 500);
    int S = LF + EXP;
    if (sNum(S)){
        unitList->setCapacity(12);
    }
    else {
        unitList->setCapacity(8);
    }
}

int LiberationArmy::nextFib(int n){ //checked
    int a = 1, b = 1;
    while(b <= n){
        int temp = b;
        b+=a;
        a = temp;
    }
    return b;
} 
//--End of extra fucntions
void LiberationArmy::fight(Army *enemy, bool defense ){
    if(!defense){
        LF = ceil(LF * 1.5);
        EXP = ceil(EXP * 1.5);
        
        vector<Unit*> VehicleList;
        vector<Unit*> InfantryList;
        Node* current = this -> unitList -> getHead();
        while(current != nullptr){
            if(current -> data -> isVehicle()){
                VehicleList.push_back(current -> data);
            }else InfantryList.push_back(current -> data);
            current = current -> next;
        }
        
        ArmyHelper helper;
        vector<Unit*> bestCommboV = helper.findBestCombo(VehicleList, enemy -> getLF());
        vector<Unit*> bestComboI = helper.findBestCombo(InfantryList, enemy -> getEXP());
        
        bool validLF = !bestCommboV.empty();
        bool validEXP = !bestComboI.empty();
        
        // for( int i = 0; i < bestCommboV.size(); i++){
        //     cout << bestCommboV[i] -> str() << ": " << bestCommboV[i] -> getAttackScore();
        //     cout << endl;
        // }
        // for( int i = 0; i < bestComboI.size(); i++){
        //     cout << bestComboI[i] -> str() << ": " << bestComboI[i] -> getAttackScore();
        //     cout << endl;
        // }
        // cout << "Enemy Vehicle: " << enemy -> getLF();
        // cout << "Enemy Infantry: " << enemy -> getEXP();
        // cout<<validEXP<< endl;
        // cout << validLF << endl;
        
        if(validEXP && validLF){
            // cout << "Toan thang!";

            deleteList(bestComboI);
            deleteList(bestCommboV);
            
            war = 1;
        }
        else if(validEXP || validLF){
            int sumLF = 0, sumEXP = 0;
            Node* current = this -> unitList ->getHead();
            while(current != nullptr){
                if(current -> data -> isInfantry()) sumEXP += current -> data -> getAttackScore();
                else sumLF += current -> data -> getAttackScore();
                current = current -> next;
            }
        
            int LibScore = validLF ? sumEXP : sumLF;
            int enemyScore = validLF ? enemy -> getEXP() : enemy -> getLF();
            
            if(LibScore > enemyScore){
                // string a = "Thang Vehicle", b = "Thang Infantry";
                // string c = validLF ? a : b;
                // cout << c;

                deleteList(validLF ? bestComboI : bestCommboV);
                
                war = 1;
            }
            else{
                // cout << "Khong giao tranh";
                war = 0;
            }
        }
        else{
            // cout << "Khong giao tranh";
            war = 0;
        }
        if(war){
            Node* eCurrent = enemy -> getUnitList() -> getHead();
            vector<Unit*> reversed;
            while(eCurrent != nullptr){
                Unit* duplicate = eCurrent -> data -> dup();
                reversed.push_back(duplicate);
                eCurrent = eCurrent -> next;
            }
            for(int i = reversed.size() - 1; i >= 0; i--){
                this -> unitList -> insert(reversed[i]);
            }
            enemy -> getUnitList() -> setHead(nullptr);
            this -> reCal();
        }
        else{
            Node* current = this -> unitList -> getHead();
            while(current != nullptr){
                current -> data -> setW(ceil(current -> data -> getW() * 0.9));
                current = current -> next;
            }
            this -> reCal();
        }
    }
    else{
        LF = ceil(LF * 1.3);
        EXP = ceil(EXP * 1.3);
        if(LF > enemy -> getLF() && EXP > enemy -> getEXP()){
            war = 1;
        }
        else if(LF > enemy -> getLF() || EXP > enemy -> getEXP()){
            Node* current = this -> unitList -> getHead();
            while(current != nullptr){
                current -> data -> setQuantity(ceil(current -> data -> getQuantity() * (90.0/ 100)));
                current = current -> next;
            }
            this -> reCal();
        }
        else {
            Node* current = this -> unitList -> getHead();
            while(current != nullptr){
                current -> data -> setQuantity(nextFib(current -> data -> getQuantity()));
            }
            this -> reCal();
        }
    }
}

//str() Method
string LiberationArmy::str() const{
    ostringstream oss;
    oss << "LiberationArmy["
        << "LF=" << LF
        << ",EXP=" << EXP
        << ",unitList=" << unitList -> str()
        << ",battleField="
        << "]";
    return oss.str();
}

//3.4.2 Army Class
//Constructor
ARVN::ARVN(Unit **unitArray , int size , string name , BattleField * battleField ) : Army(unitArray, size, name , battleField){}
//fight Method
//Extra functions
bool UnitList::exist(Unit* u){
    Node* current = head;
    while(current != nullptr){
        if(current -> data == u){
            return true;
        }
        current = current -> next;
    }
    return false;
}
//--End of extra functions
void ARVN::fight(Army* enemy, bool defense){
    if(!defense){
        Node* current = this -> unitList -> getHead();
        Node* prev = nullptr;
        while(current != nullptr){
            current -> data -> setQuantity( ceil(current -> data -> getQuantity() * 0.8) );
            // if(current -> data -> getQuantity() == 1){
            //     Node* del = current;
            //     current = current -> next;
            //     delete del;
            //     continue;
            // }
            // current = current -> next;
            if(current -> data -> getQuantity() == 1){
                Node* delTarget = current;
                if(prev == nullptr){
                    this -> unitList -> setHead(current->next);
                    current = current->next;
                }
                else{
                    prev = current -> next;
                    current = current -> next;
                }
            delete delTarget;
            continue;
            }
            prev = current;
            current = current->next;
        }
        this -> reCal();
    }
    else{
        if(!unitList || !unitList->getHead()){
            LF = 0;
            EXP = 0;
            return;
        }
        else{
            Node *current = unitList->getHead();
            UnitList* libList = enemy->getUnitList(); 
            if (libList->exist(current->data)){
                int x = current->data->getW();
                current->data->setW(ceil(0.8 * x));
            }
            else {
                Node* toDel = current;
                current = current->next;
                delete toDel;
                unitList->setHead(current);
            }
            while (current != NULL && current->next != NULL){
                if (!libList->exist(current->next->data)){
                    Node *toDel = current->next;
                    current->next = current->next->next;
                    delete toDel;
                }
                else {
                    int x = current->next->data->getW();
                    current->next->data->setW(ceil(0.8 * x));
                }
                current = current->next;
            }
            this->reCal();
        }
    }
}

//str Method
string ARVN::str()const{
    ostringstream oss;
    oss << "ARVN["
        << "LF=" << LF
        << ",EXP=" << EXP
        << ",unitList=" << unitList -> str()
        << ",battleField=" << "]";
    return oss.str();
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
        Node* next = current->next;
        // Only delete Unit* if ownership belongs to UnitList (which it doesn't in your current design).
        delete current;  // ✅ Correct: delete node
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
    int count = 0;
    while(current != nullptr) {
        count++;
        current = current -> next;
    }
    return count;
}
int UnitList::getCapacity(){
    return capacity;
}
//--End of extra functions
bool UnitList::insert(Unit* unit){
    if(unit == nullptr) return false;
    if(unit -> isVehicle()){
        VehicleType type = unit -> getVT();
        if(isContain(type)){
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
            if(this -> listSize() >= this -> getCapacity()) {
                return false;
            }
            Node* newNode = new Node(unit);
            if(head == nullptr) {
                head = newNode;
                return true;
            } else {
                Node* current = head;
                while(current -> next != nullptr) current = current -> next;
                current -> next = newNode;
                return true;
            }
            throw runtime_error("Got nothing inserted!");
        }
    }
    else{
        auto type = unit -> getIT();
        if(isContain(type)){
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
    // ostringstream unitlist;
    
    string unitlist = "";
    while(current != nullptr){
        // signal(SIGSEGV, signalHandler);
        if(current -> data -> isVehicle()) countV++;
        else countI++;
        unitlist += current -> data -> str();
        if(current -> next != nullptr) unitlist += ',';
        current = current -> next;
    }
    
    string semi = (unitlist != "") ? ";" : "";
    ostringstream oss;
    oss << "UnitList[count_vehicle=" << countV
        << ";count_infantry=" << countI
        <<  semi << unitlist << "]";
    
    return oss.str();
}

////////////////////////////////////////////////
//3.7 TerrainElement Class
TerrainElement::TerrainElement(){};
TerrainElement::~TerrainElement(){};

void Road::getEffect(Army* army){};
void Mountain::getEffect(Army* army){};
void River::getEffect(Army* army){};
void Urban::getEffect(Army* army){};
void Fortification::getEffect(Army* army){};
void SpecialZone::getEffect(Army* army){};



///////////////////////////////////////////////
//3.8 BattleField Class
BattleField::BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                        vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                        vector<Position *> arrayUrban, vector<Position *> arraySpecialZone){}
BattleField::~BattleField(){}

string BattleField::str(){return "";}



/////////////////////////////////////////////////
//3.9 Configuration
Configuration::Configuration(const string &filepath) {
    ifstream fin(filepath);
    string line;

    while (getline(fin, line)) {
        if (line.empty()) continue;

        if (line.find("NUM_ROWS=") == 0) {
            num_rows = stoi(line.substr(9));
        } else if (line.find("NUM_COLS=") == 0) {
            num_cols = stoi(line.substr(9));
        } else if (line.find("ARRAY_FOREST=") == 0) {
            arrayForest = parsePositions(line.substr(13));
        } else if (line.find("ARRAY_RIVER=") == 0) {
            arrayRiver = parsePositions(line.substr(12));
        } else if (line.find("ARRAY_FORTIFICATION=") == 0) {
            arrayFortification = parsePositions(line.substr(21));
        } else if (line.find("ARRAY_URBAN=") == 0) {
            arrayUrban = parsePositions(line.substr(12));
        } else if (line.find("ARRAY_SPECIAL_ZONE=") == 0) {
            arraySpecialZone = parsePositions(line.substr(20));
        } else if (line.find("UNIT_LIST=") == 0) {
            string block = line;
            while (block.find("]") == string::npos && getline(fin, line)) {
                block += line;
            }
            parseUnits(block.substr(10));
        } else if (line.find("EVENT_CODE=") == 0) {
            int code = stoi(line.substr(11));
            if (code < 0) eventCode = 0;
            else eventCode = code % 100;
        }
    }

    fin.close();
}

Configuration::~Configuration() {
    for (Position* p : arrayForest) delete p;
    for (Position* p : arrayRiver) delete p;
    for (Position* p : arrayFortification) delete p;
    for (Position* p : arrayUrban) delete p;
    for (Position* p : arraySpecialZone) delete p;
    for (Unit* u : liberationUnits) delete u;
    for (Unit* u : ARVNUnits) delete u;
}

vector<Position*> Configuration::parsePositions(const string &data) {
    vector<Position*> positions;
    size_t i = 0;
    while (i < data.length()) {
        if (data[i] == '(') {
            size_t j = data.find(')', i);
            if (j != string::npos) {
                string pair = data.substr(i, j - i + 1);
                positions.push_back(new Position(pair));
                i = j + 1;
            } else break;
        } else i++;
    }
    return positions;
}

void Configuration::parseUnits(const string &block) {
    size_t i = 0;
    while (i < block.length()) {
        // Skip to the first '('
        while (i < block.length() && block[i] != '(') i++;
        if (i == block.length()) break;

        // Extract unit name before '('
        size_t nameEnd = i;
        size_t nameStart = nameEnd;
        while (nameStart > 0 && (isalpha(block[nameStart - 1]) || block[nameStart - 1] == '_')) {
            nameStart--;
        }
        string name = block.substr(nameStart, nameEnd - nameStart);

        // Find matching closing parenthesis
        int parenCount = 1;
        size_t j = i + 1;
        while (j < block.length() && parenCount > 0) {
            if (block[j] == '(') parenCount++;
            else if (block[j] == ')') parenCount--;
            j++;
        }
        if (parenCount != 0) break;

        string args = block.substr(i + 1, j - i - 2);  // exclude outer parens
        stringstream ss(args);
        string token;

        int q, w, r, c, b;
        getline(ss, token, ','); q = stoi(token);
        getline(ss, token, ','); w = stoi(token);

        // Now read full position "(r,c)"
        getline(ss, token, ',');  // should be like "(1"
        r = stoi(token.substr(1)); // remove '('

        getline(ss, token, ',');  // just c
        c = stoi(token);

        getline(ss, token);  // should be like "0)" but we already excluded ')'
        b = stoi(token);

        Position pos(r, c);
        Unit* unit = nullptr;

        // Instantiate unit by name
        if (name == "TANK") unit = new Vehicle(q, w, pos, TANK);
        else if (name == "TRUCK") unit = new Vehicle(q, w, pos, TRUCK);
        else if (name == "ARTILLERY") unit = new Vehicle(q, w, pos, ARTILLERY);
        else if (name == "APC") unit = new Vehicle(q, w, pos, APC);
        else if (name == "ARMOREDCAR") unit = new Vehicle(q, w, pos, ARMOREDCAR);
        else if (name == "ANTIAIRCRAFT") unit = new Vehicle(q, w, pos, ANTIAIRCRAFT);
        else if (name == "MORTAR") unit = new Vehicle(q, w, pos, MORTAR);
        else if (name == "SNIPER") unit = new Infantry(q, w, pos, SNIPER);
        else if (name == "REGULARINFANTRY") unit = new Infantry(q, w, pos, REGULARINFANTRY);
        else if (name == "ENGINEER") unit = new Infantry(q, w, pos, ENGINEER);
        else if (name == "SPECIALFORCES") unit = new Infantry(q, w, pos, SPECIALFORCES);
        else if (name == "MORTARSQUAD") unit = new Infantry(q, w, pos, MORTARSQUAD);
        else if (name == "ANTIAIRCRAFTSQUAD") unit = new Infantry(q, w, pos, ANTIAIRCRAFTSQUAD);

        if (unit) {
            if (b == 0) liberationUnits.push_back(unit);
            else ARVNUnits.push_back(unit);
        }

        i = j + 1;  // move past current unit
    }
}


string Configuration::str() {
    ostringstream oss;
    oss << "[";
    oss << "num_rows=" << num_rows << ",";
    oss << "num_cols=" << num_cols << ",";

    auto printPositions = [&](const vector<Position*>& vec) {
        oss << "[";
        for (size_t i = 0; i < vec.size(); ++i) {
            oss << vec[i]->str();
            if (i != vec.size() - 1) oss << ",";
        }
        oss << "]";
    };

    oss << "arrayForest="; printPositions(arrayForest); oss << ",";
    oss << "arrayRiver="; printPositions(arrayRiver); oss << ",";
    oss << "arrayFortification="; printPositions(arrayFortification); oss << ",";
    oss << "arrayUrban="; printPositions(arrayUrban); oss << ",";
    oss << "arraySpecialZone="; printPositions(arraySpecialZone); oss << ",";

    oss << "liberationUnits=[";
    for (size_t i = 0; i < liberationUnits.size(); ++i) {
        oss << liberationUnits[i]->str();
        if (i != liberationUnits.size() - 1) oss << ",";
    }
    oss << "],";

    oss << "ARVNUnits=[";
    for (size_t i = 0; i < ARVNUnits.size(); ++i) {
        oss << ARVNUnits[i]->str();
        if (i != ARVNUnits.size() - 1) oss << ",";
    }
    oss << "],";

    oss << "eventCode=" << setw(2) << setfill('0') << eventCode;
    oss << "]";
    return oss.str();
}



////////////////////////////////////////////////
//3.10 HCMCampaign Class
HCMCampaign::HCMCampaign(const string& config_file_path){}

void HCMCampaign::run(){
    
}

string HCMCampaign::printResult(){return "";}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
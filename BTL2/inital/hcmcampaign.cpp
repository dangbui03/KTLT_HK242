#include "hcmcampaign.h"
#include <algorithm>

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////

// Kiểm tra có phải số chính phương không
bool isPerfectSquare(int n)
{
    if (n < 0)
        return false;                      // Số âm không thể là số chính phương
    int sqrtN = static_cast<int>(sqrt(n)); // Lấy căn bậc hai của n
    return (sqrtN * sqrtN == n);           // Kiểm tra nếu căn bậc hai đã làm tròn lại bình phương ra số ban đầu
}

#pragma region Position
Position::Position(int r, int c)
{
    this->r = r;
    this->c = c;
};

Position::Position(const string &str_pos)
{
    // Parse position from string format "(r,c)"
    int start = str_pos.find('(') + 1;
    int comma = str_pos.find(',');
    int end = str_pos.find(')');

    string r_str = str_pos.substr(start, comma - start);
    string c_str = str_pos.substr(comma + 1, end - comma - 1);

    r = stoi(r_str);
    c = stoi(c_str);
}

string Position::str() const
{
    return "(" + to_string(r) + "," + to_string(c) + ")";
};

int Position::getRow() const
{
    return r;
};

int Position::getCol() const
{
    return c;
};

void Position::setRow(int r)
{
    this->r = r;
};

void Position::setCol(int c)
{
    this->c = c;
};
#pragma endregion Position

#pragma region Unit
Unit::Unit(int quantity, int weight, Position pos)
{
    this->quantity = quantity;
    this->weight = weight;
    this->pos = pos;
};

Unit::~Unit() {};

Position Unit::getCurrentPosition() const
{
    return pos;
};

string Unit::str() const
{
    return "";
};
#pragma endregion Unit

#pragma region Vehicle
string Vehicle::vehicleTypeToString(VehicleType type) const
{
    switch (type)
    {
    case TRUCK:
        return "TRUCK";
    case MORTAR:
        return "MORTAR";
    case ANTIAIRCRAFT:
        return "ANTIAIRCRAFT";
    case ARMOREDCAR:
        return "ARMOREDCAR";
    case APC:
        return "APC";
    case ARTILLERY:
        return "ARTILLERY";
    case TANK:
        return "TANK";
    default:
        return "Unknown VehicleType";
    }
}
Vehicle::Vehicle(int quantity, int weight, const Position pos, VehicleType type)
    : Unit(quantity, weight, pos), vehicleType(type) {}

VehicleType Vehicle::getType() const
{
    return vehicleType;
}

int Vehicle::getAttackScore()
{
    // score = (typeValue * 304 + quantity * weight) / 30
    int typeValue = static_cast<int>(vehicleType);
    int score = (typeValue * 304 + quantity * weight) / 30;
    return score;
}

string Vehicle::str() const
{
    return "Vehicle[vehicleType=" + to_string(static_cast<int>(vehicleType)) +
           ",quantity=" + to_string(quantity) +
           ",weight=" + to_string(weight) +
           ",pos=" + pos.str() + "]";
}
#pragma endregion Vehicle

// class Vehicle : public Unit
// {
// private:
//     VehicleType vehicleType;

//     string vehicleTypeToString(VehicleType type) const
//     {
//         switch (type)
//         {
//         case TRUCK:
//             return "TRUCK";
//         case MORTAR:
//             return "MORTAR";
//         case ANTIAIRCRAFT:
//             return "ANTIAIRCRAFT";
//         case ARMOREDCAR:
//             return "ARMOREDCAR";
//         case APC:
//             return "APC";
//         case ARTILLERY:
//             return "ARTILLERY";
//         case TANK:
//             return "TANK";
//         default:
//             return "Unknown VehicleType";
//         }
//     }

// public:
//     Vehicle(int quantity, int weight, const Position pos, VehicleType vehicleType)
//         : Unit(quantity, weight, pos), vehicleType(vehicleType)
//     {
//     }
//     ~Vehicle() {} // Clean up memory

//     int getAttackScore() override
//     {
//         int typeValue = vehicleType;
//         int score = (typeValue * 304 + quantity * weight) / 30;
//         if (score > 1000)
//             score = 1000;
//         else if (score < 0)
//             score = 0;
//         return score; // Round to threshold if exceeds
//     }

//     string str() const override
//     {
//         return "Vehicle[" + vehicleTypeToString(vehicleType) + '=' + to_string(vehicleType) + "," + to_string(quantity) + "," + to_string(weight) + "," + pos.str() + "]";
//     }
// };

#pragma region Infantry
Infantry::Infantry(int quantity, int weight, const Position pos, InfantryType type)
    : Unit(quantity, weight, pos), infantryType(type) {}

InfantryType Infantry::getType() const
{
    return infantryType;
}

bool Infantry::isPerfectSquare(int x) const
{
    if (x < 0)
        return false;
    int sqrt_x = static_cast<int>(sqrt(x));
    return sqrt_x * sqrt_x == x;
}

int Infantry::personalNumber(int number) const
{
    int year = 1975;
    int sum = number;

    // Add digits of year
    sum += 1 + 9 + 7 + 5;

    // Keep summing until single digit
    while (sum >= 10)
    {
        int temp = sum;
        sum = 0;
        while (temp > 0)
        {
            sum += temp % 10;
            temp /= 10;
        }
    }

    return sum;
}

int Infantry::getAttackScore()
{
    int typeValue = static_cast<int>(infantryType);
    int score = typeValue * 56 + quantity * weight;

    // Special condition for SPECIALFORCES
    if (infantryType == SPECIALFORCES && isPerfectSquare(weight))
    {
        score += 75; // Add 75 points for commandos
    }

    // Calculate personal number
    int personalNum = personalNumber(score);

    // Apply reinforcement or desertion
    if (personalNum > 7)
    {
        // Reinforcements: increase quantity by 20%
        int reinforcement = static_cast<int>(ceil(quantity * 0.2));
        quantity += reinforcement;
        // Recalculate score
        score = typeValue * 56 + quantity * weight;

        // Add commando bonus again if applicable
        if (infantryType == SPECIALFORCES && isPerfectSquare(weight))
        {
            score += 75;
        }
    }
    else if (personalNum < 3)
    {
        // Desertion: decrease quantity by 10%
        int desertion = static_cast<int>(ceil(quantity * 0.1));
        quantity -= desertion;
        if (quantity < 1)
            quantity = 1; // Ensure at least 1 unit remains

        // Recalculate score
        score = typeValue * 56 + quantity * weight;

        // Add commando bonus again if applicable
        if (infantryType == SPECIALFORCES && isPerfectSquare(weight))
        {
            score += 75;
        }
    }

    return score;
}

string Infantry::str() const
{
    string infantryTypeStr;
    switch (infantryType) {
        case SNIPER: infantryTypeStr = "SNIPER"; break;
        case ANTIAIRCRAFTSQUAD: infantryTypeStr = "ANTIAIRCRAFTSQUAD"; break;
        case MORTARSQUAD: infantryTypeStr = "MORTARSQUAD"; break;
        case ENGINEER: infantryTypeStr = "ENGINEER"; break;
        case SPECIALFORCES: infantryTypeStr = "SPECIALFORCES"; break;
        case REGULARINFANTRY: infantryTypeStr = "REGULARINFANTRY"; break;
        default: infantryTypeStr = "Unknown"; break;
    }
    
    return "Infantry[infantryType=" + infantryTypeStr +
           ",quantity=" + to_string(quantity) +
           ",weight=" + to_string(weight) +
           ",pos=" + pos.str() + "]";
}
#pragma endregion Infantry

// class Infantry : public Unit
// {
// private:
//     InfantryType infantryType;

//     // tìm tổng các chữ số của một số
//     int sumOfDigits(int num)
//     {
//         int sum = 0;
//         while (num > 0)
//         {
//             sum += num % 10;
//             num /= 10;
//         }
//         return sum;
//     }

//     // Tìm số cá nhân
//     int getPersonalScore(int score)
//     {
//         int personalScore = sumOfDigits(score) + sumOfDigits(1975);
//         while (personalScore > 9)
//         {
//             personalScore = sumOfDigits(personalScore);
//         }
//         return personalScore;
//     }

//     string infantryTypeToString(InfantryType type) const
//     {
//         switch (type)
//         {
//         case SNIPER:
//             return "SNIPER";
//         case ANTIAIRCRAFTSQUAD:
//             return "ANTIAIRCRAFTSQUAD";
//         case MORTARSQUAD:
//             return "MORTARSQUAD";
//         case ENGINEER:
//             return "ENGINEER";
//         case SPECIALFORCES:
//             return "SPECIALFORCES";
//         case REGULARINFANTRY:
//             return "REGULARINFANTRY";
//         default:
//             return "Unknown InfantryType";
//         }
//     }

// public:
//     Infantry(int quantity, int weight, const Position pos, InfantryType infantryType) : Unit(quantity, weight, pos), infantryType(infantryType) {}

//     ~Infantry() {}

//     int getAttackScore() override
//     {
//         int typeValue = infantryType;
//         int score = typeValue * 56 + quantity * weight;

//         if ((infantryType == SPECIALFORCES) && isPerfectSquare(weight))
//         {
//             score += 75;
//         }

//         int personalScore = getPersonalScore(score);
//         if (personalScore > 7)
//         {
//             quantity = (int)ceil(quantity * 1.2);
//             score = typeValue * 56 + quantity * weight;
//         }
//         else if (personalScore < 4)
//         {
//             quantity = (int)ceil(quantity * 0.9);
//         }

//         if (score > 500)
//             score = 500;
//         else if (score < 0)
//             score = 0;
//         return score;
//     }

//     string str() const override
//     {
//         return "Infantry[" + infantryTypeToString(infantryType) + '=' + to_string(infantryType) + "," + to_string(quantity) + "," + to_string(weight) + "," + pos.str() + "]";
//     }
// };

// UnitList class implementation
#pragma region UnitList
UnitList::UnitList(int capacity)
    : head(nullptr), tail(nullptr), capacity(capacity), sizeV(0), sizeI(0), length(0) {}

UnitList::~UnitList()
{
    Node *current = head;
    while (current != nullptr)
    {
        Node *next = current->next;
        delete current->data;
        delete current;
        current = next;
    }
}

bool UnitList::isVehicle(Unit *u) const
{
    return dynamic_cast<Vehicle *>(u) != nullptr;
}

bool UnitList::sameVehicle(Node *n, VehicleType t) const
{
    if (!n || !isVehicle(n->data))
        return false;
    Vehicle *v = dynamic_cast<Vehicle *>(n->data);
    return v->getType() == t;
}

bool UnitList::sameInfantry(Node *n, InfantryType t) const
{
    if (!n || isVehicle(n->data))
        return false;
    Infantry *i = dynamic_cast<Infantry *>(n->data);
    return i->getType() == t;
}

bool UnitList::insert(Unit *unit)
{
    // Check if list is full
    if (length >= capacity)
    {
        return false;
    }

    Node *newNode = new Node(unit);

    // If empty list
    if (head == nullptr)
    {
        head = tail = newNode;
        length++;
        if (isVehicle(unit))
            sizeV++;
        else
            sizeI++;
        return true;
    }

    // Check if unit already exists
    if (isVehicle(unit))
    {
        Vehicle *v = dynamic_cast<Vehicle *>(unit);
        Node *current = head;
        while (current != nullptr)
        {
            if (sameVehicle(current, v->getType()))
            {
                // Update quantity
                current->data->increaseQuantity(unit->getQuantity());
                delete newNode->data;
                delete newNode;
                return true;
            }
            current = current->next;
        }

        // Add vehicle to end
        tail->next = newNode;
        tail = newNode;
        sizeV++;
    }
    else
    {
        Infantry *i = dynamic_cast<Infantry *>(unit);
        Node *current = head;
        while (current != nullptr)
        {
            if (sameInfantry(current, i->getType()))
            {
                // Update quantity
                current->data->increaseQuantity(unit->getQuantity());
                delete newNode->data;
                delete newNode;
                return true;
            }
            current = current->next;
        }

        // Add infantry to front
        newNode->next = head;
        head = newNode;
        if (tail == nullptr)
            tail = head;
        sizeI++;
    }

    length++;
    return true;
}

bool UnitList::isContain(VehicleType vehicleType)
{
    Node *current = head;
    while (current != nullptr)
    {
        if (sameVehicle(current, vehicleType))
        {
            return true;
        }
        current = current->next;
    }
    return false;
}

bool UnitList::isContain(InfantryType infantryType)
{
    Node *current = head;
    while (current != nullptr)
    {
        if (sameInfantry(current, infantryType))
        {
            return true;
        }
        current = current->next;
    }
    return false;
}

string UnitList::str() const
{
    string result = "UnitList[count_vehicle=" + to_string(sizeV) +
                    ";count_infantry=" + to_string(sizeI) + ";";

    Node *current = head;
    bool first = true;
    while (current != nullptr)
    {
        if (!first)
            result += ",";
        result += current->data->str();
        first = false;
        current = current->next;
    }

    result += "]";
    return result;
}
#pragma endregion UnitList

void Mountain::getEffect(Army *army)
{
    if (dynamic_cast<LiberationArmy *>(army))
    {
        // Liberation Army effect
        // Radius 2 units
        army->recalcIndices();
    }
    else if (dynamic_cast<ARVN *>(army))
    {
        // ARVN effect
        // Radius 4 units
        army->recalcIndices();
    }
}

void River::getEffect(Army *army)
{
    // Both armies: Infantry attackScore reduced by 10% within radius 2
    army->recalcIndices();
}

void Urban::getEffect(Army *army)
{
    if (dynamic_cast<LiberationArmy *>(army))
    {
        // Liberation Army effect
        // SPECIALFORCES/REGULARINFANTRY within radius 5: increase attackScore
        // ARTILLERY within radius 2: decrease attackScore by 50%
        army->recalcIndices();
    }
    else if (dynamic_cast<ARVN *>(army))
    {
        // ARVN effect
        // REGULARINFANTRY within radius 3: increase attackScore
        army->recalcIndices();
    }
}

void Fortification::getEffect(Army *army)
{
    if (dynamic_cast<LiberationArmy *>(army))
    {
        // Liberation Army effect
        // All units within radius 2: reduce attackScore by 20%
        army->recalcIndices();
    }
    else if (dynamic_cast<ARVN *>(army))
    {
        // ARVN effect
        // All units within radius 2: increase attackScore by 20%
        army->recalcIndices();
    }
}

void SpecialZone::getEffect(Army *army)
{
    // Both armies: Units within radius 1 have attackScore reduced to 0
    army->recalcIndices();
}

BattleField::BattleField(int n_rows, int n_cols,
                         vector<Position *> forest,
                         vector<Position *> river,
                         vector<Position *> fortification,
                         vector<Position *> urban,
                         vector<Position *> special)
    : n_rows(n_rows), n_cols(n_cols)
{

    // Initialize terrain grid with roads (nullptr)
    terrain.resize(n_rows);
    for (int i = 0; i < n_rows; i++)
    {
        terrain[i].resize(n_cols, nullptr);
    }

    // Add terrain elements
    for (auto pos : forest)
    {
        terrain[pos->getRow()][pos->getCol()] = new Mountain(*pos);
    }

    for (auto pos : river)
    {
        terrain[pos->getRow()][pos->getCol()] = new River(*pos);
    }

    for (auto pos : fortification)
    {
        terrain[pos->getRow()][pos->getCol()] = new Fortification(*pos);
    }

    for (auto pos : urban)
    {
        terrain[pos->getRow()][pos->getCol()] = new Urban(*pos);
    }

    for (auto pos : special)
    {
        terrain[pos->getRow()][pos->getCol()] = new SpecialZone(*pos);
    }

    // Fill remaining cells with roads
    for (int i = 0; i < n_rows; i++)
    {
        for (int j = 0; j < n_cols; j++)
        {
            if (terrain[i][j] == nullptr)
            {
                terrain[i][j] = new Road(Position(i, j));
            }
        }
    }
}

BattleField::~BattleField()
{
    for (int i = 0; i < n_rows; i++)
    {
        for (int j = 0; j < n_cols; j++)
        {
            delete terrain[i][j];
        }
    }
}

string BattleField::str() const
{
    return "BattleField[n_rows=" + to_string(n_rows) + ",n_cols=" + to_string(n_cols) + "]";
}

TerrainElement *BattleField::at(int r, int c) const
{
    if (r >= 0 && r < n_rows && c >= 0 && c < n_cols)
    {
        return terrain[r][c];
    }
    return nullptr;
}

bool Army::isSpecial(int S) {
    // Check if S is a special number for any odd prime base less than 10
    // Odd primes less than 10: 3, 5, 7
    int primes[] = {3, 5, 7};
    
    for (int base : primes) {
        // Check if S can be expressed as sum of distinct powers of base
        int sum = 0;
        int powers[32] = {0}; // To track which powers we've used
        int copy = S;
        bool possible = true;
        
        // Try to decompose S into sum of powers of base
        for (int power = 0; possible && copy > 0; power++) {
            int value = pow(base, power);
            if (value > copy) {
                possible = false;
                break;
            }
            
            // If adding this power doesn't exceed S, include it
            if (sum + value <= S) {
                sum += value;
                powers[power] = 1;
                copy -= value;
                
                // If we've reached S, we've found a valid decomposition
                if (sum == S) {
                    return true;
                }
            }
            
            // If we can't add the current power, try the next one
        }
    }
    
    return false;
}

Army::Army(Unit** unitArray, int size, string name, BattleField* bf) 
    : LF(0), EXP(0), name(name), battleField(bf) {
    
    // Calculate the capacity for the unit list based on size
    // Use isSpecial helper to determine capacity
    int capacity = isSpecial(size) ? 2 * size : size;
    
    // Create the unit list with the calculated capacity
    unitList = new UnitList(capacity);
    
    // Insert each unit into the unitList and calculate LF and EXP
    for (int i = 0; i < size; i++) {
        if (unitList->insert(unitArray[i])) {
            // Check if the unit is a Vehicle or Infantry
            Vehicle* vehicle = dynamic_cast<Vehicle*>(unitArray[i]);
            if (vehicle) {
                // It's a Vehicle, add to LF
                LF += vehicle->getAttackScore();
            } else {
                // It's an Infantry, add to EXP
                Infantry* infantry = dynamic_cast<Infantry*>(unitArray[i]);
                if (infantry) {
                    EXP += infantry->getAttackScore();
                }
            }
        }
    }
    
    // Ensure LF and EXP are within their valid ranges
    LF = std::min(1000, std::max(0, LF));
    EXP = std::min(500, std::max(0, EXP));
}

Army::~Army() {
    delete unitList;
}

void Army::recalcIndices() {
    // Reset LF and EXP
    LF = 0;
    EXP = 0;
    
    // Recalculate by iterating through all units
    unitList->forEach([this](Unit* unit) {
        Vehicle* vehicle = dynamic_cast<Vehicle*>(unit);
        if (vehicle) {
            // It's a Vehicle
            LF += vehicle->getAttackScore();
        } else {
            // It's an Infantry
            Infantry* infantry = dynamic_cast<Infantry*>(unit);
            if (infantry) {
                EXP += infantry->getAttackScore();
            }
        }
    });
    
    // Ensure LF and EXP are within their valid ranges
    LF = std::min(1000, std::max(0, LF));
    EXP = std::min(500, std::max(0, EXP));
}

// ======================= LIBERATION ARMY ============================
LiberationArmy::LiberationArmy(Unit** unitArray, int size, string name, BattleField* bf)
    : Army(unitArray, size, name, bf) {
}

void LiberationArmy::confiscate(Army* enemy) {
    // Function to confiscate and repurpose enemy units
    // Implementation would depend on how enemy units are accessed
    recalcIndices();
}

void LiberationArmy::fight(Army* enemy, bool defense) {
    if (!defense) {
        // Offensive case
        // Multiply LF and EXP by 1.5
        int offensiveLF = static_cast<int>(LF * 1.5);
        int offensiveEXP = static_cast<int>(EXP * 1.5);
        
        // Implementation of attack strategy
        // This would require tracking the units selected for combinations A and B
        
        // Placeholder for fight logic
        confiscate(enemy);
    } else {
        // Defensive case
        // Multiply LF and EXP by 1.3
        int defensiveLF = static_cast<int>(LF * 1.3);
        int defensiveEXP = static_cast<int>(EXP * 1.3);
        
        // Compare indices
        if (defensiveLF >= enemy->getLF() && defensiveEXP >= enemy->getEXP()) {
            // Liberation Army wins
        } else if (defensiveLF < enemy->getLF() && defensiveEXP < enemy->getEXP()) {
            // Need reinforcements
            unitList->forEach([](Unit* unit) {
                // Increase quantity to nearest Fibonacci number
                int qty = unit->getQuantity();
                int fib1 = 1, fib2 = 1;
                while (fib2 < qty) {
                    int temp = fib2;
                    fib2 = fib1 + fib2;
                    fib1 = temp;
                }
                unit->increaseQuantity(fib2 - qty);
            });
            
            // Recalculate indices
            recalcIndices();
        } else {
            // One index is lower, reduce quantities by 10%
            unitList->forEach([](Unit* unit) {
                unit->scaleQuantity(0.9);
            });
            
            // Recalculate indices
            recalcIndices();
        }
    }
}

string LiberationArmy::str() const {
    return "LiberationArmy[name=" + name + 
           ",LF=" + to_string(LF) + 
           ",EXP=" + to_string(EXP) + 
           ",unitList=" + unitList->str() + 
           ",battleField=" + battleField->str() + "]";
}

// ======================= ARVN =======================================
ARVN::ARVN(Unit** unitArray, int size, string name, BattleField* bf)
    : Army(unitArray, size, name, bf) {
    // Constructor needs no additional functionality beyond what Army constructor provides
}

void ARVN::fight(Army* enemy, bool defense) {
    if (!defense) {
        // Offensive case (ARVN attacking the Liberation Army)
        // ARVN always loses when attacking and loses 20% of each unit's quantity
        
        // Create a list of units to remove (those with quantity = 1)
        vector<Unit*> unitsToRemove;
        
        unitList->forEach([&unitsToRemove](Unit* unit) {
            // Reduce quantity by 20%
            unit->scaleQuantity(0.8);
            
            // If quantity becomes 1, mark for removal
            if (unit->getQuantity() == 1) {
                unitsToRemove.push_back(unit);
            }
        });
        
        // Remove units with quantity = 1
        // Since UnitList doesn't have a direct remove method, we need to recreate the list
        // This is a simplified approach - in a real implementation, we would add a remove method to UnitList
        
        // Create a new UnitList with the same capacity
        UnitList* newList = new UnitList(unitList->vehicleCount() + unitList->infantryCount());
        
        // Re-add all units except those marked for removal
        unitList->forEach([&newList, &unitsToRemove](Unit* unit) {
            // Check if unit is not in the removal list
            bool shouldKeep = true;
            for (auto* unitToRemove : unitsToRemove) {
                if (unit == unitToRemove) {
                    shouldKeep = false;
                    break;
                }
            }
            
            if (shouldKeep) {
                // Create a deep copy of the unit
                if (Vehicle* vehicle = dynamic_cast<Vehicle*>(unit)) {
                    Vehicle* copy = new Vehicle(vehicle->getQuantity(), vehicle->getWeight(),
                                              vehicle->getCurrentPosition(), vehicle->getType());
                    newList->insert(copy);
                } else if (Infantry* infantry = dynamic_cast<Infantry*>(unit)) {
                    Infantry* copy = new Infantry(infantry->getQuantity(), infantry->getWeight(),
                                                infantry->getCurrentPosition(), infantry->getType());
                    newList->insert(copy);
                }
            }
        });
        
        // Delete old list and replace with new one
        delete unitList;
        unitList = newList;
        
        // Recalculate indices after changes
        recalcIndices();
    } 
    else {
        // Defensive case (ARVN being attacked by the Liberation Army)
        // The outcome depends on the Liberation Army's strategy
        
        // If the Liberation Army wins, ARVN units are confiscated
        // This will be handled by the Liberation Army's fight method
        
        // If the battle doesn't happen, ARVN forces are preserved
        // In this case, we don't need to do anything
        
        // If the Liberation Army confiscates units, we need to reduce weight by 20% for remaining units
        bool battleOccurred = false;  // This would need to be determined somehow
        
        // We need a way to know if a battle occurred
        // For now, we'll assume it did (this should be communicated between objects)
        battleOccurred = true;
        
        if (battleOccurred) {
            // Reduce weight by 20% for all remaining units
            unitList->forEach([](Unit* unit) {
                // Since we can't directly modify weight, we need a workaround
                // This is a placeholder for the actual implementation
                // We would need to add a method to reduce weight in the Unit class
                
                // Assuming we have a method like setWeight()
                int newWeight = static_cast<int>(unit->getWeight() * 0.8);
                if (newWeight < 1) newWeight = 1;
                // unit->setWeight(newWeight);  // This method doesn't exist yet
                
                // For now, we'll just note that this operation needs to happen
                // A proper implementation would require adding a weight modification method
            });
            
            // Recalculate indices after changes
            recalcIndices();
        }
    }
}

string ARVN::str() const {
    return "ARVN[name=" + name + 
           ",LF=" + to_string(LF) + 
           ",EXP=" + to_string(EXP) + 
           ",unitList=" + unitList->str() + 
           ",battleField=" + battleField->str() + "]";
}

// ======================= CONFIGURATION ==============================
Configuration::Configuration(const string& filepath) {
    // Default values
    num_rows = 0;
    num_cols = 0;
    eventCode = 0;
    
    // Open file
    ifstream file(filepath);
    if (!file.is_open()) {
        return;
    }
    
    string line;
    while (getline(file, line)) {
        // Skip empty lines
        if (line.empty()) continue;
        
        // Parse line
        size_t pos = line.find('=');
        if (pos == string::npos) continue;
        
        string key = line.substr(0, pos);
        string value = line.substr(pos + 1);
        
        if (key == "NUM_ROWS") {
            num_rows = stoi(value);
        } else if (key == "NUM_COLS") {
            num_cols = stoi(value);
        } else if (key == "EVENT_CODE") {
            eventCode = stoi(value);
            // Adjust event code
            if (eventCode > 99) {
                eventCode %= 100;
            } else if (eventCode < 0) {
                eventCode = 0;
            }
        } else if (key == "ARRAY_FOREST") {
            // Parse positions from value
            size_t start = value.find('[');
            size_t end = value.find(']');
            if (start != string::npos && end != string::npos) {
                string posArray = value.substr(start + 1, end - start - 1);
                stringstream ss(posArray);
                string posStr;
                while (getline(ss, posStr, ',')) {
                    Position* pos = parsePos(posStr);
                    if (pos) arrForest.push_back(pos);
                }
            }
        } else if (key == "ARRAY_RIVER") {
            // Parse positions from value
            size_t start = value.find('[');
            size_t end = value.find(']');
            if (start != string::npos && end != string::npos) {
                string posArray = value.substr(start + 1, end - start - 1);
                stringstream ss(posArray);
                string posStr;
                while (getline(ss, posStr, ',')) {
                    Position* pos = parsePos(posStr);
                    if (pos) arrRiver.push_back(pos);
                }
            }
        } else if (key == "ARRAY_FORTIFICATION") {
            // Parse positions from value
            size_t start = value.find('[');
            size_t end = value.find(']');
            if (start != string::npos && end != string::npos) {
                string posArray = value.substr(start + 1, end - start - 1);
                stringstream ss(posArray);
                string posStr;
                while (getline(ss, posStr, ',')) {
                    Position* pos = parsePos(posStr);
                    if (pos) arrFort.push_back(pos);
                }
            }
        } else if (key == "ARRAY_URBAN") {
            // Parse positions from value
            size_t start = value.find('[');
            size_t end = value.find(']');
            if (start != string::npos && end != string::npos) {
                string posArray = value.substr(start + 1, end - start - 1);
                stringstream ss(posArray);
                string posStr;
                while (getline(ss, posStr, ',')) {
                    Position* pos = parsePos(posStr);
                    if (pos) arrUrban.push_back(pos);
                }
            }
        } else if (key == "ARRAY_SPECIAL_ZONE") {
            // Parse positions from value
            size_t start = value.find('[');
            size_t end = value.find(']');
            if (start != string::npos && end != string::npos) {
                string posArray = value.substr(start + 1, end - start - 1);
                stringstream ss(posArray);
                string posStr;
                while (getline(ss, posStr, ',')) {
                    Position* pos = parsePos(posStr);
                    if (pos) arrSpecial.push_back(pos);
                }
            }
        } else if (key == "UNIT_LIST") {
            // Parse units from value
            size_t start = value.find('[');
            size_t end = value.rfind(']');
            if (start != string::npos && end != string::npos) {
                string unitArray = value.substr(start + 1, end - start - 1);
                // Parse individual units
                size_t unitStart = 0;
                size_t unitEnd = unitArray.find("),");
                while (unitEnd != string::npos) {
                    string unitStr = unitArray.substr(unitStart, unitEnd - unitStart + 1);
                    parseUnit(unitStr);
                    unitStart = unitEnd + 2;
                    unitEnd = unitArray.find("),", unitStart);
                }
                // Parse last unit
                string unitStr = unitArray.substr(unitStart);
                parseUnit(unitStr);
            }
        }
    }
    
    file.close();
}

Configuration::~Configuration() {
    // Free allocated memory
    for (auto pos : arrForest) delete pos;
    for (auto pos : arrRiver) delete pos;
    for (auto pos : arrFort) delete pos;
    for (auto pos : arrUrban) delete pos;
    for (auto pos : arrSpecial) delete pos;
    
    for (auto unit : liberationUnits) delete unit;
    for (auto unit : ARVNUnits) delete unit;
}

Position* Configuration::parsePos(const string& token) {
    // Remove whitespace
    string t = token;
    t.erase(remove_if(t.begin(), t.end(), ::isspace), t.end());
    
    if (t.length() < 5) return nullptr; // Minimum "(r,c)"
    
    return new Position(t);
}

void Configuration::parseUnit(const string& token) {
    // Parse unit type and parameters
    size_t nameEnd = token.find('(');
    if (nameEnd == string::npos) return;
    
    string unitType = token.substr(0, nameEnd);
    
    // Parse parameters
    string params = token.substr(nameEnd + 1, token.length() - nameEnd - 2);
    stringstream ss(params);
    string quantityStr, weightStr, posStr, armyStr;
    
    getline(ss, quantityStr, ',');
    getline(ss, weightStr, ',');
    
    // Extract position
    size_t posStart = params.find('(');
    size_t posEnd = params.find(')');
    posStr = params.substr(posStart, posEnd - posStart + 1);
    
    // Get army
    size_t lastComma = params.rfind(',');
    armyStr = params.substr(lastComma + 1);
    
    int quantity = stoi(quantityStr);
    int weight = stoi(weightStr);
    Position pos(posStr);
    int army = stoi(armyStr);
    
    Unit* unit = nullptr;
    
    // Create appropriate unit based on type
    if (unitType == "TANK") {
        unit = new Vehicle(quantity, weight, pos, TANK);
    } else if (unitType == "ARTILLERY") {
        unit = new Vehicle(quantity, weight, pos, ARTILLERY);
    } else if (unitType == "ARMOREDCAR") {
        unit = new Vehicle(quantity, weight, pos, ARMOREDCAR);
    } else if (unitType == "APC") {
        unit = new Vehicle(quantity, weight, pos, APC);
    } else if (unitType == "TRUCK") {
        unit = new Vehicle(quantity, weight, pos, TRUCK);
    } else if (unitType == "MORTAR") {
        unit = new Vehicle(quantity, weight, pos, MORTAR);
    } else if (unitType == "ANTIAIRCRAFT") {
        unit = new Vehicle(quantity, weight, pos, ANTIAIRCRAFT);
    } else if (unitType == "SNIPER") {
        unit = new Infantry(quantity, weight, pos, SNIPER);
    } else if (unitType == "ANTIAIRCRAFTSQUAD") {
        unit = new Infantry(quantity, weight, pos, ANTIAIRCRAFTSQUAD);
    } else if (unitType == "MORTARSQUAD") {
        unit = new Infantry(quantity, weight, pos, MORTARSQUAD);
    } else if (unitType == "ENGINEER") {
        unit = new Infantry(quantity, weight, pos, ENGINEER);
    } else if (unitType == "SPECIALFORCES") {
        unit = new Infantry(quantity, weight, pos, SPECIALFORCES);
    } else if (unitType == "REGULARINFANTRY") {
        unit = new Infantry(quantity, weight, pos, REGULARINFANTRY);
    }
    
    if (unit) {
        if (army == 0) {
            liberationUnits.push_back(unit);
        } else {
            ARVNUnits.push_back(unit);
        }
    }
}

Unit** Configuration::libArray() {
    if (liberationUnits.empty()) return nullptr;
    
    Unit** arr = new Unit*[liberationUnits.size()];
    for (size_t i = 0; i < liberationUnits.size(); i++) {
        arr[i] = liberationUnits[i];
    }
    return arr;
}

int Configuration::libSize() const {
    return liberationUnits.size();
}

Unit** Configuration::arvnArray() {
    if (ARVNUnits.empty()) return nullptr;
    
    Unit** arr = new Unit*[ARVNUnits.size()];
    for (size_t i = 0; i < ARVNUnits.size(); i++) {
        arr[i] = ARVNUnits[i];
    }
    return arr;
}

int Configuration::arvnSize() const {
    return ARVNUnits.size();
}

string Configuration::str() const {
    string result = "Configuration[\n";
    result += "num_rows=" + to_string(num_rows) + "\n";
    result += "num_cols=" + to_string(num_cols) + "\n";
    
    result += "eventCode=" + to_string(eventCode) + "\n";
    result += "forest=[";
    for (auto pos : arrForest) {
        result += pos->str() + ",";
    }
    result += "]\n";
    result += "river=[";
    for (auto pos : arrRiver) {
        result += pos->str() + ",";
    }
    result += "]\n";
    result += "fortification=[";
    for (auto pos : arrFort) {
        result += pos->str() + ",";
    }
    result += "]\n";
    result += "urban=[";
    for (auto pos : arrUrban) {
        result += pos->str() + ",";
    }
    result += "]\n";
    result += "special=[";
    for (auto pos : arrSpecial) {
        result += pos->str() + ",";
    }
    result += "]\n";
    result += "liberationUnits=[";
    for (auto unit : liberationUnits) {
        result += unit->str() + ",";
    }
    result += "]\n";
    result += "ARVNUnits=[";
    for (auto unit : ARVNUnits) {
        result += unit->str() + ",";
    }
    result += "]\n";
    result += "]\n";
    return result;
}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
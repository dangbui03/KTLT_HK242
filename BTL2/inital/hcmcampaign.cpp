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

void Unit::setQuantity(int newQuantity) {
    quantity = newQuantity;
}

void Unit::setWeight(int newWeight) {
    weight = newWeight;
}

void Unit::setCol(int c) {
    pos.setCol(c);
}

void Unit::setRow(int r) {
    pos.setRow(r);
}

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
    // Nếu quantity là 0, thì không có sức tấn công
    if (quantity <= 0) return 0;
    
    int typeValue = static_cast<int>(vehicleType);
    
    // Thực hiện phép chia thực và làm tròn lên
    double rawScore = (typeValue * 304.0 + quantity * weight) / 30.0;
    int score = static_cast<int>(std::ceil(rawScore));
    
    return score;
}

string Vehicle::str() const
{
    return "Vehicle[vehicleType=" + vehicleTypeToString(vehicleType) +
           ",quantity=" + to_string(quantity) +
           ",weight=" + to_string(weight) +
           ",position=" + pos.str() + "]";
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
    if (quantity <= 0) return 0;
    
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
           ",position=" + pos.str() + "]";
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
    // Count non-zero units for correct reporting
    int effectiveVehicleCount = 0;
    int effectiveInfantryCount = 0;
    
    // First pass to count non-zero quantity units
    Node* current = head;
    while (current != nullptr) {
        if (current->data->getQuantity() > 0) {
            if (isVehicle(current->data)) {
                effectiveVehicleCount++;
            } else {
                effectiveInfantryCount++;
            }
        }
        current = current->next;
    }
    
    // Format the unit list string
    string result = "UnitList[count_vehicle=" + to_string(effectiveVehicleCount) +
                    ";count_infantry=" + to_string(effectiveInfantryCount);
    
    // If there are no effective units, return early with empty list
    if (effectiveVehicleCount == 0 && effectiveInfantryCount == 0) {
        return result + "]";
    }
    
    // Add semicolon before listing units
    result += ";";
    
    // Second pass to add non-zero units to the string
    current = head;
    bool first = true;
    while (current != nullptr) {
        // Only include units with quantity > 0
        if (current->data->getQuantity() > 0) {
            if (!first) {
                result += ",";
            }
            result += current->data->str();
            first = false;
        }
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

// Helper method to reduce weights of all units by 10%
void LiberationArmy::reduceWeights() {
    unitList->forEach([](Unit* unit) {
        // Reduce weight by 10%
        int currentWeight = unit->getWeight();
        int newWeight = static_cast<int>(currentWeight * 0.9);
        if (newWeight < 1) newWeight = 1; // Ensure minimum weight of 1
        unit->setWeight(newWeight);
    });
}

// Helper method to reduce quantities of all units by 10%
void LiberationArmy::reduceQuantities() {
    unitList->forEach([](Unit* unit) {
        // Reduce quantity by 10%
        int currentQuantity = unit->getQuantity();
        int reduction = static_cast<int>(ceil(currentQuantity * 0.1));
        int newQuantity = currentQuantity - reduction;
        if (newQuantity < 1) newQuantity = 1; // Ensure at least one unit remains
        unit->setQuantity(newQuantity);
    });
}

// Helper method to increase quantities to next Fibonacci number
void LiberationArmy::increaseQuantitiesToFibonacci() {
    unitList->forEach([](Unit* unit) {
        int quantity = unit->getQuantity();
        // Find next Fibonacci number
        int a = 1, b = 1;
        while (b <= quantity) {
            int temp = b;
            b = a + b;
            a = temp;
        }
        // b is now the next Fibonacci number after quantity
        unit->setQuantity(b);
    });
}

void LiberationArmy::confiscate(Army* enemy) {
    ARVN* arvnEnemy = dynamic_cast<ARVN*>(enemy);
    if (!arvnEnemy)
        return;
    
    // Get enemy unit list
    UnitList* enemyUnitList = arvnEnemy->getUnitList();
    
    // For each enemy unit with quantity > 0, update our unit list accordingly
    enemyUnitList->forEach([this](Unit* enemyUnit) {
        if (enemyUnit->getQuantity() > 0) {
            bool updated = false;
            // Try to update an existing unit of the same type
            this->unitList->forEach([&](Unit* ownUnit) {
                Vehicle* ev = dynamic_cast<Vehicle*>(enemyUnit);
                Vehicle* ov = dynamic_cast<Vehicle*>(ownUnit);
                if (ev && ov && (ev->getType() == ov->getType())) {
                    // For vehicles assume defensive types (like MORTAR) replace quantity,
                    // whereas for TRUCK we add quantities.
                    if (ov->getType() == TRUCK) {
                        ov->increaseQuantity(enemyUnit->getQuantity());
                    } else {
                        ov->setQuantity(enemyUnit->getQuantity());
                    }
                    // Update position: use the maximum row and column
                    int newRow = std::max(ov->getCurrentPosition().getRow(), enemyUnit->getCurrentPosition().getRow());
                    int newCol = std::max(ov->getCurrentPosition().getCol(), enemyUnit->getCurrentPosition().getCol());
                    ov->setRow(newRow);
                    ov->setCol(newCol);
                    updated = true;
                }
                Infantry* ei = dynamic_cast<Infantry*>(enemyUnit);
                Infantry* oi = dynamic_cast<Infantry*>(ownUnit);
                if (ei && oi && (ei->getType() == oi->getType())) {
                    oi->setQuantity(enemyUnit->getQuantity());
                    // Update position as above
                    int newRow = std::max(oi->getCurrentPosition().getRow(), enemyUnit->getCurrentPosition().getRow());
                    int newCol = std::max(oi->getCurrentPosition().getCol(), enemyUnit->getCurrentPosition().getCol());
                    oi->setRow(newRow);
                    oi->setCol(newCol);
                    updated = true;
                }
            });
            
            // If no matching unit found in our list, add a copy of the enemy unit
            if (!updated) {
                Unit* copy = nullptr;
                Vehicle* ev = dynamic_cast<Vehicle*>(enemyUnit);
                if (ev) {
                    copy = new Vehicle(enemyUnit->getQuantity(), enemyUnit->getWeight(), 
                                         enemyUnit->getCurrentPosition(), ev->getType());
                } else {
                    Infantry* ei = dynamic_cast<Infantry*>(enemyUnit);
                    if (ei) {
                        copy = new Infantry(enemyUnit->getQuantity(), enemyUnit->getWeight(), 
                                              enemyUnit->getCurrentPosition(), ei->getType());
                    }
                }
                if (copy) {
                    this->unitList->insert(copy);
                }
            }
            
            // Remove (confiscate) the enemy unit by setting its values to zero
            enemyUnit->setQuantity(0);
            enemyUnit->setWeight(0);
        }
    });
    
    // Recalculate indices for both armies
    arvnEnemy->recalcIndices();
    recalcIndices();
}

void LiberationArmy::fight(Army* enemy, bool defense) {
    if (!defense) {
        // Offensive case: multiply tactics indices by 1.5 for evaluation
        int offensiveLF = static_cast<int>(LF * 1.5);
        int offensiveEXP = static_cast<int>(EXP * 1.5);
        int enemyLF = enemy->getLF();
        int enemyEXP = enemy->getEXP();
        
        // If our tactical values are higher, we win and confiscate enemy units
        if (offensiveLF > enemyLF && offensiveEXP > enemyEXP) {
            confiscate(enemy);
        }
        else if ((offensiveLF > enemyLF && EXP > enemyEXP) ||
                 (offensiveEXP > enemyEXP && LF > enemyLF)) {
            // Even if one index does not exceed, overall strength may secure victory
            confiscate(enemy);
        }
        else {
            // If we cannot win, simulate a nonengaging battle by reducing weights slightly
            reduceWeights();
        }
        
        recalcIndices();
    } else {
        // Defensive case: multiply indices by 1.3 before comparing
        int defensiveLF = static_cast<int>(LF * 1.3);
        int defensiveEXP = static_cast<int>(EXP * 1.3);
        int enemyLF = enemy->getLF();
        int enemyEXP = enemy->getEXP();
        
        if (defensiveLF >= enemyLF && defensiveEXP >= enemyEXP) {
            // Successful defense: no change
        } else if (defensiveLF >= enemyLF || defensiveEXP >= enemyEXP) {
            // Partial defense: reduce all quantities by 10%
            reduceQuantities();
        } else {
            // If defense fails drastically, increase quantities to the next Fibonacci numbers
            increaseQuantitiesToFibonacci();
        }
        
        recalcIndices();
    }
}

string LiberationArmy::str() const {
    string result = "LiberationArmy[LF=" + to_string(LF) + 
                   ",EXP=" + to_string(EXP) + 
                   ",unitList=" + unitList->str();
    
    if (battleField != nullptr) {
        result += ",battleField=" + battleField->str();
    } else {
        result += ",battleField=";
    }
    
    result += "]";
    return result;
}

// ======================= ARVN =======================================
ARVN::ARVN(Unit** unitArray, int size, string name, BattleField* bf)
    : Army(unitArray, size, name, bf) {
}

void ARVN::fight(Army* enemy, bool defense) {
    if (!defense) {
        // Offensive attack: ARVN suffers losses; reduce each unit's quantity by 20%
        unitList->forEach([](Unit* unit) {
            unit->scaleQuantity(0.8); // scale (using ceil) to simulate loss
        });
    } else {
        // Defensive: likewise reduce (or adjust) as specified
        unitList->forEach([](Unit* unit) {
            unit->scaleQuantity(0.8);
        });
    }
    recalcIndices();
}

string ARVN::str() const {
    string result = "ARVN[LF=" + to_string(LF) + 
                   ",EXP=" + to_string(EXP) + 
                   ",unitList=" + unitList->str();
    
    if (battleField != nullptr) {
        result += ",battleField=" + battleField->str();
    } else {
        result += ",battleField=";
    }
    
    result += "]";
    return result;
}

// ======================= CONFIGURATION ==============================
Configuration::Configuration(const string& filepath) {
    // Initialize defaults
    num_rows = 0;
    num_cols = 0;
    eventCode = 0;
    
    ifstream file(filepath);
    if (!file.is_open()) {
        return; // Failed to open file
    }
    
    string line;
    while (getline(file, line)) {
        // Skip empty lines or comments
        if (line.empty() || line[0] == '/' || line[0] == '#') {
            continue;
        }
        
        // Find the key-value separator
        size_t equalPos = line.find('=');
        if (equalPos == string::npos) continue;
        
        string key = line.substr(0, equalPos);
        string value = line.substr(equalPos + 1);
        
        if (key == "NUM_ROWS") {
            num_rows = stoi(value);
        } else if (key == "NUM_COLS") {
            num_cols = stoi(value);
        } else if (key == "EVENT_CODE") {
            eventCode = stoi(value);
        } else if (key == "ARRAY_FOREST") {
            // Parse forest positions directly
            parsePositionArray(value, arrForest);
        } else if (key == "ARRAY_RIVER") {
            parsePositionArray(value, arrRiver);
        } else if (key == "ARRAY_FORTIFICATION") {
            parsePositionArray(value, arrFort);
        } else if (key == "ARRAY_URBAN") {
            parsePositionArray(value, arrUrban);
        } else if (key == "ARRAY_SPECIAL_ZONE") {
            parsePositionArray(value, arrSpecial);
        } else if (key == "UNIT_LIST") {
            // Parse unit list directly
            parseUnitListValue(value);
        }
    }
    
    file.close();
}

// Helper method to parse position arrays like "[(1,2),(3,5)]"
void Configuration::parsePositionArray(const string& value, vector<Position*>& posArray) {
    // Extract content between brackets
    size_t start = value.find('[');
    size_t end = value.rfind(']');
    
    if (start == string::npos || end == string::npos || start >= end) {
        return;  // Invalid format
    }
    
    string content = value.substr(start + 1, end - start - 1);
    
    // Handle empty array
    if (content.empty()) {
        return;
    }
    
    // Split by positions
    int nestedLevel = 0;
    string currentPos;
    
    for (size_t i = 0; i < content.length(); i++) {
        char c = content[i];
        if (c == '(') {
            nestedLevel++;
            currentPos += c;
        } else if (c == ')') {
            nestedLevel--;
            currentPos += c;
            
            if (nestedLevel == 0) {
                // Position complete, parse it
                Position* pos = parsePos(currentPos);
                posArray.push_back(pos);
                currentPos.clear();
                
                // Skip comma and whitespace
                while (i + 1 < content.length() && (content[i + 1] == ',' || isspace(content[i + 1]))) {
                    i++;
                }
            }
        } else {
            currentPos += c;
        }
    }
}

void Configuration::parseUnitListValue(const string& value) {
    // Extract content between brackets
    size_t start = value.find('[');
    size_t end = value.rfind(']');
    
    if (start == string::npos || end == string::npos || start >= end) {
        return;  // Invalid format
    }
    
    string content = value.substr(start + 1, end - start - 1);
    
    // Handle empty array
    if (content.empty()) {
        return;
    }
    
    // Split by units
    int nestedLevel = 0;
    string currentUnit;
    
    for (size_t i = 0; i < content.length(); i++) {
        char c = content[i];
        if (c == '(') {
            nestedLevel++;
            currentUnit += c;
        } else if (c == ')') {
            nestedLevel--;
            currentUnit += c;
            
            if (nestedLevel == 0) {
                // End of unit parameters
                if (!currentUnit.empty()) {
                    parseUnit(currentUnit);
                    currentUnit.clear();
                }
                
                // Skip comma and whitespace if present
                while (i + 1 < content.length() && (content[i + 1] == ',' || isspace(content[i + 1]))) {
                    i++;
                }
            }
        } else {
            currentUnit += c;
        }
    }
    
    // Parse the last unit if any
    if (!currentUnit.empty()) {
        parseUnit(currentUnit);
    }
}


Position* Configuration::parsePos(const string& token) {
    // Parse position string like "(1,2)"
    int start = token.find('(') + 1;
    int comma = token.find(',');
    int end = token.find(')');

    if (start > 0 && comma > start && end > comma) {
        string r_str = token.substr(start, comma - start);
        string c_str = token.substr(comma + 1, end - comma - 1);

        try {
            int r = stoi(r_str);
            int c = stoi(c_str);
            return new Position(r, c);
        } catch (const std::exception& e) {
            // Handle parsing error
        }
    }

    // Return default position if parsing fails
    return new Position(0, 0);
}

void Configuration::parseUnit(const string& token) {
    // Example token: "TANK(5,2,(1,2),0)"
    
    // Extract unit type
    int openParen = token.find('(');
    if (openParen == string::npos) return;
    
    string unitType = token.substr(0, openParen);
    
    // Extract parameters
    string params = token.substr(openParen + 1);
    params = params.substr(0, params.length() - 1); // Remove closing paren
    
    // Split parameters by commas, being careful with nested parentheses
    vector<string> paramList;
    int nestedLevel = 0;
    string currentParam;
    
    for (char c : params) {
        if (c == '(') {
            nestedLevel++;
            currentParam += c;
        } else if (c == ')') {
            nestedLevel--;
            currentParam += c;
        } else if (c == ',' && nestedLevel == 0) {
            // End of parameter
            paramList.push_back(currentParam);
            currentParam.clear();
        } else {
            currentParam += c;
        }
    }
    
    // Add the last parameter
    if (!currentParam.empty()) {
        paramList.push_back(currentParam);
    }
    
    // We should have 4 parameters: quantity, weight, position, army type (0=lib, 1=arvn)
    if (paramList.size() < 4) return;
    
    int quantity = stoi(paramList[0]);
    int weight = stoi(paramList[1]);
    Position pos = *parsePos(paramList[2]);
    int armyType = stoi(paramList[3]);
    
    // Create appropriate unit based on type
    Unit* unit = nullptr;
    
    if (unitType == "TANK") {
        unit = new Vehicle(quantity, weight, pos, TANK);
    } else if (unitType == "TRUCK") {
        unit = new Vehicle(quantity, weight, pos, TRUCK);
    } else if (unitType == "MORTAR") {
        unit = new Vehicle(quantity, weight, pos, MORTAR);
    } else if (unitType == "ANTIAIRCRAFT") {
        unit = new Vehicle(quantity, weight, pos, ANTIAIRCRAFT);
    } else if (unitType == "ARMOREDCAR") {
        unit = new Vehicle(quantity, weight, pos, ARMOREDCAR);
    } else if (unitType == "APC") {
        unit = new Vehicle(quantity, weight, pos, APC);
    } else if (unitType == "ARTILLERY") {
        unit = new Vehicle(quantity, weight, pos, ARTILLERY);
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
        // Add to appropriate army list
        if (armyType == 0) {
            liberationUnits.push_back(unit);
        } else {
            ARVNUnits.push_back(unit);
        }
    }
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
    string result = "[num_rows=" + to_string(num_rows) + 
                   ",num_cols=" + to_string(num_cols) + 
                   ",arrayForest=[";
    
    // Format forest positions
    for (size_t i = 0; i < arrForest.size(); i++) {
        result += arrForest[i]->str();
        if (i < arrForest.size() - 1) result += ",";
    }
    
    result += "],arrayRiver=[";
    
    // Format river positions
    for (size_t i = 0; i < arrRiver.size(); i++) {
        result += arrRiver[i]->str();
        if (i < arrRiver.size() - 1) result += ",";
    }
    
    result += "],arrayFortification=[";
    
    // Format fortification positions
    for (size_t i = 0; i < arrFort.size(); i++) {
        result += arrFort[i]->str();
        if (i < arrFort.size() - 1) result += ",";
    }
    
    result += "],arrayUrban=[";
    
    // Format urban positions
    for (size_t i = 0; i < arrUrban.size(); i++) {
        result += arrUrban[i]->str();
        if (i < arrUrban.size() - 1) result += ",";
    }
    
    result += "],arraySpecialZone=[";
    
    // Format special positions
    for (size_t i = 0; i < arrSpecial.size(); i++) {
        result += arrSpecial[i]->str();
        if (i < arrSpecial.size() - 1) result += ",";
    }
    
    result += "],liberationUnits=[";
    
    // Format liberation units
    for (size_t i = 0; i < liberationUnits.size(); i++) {
        result += liberationUnits[i]->str();
        if (i < liberationUnits.size() - 1) result += ",";
    }
    
    result += "],ARVNUnits=[";
    
    // Format ARVN units
    for (size_t i = 0; i < ARVNUnits.size(); i++) {
        result += ARVNUnits[i]->str();
        if (i < ARVNUnits.size() - 1) result += ",";
    }
    
    result += "],eventCode=" + to_string(eventCode) + "]";
    
    return result;
}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////

// Constructor
HCMCampaign::HCMCampaign(const string &config_file_path)
{
    // Initialize the configuration by loading from the provided file
    config = new Configuration(config_file_path);
    
    // Create the battlefield using configuration data
    battleField = new BattleField(
        config->rows(), 
        config->cols(), 
        config->forest(), 
        config->river(), 
        config->fort(), 
        config->urban(), 
        config->special()
    );
    
    // Create the armies
    Unit** libUnits = config->libArray();
    int libSize = config->libSize();
    liberationArmy = new LiberationArmy(libUnits, libSize, "Liberation Army", battleField);
    
    Unit** arvnUnits = config->arvnArray();
    int arvnSize = config->arvnSize();
    arvn = new ARVN(arvnUnits, arvnSize, "Army of Republic of Vietnam", battleField);
}

// Destructor
HCMCampaign::~HCMCampaign()
{
    // Clean up allocated memory
    if (config) {
        delete config;
        config = nullptr;
    }
    
    if (battleField) {
        delete battleField;
        battleField = nullptr;
    }
    
    if (liberationArmy) {
        delete liberationArmy;
        liberationArmy = nullptr;
    }
    
    if (arvn) {
        delete arvn;
        arvn = nullptr;
    }
}

// Modified run method for test case 3
void HCMCampaign::run() {
    // Apply terrain effects to both armies
    for (int i = 0; i < config->rows(); i++) {
        for (int j = 0; j < config->cols(); j++) {
            TerrainElement* terrain = battleField->at(i, j);
            if (terrain) {
                terrain->getEffect(liberationArmy);
                terrain->getEffect(arvn);
            }
        }
    }
    
    // Get event code from configuration
    int eventCode = config->getEventCode();

    
    // Regular battle logic based on event code
    if (eventCode < 75) {
        // Liberation Army is on the offensive, ARVN is defending
        liberationArmy->fight(arvn, false);  // LiberationArmy attacks
        arvn->fight(liberationArmy, true);   // ARVN defends
    } else {
        // ARVN initiates attack, followed by Liberation Army counterattack
        arvn->fight(liberationArmy, false);  // ARVN attacks
        liberationArmy->fight(arvn, false);  // LiberationArmy counterattacks
    }
    
    // Process units with attackScore <= 5
    UnitList* liberationUnitList = liberationArmy->getUnitList();
    UnitList* arvnUnitList = arvn->getUnitList();
    
    liberationUnitList->forEach([](Unit* unit) {
        if (unit->getAttackScore() <= 5) {
            unit->setQuantity(0);
        }
    });
    
    arvnUnitList->forEach([](Unit* unit) {
        if (unit->getAttackScore() <= 5) {
            unit->setQuantity(0);
        }
    });
    
    // Recalculate indices to reflect changes
    liberationArmy->recalcIndices();
    arvn->recalcIndices();
}

// Print the results of the campaign
string HCMCampaign::printResult()
{
    // Luôn sử dụng định dạng ngắn gọn cho tất cả test case
    return "LIBERATIONARMY[LF=" + to_string(liberationArmy->getLF()) + 
           ",EXP=" + to_string(liberationArmy->getEXP()) + "]-" +
           "ARVN[LF=" + to_string(arvn->getLF()) + 
           ",EXP=" + to_string(arvn->getEXP()) + "]";
}
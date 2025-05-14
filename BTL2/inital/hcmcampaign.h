/*
 * Ho Chi Minh City University of Technology – Assignment 2
 * MOST OF THE LOGIC IS IMPLEMENTED IN hcmcampaign.cpp
 * Do NOT add more #include here (see requirement 5 in pdf).
 */
#ifndef _H_HCM_CAMPAIGN_H_
#define _H_HCM_CAMPAIGN_H_

#include "main.h" // đã khai báo <string>, <vector>, <cmath>, …

// ======================= Forward declarations =======================
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

// ======================= Enumerations ===============================
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

// ======================= POSITION ===================================
class Position
{
    int r, c;

public:
    Position(int r = 0, int c = 0);
    Position(const string &str_pos); // "(x,y)"
    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    string str() const;
};
// ======================= UNIT (abstract) ============================
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
    int getQuantity() const { return quantity; }
    int getWeight() const { return weight; }
    void setQuantity(int newQuantity);
    void setWeight(int newWeight);
    void setRow(int r);
    void setCol(int c);
    void increaseQuantity(int delta) { quantity += delta; }
    void scaleQuantity(double factor) // nhân & làm tròn lên
    {
        quantity = static_cast<int>(std::ceil(quantity * factor));
    }
};
// ======================= VEHICLE & INFANTRY =========================
class Vehicle : public Unit
{
    VehicleType vehicleType;

public:
    Vehicle(int quantity, int weight, const Position pos, VehicleType type);
    VehicleType getType() const;
    int getAttackScore() override;
    string str() const override;
    string vehicleTypeToString(VehicleType type) const;
};

class Infantry : public Unit
{
    InfantryType infantryType;
    bool isPerfectSquare(int x) const;
    int personalNumber(int number) const;

public:
    Infantry(int quantity, int weight, const Position pos, InfantryType type);
    InfantryType getType() const;
    int getAttackScore() override;
    string str() const override;
};
// ======================= UNIT LIST ==================================
class UnitList
{
    struct Node
    {
        Unit *data;
        Node *next;
        Node(Unit *u) : data(u), next(nullptr) {}
    };
    Node *head;
    Node *tail;
    int capacity, sizeV, sizeI, length;
    bool isVehicle(Unit *u) const;
    bool sameVehicle(Node *n, VehicleType t) const;
    bool sameInfantry(Node *n, InfantryType t) const;
    

public:
    UnitList(int capacity);
    ~UnitList();
    bool insert(Unit *unit);                   // true if inserted
    bool isContain(VehicleType vehicleType);   // true if exists
    bool isContain(InfantryType infantryType); // true if exists
    int vehicleCount() const { return sizeV; }
    int infantryCount() const { return sizeI; }
    string str() const;
    // simple traversal helpers
    template <typename F>
    
    void forEach(F func) const
    {
        for (Node *p = head; p; p = p->next)
            func(p->data);
    }
    
    // Replace the removeIf declaration with this template method:
    template <typename Predicate>
    void removeIf(Predicate pred) {
        Node* current = head;
        Node* prev = nullptr;
        
        while (current != nullptr) {
            // Check if we should remove this unit
            if (pred(current->data)) {
                // Update counters
                if (isVehicle(current->data))
                    sizeV--;
                else
                    sizeI--;
                length--;
                
                // Remove node
                Node* toDelete = current;
                
                if (prev == nullptr) {
                    // Removing head node
                    head = current->next;
                    if (head == nullptr) {
                        // List is now empty
                        tail = nullptr;
                    }
                } else {
                    // Removing non-head node
                    prev->next = current->next;
                    if (current == tail) {
                        // Removing tail node
                        tail = prev;
                    }
                }
                
                current = current->next;
                delete toDelete->data;  // Free the unit
                delete toDelete;        // Free the node
            } else {
                // Keep this node
                prev = current;
                current = current->next;
            }
        }
    }
};
// ======================= TERRAIN ====================================
class TerrainElement
{
protected:
    Position pos;

public:
    TerrainElement(Position p) : pos(p) {}
    virtual ~TerrainElement() {}
    virtual void getEffect(Army *army) = 0;
};
class Road : public TerrainElement
{
public:
    using TerrainElement::TerrainElement;
    void getEffect(Army *) override {}
};
class Mountain : public TerrainElement
{
public:
    using TerrainElement::TerrainElement;
    void getEffect(Army *) override;
};
class River : public TerrainElement
{
public:
    using TerrainElement::TerrainElement;
    void getEffect(Army *) override;
};
class Urban : public TerrainElement
{
public:
    using TerrainElement::TerrainElement;
    void getEffect(Army *) override;
};
class Fortification : public TerrainElement
{
public:
    using TerrainElement::TerrainElement;
    void getEffect(Army *) override;
};
// ======================= SPECIAL ZONE: TERRAIN     ====================================

class SpecialZone : public TerrainElement
{
public:
    using TerrainElement::TerrainElement;
    void getEffect(Army *) override;
};

// ======================= BATTLE FIELD ===============================
class BattleField
{
    int n_rows, n_cols;
    vector<vector<TerrainElement *>> terrain; 
public:
    BattleField(int n_rows, int n_cols,
                vector<Position *> forest,
                vector<Position *> river,
                vector<Position *> fortification,
                vector<Position *> urban,
                vector<Position *> special);
    ~BattleField();
    string str() const;
    TerrainElement *at(int r, int c) const;
};
// ======================= ARMY (abstract) ============================
class Army
{
protected:
    int LF, EXP;
    string name;
    UnitList *unitList;
    BattleField *battleField;

    static bool isSpecial(int S); // helper for capacity (pdf 3.5)

public:
    void recalcIndices();         // recompute LF & EXP from list
    UnitList* getUnitList() { return unitList; }
    Army(Unit **unitArray, int size, string name, BattleField *bf);
    virtual ~Army();
    virtual void fight(Army *enemy, bool defense = false) = 0;
    virtual string str() const = 0;
    int getLF() const { return LF; }
    int getEXP() const { return EXP; }
};
// ---------------- LiberationArmy & ARVN -----------------------------
class LiberationArmy : public Army {
public:
    LiberationArmy(Unit** unitArray, int size, string name, BattleField* bf);
    void fight(Army* enemy, bool defense = false) override;
    void confiscate(Army* enemy);
    string str() const override;

private:
    void reduceWeights();
    void reduceQuantities();
    void increaseQuantitiesToFibonacci();
};
class ARVN : public Army
{
public:
    ARVN(Unit **unitArray, int size, string name, BattleField *bf);
    void fight(Army *enemy, bool defense = false) override;
    string str() const override;
};
// ======================= CONFIGURATION ==============================
class Configuration
{
    int num_rows, num_cols;
    vector<Position *> arrForest, arrRiver, arrFort, arrUrban, arrSpecial;
    vector<Unit *> liberationUnits, ARVNUnits;
    int eventCode;
    void parseUnit(const string &token);
    Position *parsePos(const string &token);

    void parsePositionArray(const string &value, vector<Position*> &posArray);
    void parseUnitListValue(const string &value);

public:
    Configuration(const string &filepath);
    ~Configuration();
    int rows() const { return num_rows; }
    int cols() const { return num_cols; }
    vector<Position *> &forest() { return arrForest; }
    vector<Position *> &river() { return arrRiver; }
    vector<Position *> &fort() { return arrFort; }
    vector<Position *> &urban() { return arrUrban; }
    vector<Position *> &special() { return arrSpecial; }
    Unit **libArray();
    int libSize() const;
    Unit **arvnArray();
    int arvnSize() const;
    int getEventCode() const { return eventCode; }
    string str() const;
};
// ======================= CAMPAIGN ===================================
class HCMCampaign
{
    Configuration *config;
    BattleField *battleField;
    LiberationArmy *liberationArmy;
    ARVN *arvn;

public:
    HCMCampaign(const string &config_file_path);
    ~HCMCampaign();
    void run();
    string printResult();
};

#endif

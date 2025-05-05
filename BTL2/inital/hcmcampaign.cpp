#include "hcmcampaign.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <iomanip> // for ceil in scores

// ======== Position ==================================================
Position::Position(int r, int c) : r(r), c(c) {}
Position::Position(const string &s)
{
    sscanf(s.c_str(), "(%d,%d)", &r, &c);
}
int Position::getRow() const { return r; }
int Position::getCol() const { return c; }
void Position::setRow(int _r) { r = _r; }
void Position::setCol(int _c) { c = _c; }
string Position::str() const
{
    stringstream ss;
    ss << "(" << r << "," << c << ")";
    return ss.str();
}

// ======== Unit base =================================================
Unit::Unit(int q, int w, Position p) : quantity(q), weight(w), pos(p) {}
Unit::~Unit() = default;
Position Unit::getCurrentPosition() const { return pos; }

// ======== Vehicle ===================================================
Vehicle::Vehicle(int q, int w, const Position p, VehicleType t) : Unit(q, w, p), vehicleType(t) {}
VehicleType Vehicle::getType() const { return vehicleType; }
int Vehicle::getAttackScore()
{
    long long base = (static_cast<int>(vehicleType)) * 304LL + quantity * weight;
    return static_cast<int>(ceil(base / 30.0));
}
string Vehicle::str() const
{
    stringstream ss;
    ss << "Vehicle[vehicleType=" << vehicleType << ",quantity=" << quantity
       << ",weight=" << weight << ",pos=" << pos.str() << "]";
    return ss.str();
}

// ======== Infantry ==================================================
bool Infantry::isPerfectSquare(int x) const
{
    int r = static_cast<int>(sqrt(x));
    return r * r == x;
}
int Infantry::personalNumber(int num) const
{
    auto digitSum = [](int x)
    {int s=0;while(x){s+=x%10;x/=10;}return s; };
    int y = digitSum(num) + digitSum(1975);
    while (y >= 10)
        y = digitSum(y);
    return y;
}
Infantry::Infantry(int q, int w, const Position p, InfantryType t) : Unit(q, w, p), infantryType(t) {}
InfantryType Infantry::getType() const { return infantryType; }
int Infantry::getAttackScore()
{
    int score = static_cast<int>(infantryType) * 56 + quantity * weight;
    if (infantryType == SPECIALFORCES && isPerfectSquare(weight))
        score += 75;
    int pn = personalNumber(score);
    if (pn > 7)
    {
        quantity = static_cast<int>(ceil(quantity * 1.2));
        score = static_cast<int>(infantryType) * 56 + quantity * weight;
    }
    else if (pn < 3)
    {
        quantity = static_cast<int>(ceil(quantity * 0.9));
        score = static_cast<int>(infantryType) * 56 + quantity * weight;
    }
    return score;
}
string Infantry::str() const
{
    stringstream ss;
    ss << "Infantry[infantryType=" << infantryType << ",quantity=" << quantity
       << ",weight=" << weight << ",pos=" << pos.str() << "]";
    return ss.str();
}

// ======== UnitList ==================================================
bool UnitList::isVehicle(Unit *u) const
{
    return dynamic_cast<Vehicle *>(u) != nullptr;
}
UnitList::UnitList(int cap) : head(nullptr), tail(nullptr), capacity(cap), sizeV(0), sizeI(0), length(0) {}
UnitList::~UnitList()
{
    Node *p = head;
    while (p)
    {
        Node *t = p->next;
        delete p->data;
        delete p;
        p = t;
    }
}
bool UnitList::sameVehicle(Node *n, VehicleType t) const
{
    auto *v = dynamic_cast<Vehicle *>(n->data);
    return v && v->getType() == t;
}
bool UnitList::sameInfantry(Node *n, InfantryType t) const
{
    auto *i = dynamic_cast<Infantry *>(n->data);
    return i && i->getType() == t;
}
bool UnitList::insert(Unit *u)
{
    if (length >= capacity)
        return false;
    // check duplication
    if (isVehicle(u))
    {
        Vehicle *v = dynamic_cast<Vehicle *>(u);
        for (Node *p = head; p; p = p->next)
        {
            if (sameVehicle(p, v->getType()))
            {
                auto *ov = dynamic_cast<Vehicle *>(p->data);
                ov->increaseQuantity(v->getQuantity());
                delete v;
                return true;
            }
        }
    }
    else
    {
        Infantry *i = dynamic_cast<Infantry *>(u);
        for (Node *p = head; p; p = p->next)
        {
            if (sameInfantry(p, i->getType()))
            {
                auto *oi = dynamic_cast<Infantry *>(p->data);
                oi->increaseQuantity(i->getQuantity());
                delete i;
                return true;
            }
        }
    }
    Node *node = new Node(u);
    if (isVehicle(u))
    { // add to tail
        if (!tail)
            head = tail = node;
        else
        {
            tail->next = node;
            tail = node;
        }
        ++sizeV;
    }
    else
    { // add to head
        node->next = head;
        head = node;
        if (!tail)
            tail = head;
        ++sizeI;
    }
    ++length;
    return true;
}
bool UnitList::isContain(VehicleType t)
{
    for (Node *p = head; p; p = p->next)
        if (sameVehicle(p, t))
            return true;
    return false;
}
bool UnitList::isContain(InfantryType t)
{
    for (Node *p = head; p; p = p->next)
        if (sameInfantry(p, t))
            return true;
    return false;
}
string UnitList::str() const
{
    stringstream ss;
    ss << "UnitList[count_vehicle=" << sizeV << ";count_infantry=" << sizeI << ";";
    Node *p = head;
    while (p)
    {
        ss << p->data->str();
        if (p->next)
            ss << ",";
        p = p->next;
    }
    ss << "]";
    return ss.str();
}

// ======== BattleField ===============================================
BattleField::BattleField(int r, int c, vector<Position *> forest, vector<Position *> river,
                         vector<Position *> fort, vector<Position *> urban, vector<Position *> special)
    : n_rows(r), n_cols(c)
{
    terrain.assign(n_rows, vector<TerrainElement *>(n_cols, nullptr));
    auto place = [this](vector<Position *> &arr, auto maker)
    {
        for (auto *p : arr)
        {
            if (p->getRow() < n_rows && p->getCol() < n_cols)
            {
                delete terrain[p->getRow()][p->getCol()];
                terrain[p->getRow()][p->getCol()] = maker(*p);
            }
        }
    };
    place(forest, [](Position p)
          { return new Mountain(p); });
    place(river, [](Position p)
          { return new River(p); });
    place(fort, [](Position p)
          { return new Fortification(p); });
    place(urban, [](Position p)
          { return new Urban(p); });
    place(special, [](Position p)
          { return new SpecialZone(p); });
}
BattleField::~BattleField()
{
    for (auto &row : terrain)
        for (auto *t : row)
            delete t;
}
TerrainElement *BattleField::at(int r, int c) const
{
    if (r < 0 || c < 0 || r >= n_rows || c >= n_cols)
        return nullptr;
    return terrain[r][c];
}
string BattleField::str() const
{
    stringstream ss;
    ss << "BattleField[n_rows=" << n_rows << ",n_cols=" << n_cols << "]";
    return ss.str();
}

// ======== Terrain effects (simple placeholder) ======================
void Mountain::getEffect(Army *army) { /* see pdf table */ }
void River::getEffect(Army *army) { /* reduce infantry attack 10% */ }
void Urban::getEffect(Army *army) { /* complex rules – omitted here */ }
void Fortification::getEffect(Army *army) { /* */ }
void SpecialZone::getEffect(Army *army) { /* */ }

// ======== Army helper ===============================================
bool Army::isSpecial(int S)
{
    auto check = [S](int base)
    {
        int x = S;
        while (x)
        {
            if (x % base > 1)
                return false;
            x /= base;
        }
        return true;
    };
    for (int p : {3, 5, 7})
        if (check(p))
            return true;
    return false;
}
void Army::recalcIndices()
{
    LF = EXP = 0;
    unitList->forEach([&](Unit *u)
                      {
        if(dynamic_cast<Vehicle*>(u)) LF+=u->getAttackScore();
        else EXP+=u->getAttackScore(); });
    if (LF > 1000)
        LF = 1000;
    if (EXP > 500)
        EXP = 500;
}
Army::Army(Unit **arr, int sz, string n, BattleField *bf) : name(std::move(n)), battleField(bf)
{
    LF = EXP = 0;
    // temporary list to compute capacity
    for (int i = 0; i < sz; ++i)
    {
        if (dynamic_cast<Vehicle *>(arr[i]))
            LF += arr[i]->getAttackScore();
        else
            EXP += arr[i]->getAttackScore();
    }
    int S = LF + EXP;
    int cap = isSpecial(S) ? 12 : 8;
    unitList = new UnitList(cap);
    for (int i = 0; i < sz; ++i)
        unitList->insert(arr[i]);
    recalcIndices();
}
Army::~Army() { delete unitList; }

// ======== LiberationArmy ============================================
LiberationArmy::LiberationArmy(Unit **arr, int sz, string n, BattleField *bf)
    : Army(arr, sz, n, bf) {}
void LiberationArmy::confiscate(Army *enemy)
{
    // move all enemy units into own list
    // (simple approach: iterate enemy unitList and insert to this->unitList)
}
void LiberationArmy::fight(Army *enemy, bool defense)
{
    if (!defense)
    { // attack
        LF = static_cast<int>(ceil(LF * 1.5));
        EXP = static_cast<int>(ceil(EXP * 1.5));
        // simplified win condition: if LF>enemy->getLF() && EXP>enemy->getEXP()
        if (LF >= enemy->getLF() || EXP >= enemy->getEXP())
        {
            confiscate(enemy);
        }
    }
    else
    { // defend
        LF = static_cast<int>(ceil(LF * 1.3));
        EXP = static_cast<int>(ceil(EXP * 1.3));
    }
}
string LiberationArmy::str() const
{
    stringstream ss;
    ss << "LiberationArmy[name=" << name << ",LF=" << LF << ",EXP=" << EXP
       << "," << unitList->str() << "," << battleField->str() << "]";
    return ss.str();
}

// ======== ARVN ======================================================
ARVN::ARVN(Unit **arr, int sz, string n, BattleField *bf) : Army(arr, sz, n, bf) {}
void ARVN::fight(Army *enemy, bool defense)
{
    if (!defense)
    { // ARVN tấn công
        unitList->forEach([](Unit *u)
                          {
                              u->scaleQuantity(0.8); // ✅ dùng hàm mới
                          });
        recalcIndices();
    }
    else
    { // defend
        // if lost -> confiscated handled by enemy
    }
}
string ARVN::str() const
{
    stringstream ss;
    ss << "ARVN[name=" << name << ",LF=" << LF << ",EXP=" << EXP
       << "," << unitList->str() << "," << battleField->str() << "]";
    return ss.str();
}

// ======== Configuration =============================================
Position *Configuration::parsePos(const string &t)
{
    return new Position(t);
}
void Configuration::parseUnit(const string &token)
{
    // very simple parser supporting e.g. TANK(5,2,(1,2),0)
    stringstream ss(token);
    string name;
    getline(ss, name, '(');
    int q, w, bel;
    char ch;
    string posStr;
    ss >> q;
    ss.ignore(1, ',');
    ss >> w;
    ss.ignore(1, ',');
    getline(ss, posStr, ')');
    ss >> ch;
    ss.ignore(1, ',');
    ss >> bel;
    Position *p = new Position(posStr);
    Unit *u = nullptr;
    if (name == "TANK")
        u = new Vehicle(q, w, *p, TANK);
    else if (name == "REGULARINFANTRY")
        u = new Infantry(q, w, *p, REGULARINFANTRY);
    //  extend for other names
    if (bel == 0)
        liberationUnits.push_back(u);
    else
        ARVNUnits.push_back(u);
}
Configuration::Configuration(const string &path)
{
    ifstream fi(path);
    string line;
    while (getline(fi, line))
    {
        if (line.find("NUM_ROWS=") == 0)
            num_rows = stoi(line.substr(9));
        else if (line.find("NUM_COLS=") == 0)
            num_cols = stoi(line.substr(9));
        else if (line.find("ARRAY_FOREST=[") == 0)
        {
            size_t l = line.find('[') + 1, r = line.find(']');
            string arr = line.substr(l, r - l);
            stringstream ss(arr);
            string tok;
            while (getline(ss, tok, ')'))
            {
                if (tok.empty())
                    continue;
                tok.push_back(')');
                arrForest.push_back(parsePos(tok));
                ss.ignore(1, ',');
            }
        }
        // analogous parsing for other arrays (omitted for brevity)
        else if (line.find("UNIT_LIST=[") == 0)
        {
            size_t l = line.find('[') + 1, r = line.find(']');
            string arr = line.substr(l, r - l);
            stringstream ss(arr);
            string tok;
            while (getline(ss, tok, ')'))
            {
                if (tok.empty())
                    continue;
                tok.push_back(')');
                parseUnit(tok);
                ss.ignore(1, ',');
            }
        }
        else if (line.find("EVENT_CODE=") == 0)
        {
            eventCode = stoi(line.substr(11)) % 100;
        }
    }
}
Configuration::~Configuration()
{
    for (auto p : arrForest)
        delete p;
    for (auto p : arrRiver)
        delete p;
    for (auto p : arrFort)
        delete p;
    for (auto p : arrUrban)
        delete p;
    for (auto p : arrSpecial)
        delete p;
    for (auto u : liberationUnits)
        delete u;
    for (auto u : ARVNUnits)
        delete u;
}
Unit **Configuration::libArray() { return liberationUnits.data(); }
Unit **Configuration::arvnArray() { return ARVNUnits.data(); }
int Configuration::libSize() const { return (int)liberationUnits.size(); }
int Configuration::arvnSize() const { return (int)ARVNUnits.size(); }
string Configuration::str() const
{
    stringstream ss;
    ss << "Configuration[num_rows=" << num_rows << ",num_cols=" << num_cols << "]";
    return ss.str();
}

// ======== HCMCampaign ==============================================
HCMCampaign::HCMCampaign(const string &cfg)
{
    config = new Configuration(cfg);
    battleField = new BattleField(config->rows(), config->cols(),
                                  config->forest(), config->river(),
                                  config->fort(), config->urban(), config->special());
    liberationArmy = new LiberationArmy(config->libArray(), config->libSize(), "Liberation", battleField);
    arvn = new ARVN(config->arvnArray(), config->arvnSize(), "ARVN", battleField);
}
HCMCampaign::~HCMCampaign()
{
    delete liberationArmy;
    delete arvn;
    delete battleField;
    delete config;
}
void HCMCampaign::run()
{
    int ec = config->getEventCode();
    if (ec < 75)
    {
        liberationArmy->fight(arvn, false); // LA attacks
    }
    else
    {
        arvn->fight(liberationArmy, false); // ARVN attacks
        liberationArmy->fight(arvn, false); // counter attack
    }
}
string HCMCampaign::printResult()
{
    stringstream ss;
    ss << "LIBERATIONARMY[LF=" << liberationArmy->getLF() << ",EXP=" << liberationArmy->getEXP()
       << "]-ARVN[LF=" << arvn->getLF() << ",EXP=" << arvn->getEXP() << "]";
    return ss.str();
}

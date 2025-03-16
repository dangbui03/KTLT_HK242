#include "tay_nguyen_campaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// 2153289 - Bùi Hồ Hải Đăng
const int MAX_LINES = 5;         
const int MAX_LINE_LENGTH = 100;

// Task 0: Read input file
void checkT (int &T1, int &T2);
void checkE (int &E);
void checkEXP (int &EXP1, int &EXP2);
void checkArr (int arr[], int n);

bool readFile(
    const string &filename,
    int LF1[], int LF2[],
    int &EXP1, int &EXP2,
    int &T1, int &T2, int &E)
{
  char data[MAX_LINES][MAX_LINE_LENGTH];
  int numLines = 0;

  ifstream ifs(filename);
  if (!ifs.is_open())
  {
    return false;
  }

  while (numLines < MAX_LINES && ifs.getline(data[numLines], MAX_LINE_LENGTH))
  {
    numLines++;
  }
  ifs.close();

  if (numLines < MAX_LINES)
  {
    return false;
  }

  // TODO: Extract values from the `data` array and store them in respective variables
  string firstline = string(data[0]).substr(1, strlen(data[0]) - 2);
  string secondline = string(data[1]).substr(1, strlen(data[1]) - 2);
  stringstream ss1(firstline);
  stringstream ss2(secondline);

  string token;
  int i = 0;
  while (getline(ss1, token, ','))
  {
      LF1[i++] = stoi(token); // Convert string to integer and store in LF1
  }

  i = 0;
  while (getline(ss2, token, ','))
  {
      LF2[i++] = stoi(token); // Convert string to integer and store in LF2
  }

  // Process the third line into EXP1 and EXP2
  stringstream ss3(data[2]);
  ss3 >> EXP1 >> EXP2;

  // Process the fourth line into T1 and T2
  stringstream ss4(data[3]);
  ss4 >> T1 >> T2;

  // Process the fifth line into E
  stringstream ss5(data[4]);
  ss5 >> E;

  checkT(T1, T2);
  checkE(E);
  checkEXP(EXP1, EXP2);
  checkArr(LF1, 17);
  checkArr(LF2, 17);
  return true;
}

void checkT (int &T1, int &T2) {
  if (T1 > 3000) T1 = 3000;
  if (T1 < 0) T1 = 0;
  if (T2 > 3000) T2 = 3000;
  if (T2 < 0) T2 = 0;
}

void checkE (int &E) {
  if (E > 99) E = 99;
  if (E < 0) E = 0;
}

void checkEXP (int &EXP1, int &EXP2) {
  if (EXP1 > 600) EXP1 = 600;
  if (EXP1 < 0) EXP1 = 0;
  if (EXP2 > 600) EXP2 = 600;
  if (EXP2 < 0) EXP2 = 0;
}

void checkArr (int arr[], int n) {
  for (int i = 0; i < n; i++) {
    if (arr[i] < 0) arr[i] = 0;
    if (arr[i] > 1000) arr[i] = 1000;
  }
}

// Task 1
int gatherForces(int LF1[], int LF2[])
{
  // TODO: Implement this function
  int MilitaryForceStrength[17] = {1, 2, 3, 4, 5, 7, 8, 9, 10, 12, 15, 18, 20, 30, 40, 50, 70};

  int sumLF1 = 0, sumLF2 = 0;
  for (int i = 0; i < 17; i ++)
  {
    if (LF1[i] < 0) LF1[i] = 0;
    if (LF2[i] < 0) LF2[i] = 0;
    if (LF1[i] > 1000) LF1[i] = 1000;
    if (LF2[i] > 1000) LF2[i] = 1000;
    
    sumLF1 += LF1[i] * MilitaryForceStrength[i];
    sumLF2 += LF2[i] * MilitaryForceStrength[i];
  }

  // cout << "LF1: " << sumLF1 << endl;
  // cout << "LF2: " << sumLF2 << endl;

  return sumLF1 + sumLF2;
}

string checkID (const int ID) {
  if (ID >= 0 && ID <= 2) {
    return "DECOY";
  } else {
    switch (ID) {
      case 3:
        return "Buon Ma Thuot";
        break;
      case 4:
        return "Duc Lap";
        break;
      case 5:
        return "Dak Lak";
        break;
      case 6:
        return "National Route 21";
        break;
      case 7:
        return "National Route 14";
        break;
    }
  } 
  return "INVALID";
}

void Sort(int arr[], int n) {
  bool swapped;
  for (int i = 0; i < n - 1; i++) {
      swapped = false;
      for (int j = 0; j < n - i - 1; j++) {
          if (arr[j] > arr[j + 1]) {
              // Swap elements if they are in the wrong order
              swap(arr[j], arr[j + 1]);
              swapped = true;
          }
      }
      // If no swaps happened, the array is already sorted
      if (!swapped)
          break;
  }
}

// Task 2.1
string determineRightTarget(const string &target) {
  // Todo
  int ID_Arr[100] = {0};
  int length = target.length();

  int j = 0, k = 0;
  for (int i = 0; i < length; i++)
  {
    if(isdigit(target[i]))
    {
      // int check3Letter = 1;
      string temp = "";
      k = i+1;
      temp += target[i];
      while(isdigit(target[k]) && k < length)
      {
        // if (check3Letter == 3) break;
        // check3Letter++;
        temp += target[k];
        k++;
        if (stoi(temp) > 100) break;
      }
      ID_Arr[j] += stoi(temp);
      j++;
      i = k - 1;
    }
  }
  // cout << j << endl;

  if (j == 1) {
    return checkID(ID_Arr[0]);
  } else if (j == 2) {
      int ID = ((ID_Arr[0] + ID_Arr[1]) % 5 ) + 3;
      return checkID(ID);
  } else if (j == 3) {
      Sort(ID_Arr, 3);
      int ID = (ID_Arr[2] % 5 ) + 3;
      // cout << ID << endl;
      return checkID(ID);
  }
  return "INVALID";
}

string reverseString(const string& message) {
  string reversed = message;
  for (int i = 0, j = message.length() - 1; i < j; i++, j--) {
      swap(reversed[i], reversed[j]);
  }
  return reversed;
}

string caesarCipherDecrypt(const string& message, int shift) {
  string decrypted = "";
  for (int i = 0; i < message.length(); i++) {
      char c = message[i];
      if (islower(c)) {
          decrypted += (char)(((c - 'a' + shift) % 26) + 'a');
      } else if (isupper(c)) {
          decrypted += (char)(((c - 'A' + shift) % 26) + 'A');
      } else {
          decrypted += c;
      }
  }
  return decrypted;
}

// Task 2.2
string decodeTarget(const string &message, int EXP1, int EXP2) {
  // Todo
  checkEXP(EXP1, EXP2);
  
  string decodedMessage;
  if (EXP1 >= 300 && EXP2 >= 300) {
    int shift = (EXP1 + EXP2) % 26;
    decodedMessage = caesarCipherDecrypt(message, shift);
  } else if (EXP1 < 300 || EXP2 < 300) {
    decodedMessage = reverseString(message);
  }

  string a[5] = {"Buon Ma Thuot", "Duc Lap", "Dak Lak", "National Route 21", "National Route 14"};
  string b[5] = {"buon ma thuot", "duc lap", "dak lak", "national route 21", "national route 14"};
  
  // Compare with the target
  string lowerDecodedMessage = "";
  for (int i = 0; i < decodedMessage.length(); i++) {
    lowerDecodedMessage += tolower(decodedMessage[i]);
  }
  for (int i = 0; i < 5; i++) {
    if (lowerDecodedMessage == b[i]) {
      return a[i];
      break;
    }
  }
  return "INVALID";
}

// Task 3
void manageLogistics(int LF1, int LF2, int EXP1, int EXP2, int &T1, int &T2, int E)
{
  // TODO: Implement this function
  if (LF1 < 0) LF1 = 0;
  if (LF2 < 0) LF2 = 0;
  
  checkT(T1, T2);
  checkE(E);
  checkEXP(EXP1, EXP2);

  if (E == 0) {
    double delta_T1 = (((double)LF1 / (double(LF1) + double(LF2))) * (double)(T1 + T2)) * (double(1) + (double)((EXP1 - EXP2) / double(100)));
    double delta_T2 = (double)(T1 + T2) - delta_T1;
    T1 = (int)ceil((double)T1 + delta_T1);
    T2 = (int)ceil((double)T2 + delta_T2);

    checkT(T1, T2);
  } else if (E >= 1 && E <= 9) {
    T1 = (int)ceil(T1 - E * 0.01 * T1);
    T2 = (int)ceil(T2 - E * 0.005 * T2);
    
    checkT(T1, T2);
  } else if (E >= 10 && E <= 29) {
    T1 = (int)ceil(T1 + E * 50);
    T2 = (int)ceil(T2 + E * 50);
    
    checkT(T1, T2);
  } else if (E >= 30 && E <= 59) {
    T1 = (int)ceil(T1 + E * 0.005 * T1);
    T2 = (int)ceil(T2 + E * 0.002 * T2);

    checkT(T1, T2);
  }

  checkT(T1, T2);
}

// Task 4
int planAttack(int LF1, int LF2, int EXP1, int EXP2, int T1, int T2, int battleField[10][10])
{
  // TODO: Implement this function
  if (LF1 < 0) LF1 = 0;
  if (LF2 < 0) LF2 = 0;
  
  checkT(T1, T2);
  checkEXP(EXP1, EXP2);

  float S = (LF1 + LF2) + (EXP1 + EXP2) * 5 + (T1 + T2) * 2;
  for (int i = 0; i < 10; i++)
  {
    for (int j = 0; j < 10; j++)
    {
      if (i % 2 == 1) {
        S -= ((float)battleField[i][j] * 1.5);
      } else {
        S -= ((float)battleField[i][j] * float(2) / float(3));
      }
    }
  }
  return (int)ceil(S);
}

// Task 5
int resupply(int shortfall, int supply[5][5])
{
  // TODO: Implement this function
  int one_demension_arr [25] = {0};

  for (int i = 0; i < 5; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      // if (supply[i][j] < 0) supply[i][j] = 0;
      one_demension_arr[i * 5 + j] = supply[i][j];
    }
  }

  Sort(one_demension_arr, 25);
  int bestSum = INT_MAX; // one_demension_arr[0] + one_demension_arr[1] + one_demension_arr[2] + one_demension_arr[3] + one_demension_arr[4];

  for (int m = 24; m >= 4; m--) {
      for (int l = m - 1; l >= 3; l--) {
          for (int k = l - 1; k >= 2; k--) {
              for (int j = k - 1; j >= 1; j--) {
                  for (int i = j - 1; i >= 0; i--) {
                      int sum = one_demension_arr[i] + one_demension_arr[j] + one_demension_arr[k] + one_demension_arr[l] + one_demension_arr[m];
                      if (sum >= shortfall && sum < bestSum) {
                          bestSum = sum;
                      }
                  }
              }
          }
      }
  }

  return bestSum;
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////

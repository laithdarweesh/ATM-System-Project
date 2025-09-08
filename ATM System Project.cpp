#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <fstream>

using namespace std;

const string ClientFileName = "Clients_Data.txt";

struct stClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double Balance;
    bool MarkForDelete = false;
};

enum enATMMenueOption
{
    enQuickWithdraw = 1, enNormalWithdraw = 2, enDeposit = 3, enCheckBalance = 4, enLogout = 5
};

stClient CurrentClient;

void Login();
void ShowATMMenueScreen();
void ShowQuickWithdrawScreen();
void ShowNormalWithdrawScreen();

void ReturnToATMMenuScreen()
{
    cout << "\n\n\npress any key to go back to ATM Menu Screen...";
    system("pause > 0");
    ShowATMMenueScreen();
}

void ShowLoginScreen()
{
    cout << "\n------------------------------------\n";
    cout << "\tLogin Screen\n";
    cout << "------------------------------------\n";
}

void ShowCheckBalanceScreen()
{
    cout << "===========================================\n";
    cout << "\t\tCheck Balance Screen\n";
    cout << "===========================================\n";

    cout << "Your Balance is " << CurrentClient.Balance << endl;
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "\nEnter Account Number? ";
    cin >> AccountNumber;

    return AccountNumber;
}

string ReadClientPinCode()
{
    string PinCode = "";
    cout << "Enter PinCode? ";
    cin >> PinCode;

    return PinCode;
}

short ReadATMMainMenueOption()
{
    short Choose;
    cout << "Choose what do you want to do? [1 To 5]? ";
    cin >> Choose;

    return Choose;
}

double ReadDepositAmount()
{
    double DepositBalance = 0;

    while (DepositBalance <= 0)
    {
        cout << "\nEnter a positive Deposit Amount? ";
        cin >> DepositBalance;
    }

    return DepositBalance;
}

int ReadNormalWithdraw()
{
    int NormalWithdraw;
    cout << "\nEnter an amount multiple of 5'? ";
    cin >> NormalWithdraw;

    return NormalWithdraw;
}

short ReadQuickWithdrawOption()
{
    short Choice = 0;

    while (Choice < 1 || Choice > 9)
    {
        cout << "Choose what to do from [1] To [9]? ";
        cin >> Choice;
    }

    return Choice;
}

vector<string> SpliteString(string Str, string Delim)
{
    vector<string> VString;
    string Word = "";
    short Pos = 0;

    while ((Pos = Str.find(Delim)) != std::string::npos)
    {
        Word = Str.substr(0, Pos);

        if (Word != "")
        {
            VString.push_back(Word);
        }

        Str.erase(0, Pos + Delim.length());
    }

    if (Str != "")
    {
        VString.push_back(Str);
    }

    return VString;
}

string ConvertClientsRecordToLineData(stClient Client, string Seperator = "#//#")
{
    string ClientData = "";

    ClientData += Client.AccountNumber + Seperator;
    ClientData += Client.PinCode + Seperator;
    ClientData += Client.Name + Seperator;
    ClientData += Client.Phone + Seperator;
    ClientData += to_string(Client.Balance) + Seperator;

    return ClientData.substr(0, ClientData.length() - Seperator.length());
}

stClient ConvertClientDataToRecord(string LineData)
{
    vector<string> VString = SpliteString(LineData, "#//#");
    stClient Client;

    Client.AccountNumber = VString[0];
    Client.PinCode = VString[1];
    Client.Name = VString[2];
    Client.Phone = VString[3];
    Client.Balance = stoi(VString[4]);

    return Client;
}

void SaveClientsDataToFile(string FileName, vector<stClient> VClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);

    if (MyFile.is_open())
    {
        string Line;

        for (stClient Client : VClients)
        {
            Line = ConvertClientsRecordToLineData(Client);
            MyFile << Line << endl;
        }

        MyFile.close();
    }
}

vector<stClient> LoadClientDataFromFile(string FileName)
{
    vector<stClient> VClient;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        stClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertClientDataToRecord(Line);
            VClient.push_back(Client);
        }

        MyFile.close();
    }

    return VClient;
}

bool IsAmountExceedsCurrentBalance(double WithdrawAmount)
{
    return (WithdrawAmount > CurrentClient.Balance) ? true : false;
}

bool IsWithdrawAmountMultipleOf5(int WithdrawAmount)
{
    return (WithdrawAmount % 5 == 0) ? true : false;
}

void PerformWithdrawAmount(double WithdrawAmount, bool DepositAmount = false, bool IsForNormalWithdraw = false)
{
    while (IsAmountExceedsCurrentBalance(WithdrawAmount) && !DepositAmount)
    {
        cout << "\nThe amount exceeds your balance, make another choise.";
        cout << "\nPress AnyKek to continue...";
        system("pause > 0");
        system("cls");

        if (IsForNormalWithdraw)
        {
            ShowNormalWithdrawScreen();
        }
        else
        {
            ShowQuickWithdrawScreen();
        }
        return;
    }

    char Answer = 'n';
    cout << "\n\nAre you sure you want perform this transaction? y/n? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        vector<stClient> VClients = LoadClientDataFromFile(ClientFileName);

        for (stClient& Client : VClients)
        {
            if (Client.AccountNumber == CurrentClient.AccountNumber)
            {
                Client.Balance -= WithdrawAmount;
                CurrentClient.Balance = Client.Balance;
                break;
            }
        }

        SaveClientsDataToFile(ClientFileName, VClients);
        cout << "\nDone Successfully. New Balance is: " << CurrentClient.Balance;
    }
}

void PerformDepositAmount()
{
    double DepositBalance = ReadDepositAmount();
    PerformWithdrawAmount(DepositBalance * -1, true);
}

void ShowDepositScreen()
{
    cout << "===========================================\n";
    cout << "\t\tDeposit Screen\n";
    cout << "===========================================\n";

    cout << "Your Balance is " << CurrentClient.Balance;
    PerformDepositAmount();
}

void PerformNormalWithdrawAmount()
{
    int WithDrawBalance = ReadNormalWithdraw();

    while (!IsWithdrawAmountMultipleOf5(WithDrawBalance))
    {
        WithDrawBalance = ReadNormalWithdraw();
    }

    PerformWithdrawAmount(WithDrawBalance, false, true);
}

void ShowNormalWithdrawScreen()
{
    cout << "===========================================\n";
    cout << "\t\tNormal Withdraw Screen\n";
    cout << "===========================================\n";

    cout << "Your Balance is " << CurrentClient.Balance;
    PerformNormalWithdrawAmount();
}

short GetQuickWithdrawAmount(short QuickWithDrawOption)
{
    switch (QuickWithDrawOption)
    {
    case 1:
        return 20;
    case 2:
        return 50;
    case 3:
        return 100;
    case 4:
        return 200;
    case 5:
        return 400;
    case 6:
        return 600;
    case 7:
        return 800;
    case 8:
        return 1000;
    default:
        return 0;
    }
}

void PerformQuickWithdrawOption(short QuickWithdtrawOption)
{
    if (QuickWithdtrawOption == 9)
        return;

    short WithDrawBalance = GetQuickWithdrawAmount(QuickWithdtrawOption);
    PerformWithdrawAmount(WithDrawBalance);
}

void ShowQuickWithdrawScreen()
{
    cout << "===========================================\n";
    cout << "\t\tQuick Withdraw\n";
    cout << "===========================================\n";

    cout << "\t[1] 20\t\t[2] 50\n";
    cout << "\t[3] 100\t\t[4] 200\n";
    cout << "\t[5] 400\t\t[6] 600\n";
    cout << "\t[7] 800\t\t[8] 1000\n";
    cout << "\t[9] Exist\n";
    cout << "===========================================\n";

    cout << "Your Balance is " << CurrentClient.Balance << endl;
    PerformQuickWithdrawOption(ReadQuickWithdrawOption());
}

void PerformATMMenueOption(enATMMenueOption Option)
{
    switch (Option)
    {
    case enATMMenueOption::enQuickWithdraw:
        system("cls");
        ShowQuickWithdrawScreen();
        ReturnToATMMenuScreen();
        break;
    case enATMMenueOption::enNormalWithdraw:
        system("cls");
        ShowNormalWithdrawScreen();
        ReturnToATMMenuScreen();
        break;
    case enATMMenueOption::enDeposit:
        system("cls");
        ShowDepositScreen();
        ReturnToATMMenuScreen();
        break;
    case enATMMenueOption::enCheckBalance:
        system("cls");
        ShowCheckBalanceScreen();
        ReturnToATMMenuScreen();
        break;
    case enATMMenueOption::enLogout:
        system("cls");
        Login();
        break;
    }
}

void ShowATMMenueScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\tATM Main Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Logout.\n";
    cout << "===========================================\n";

    PerformATMMenueOption(enATMMenueOption(ReadATMMainMenueOption()));
}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, stClient& Client)
{
    vector<stClient> VClient = LoadClientDataFromFile(ClientFileName);

    for (stClient C : VClient)
    {
        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C;
            return true;
        }
    }

    return false;
}

bool LoadClientInfo(string AccountNumber, string PinCode)
{
    if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
        return true;
    else
        return false;
}

void Login()
{
    bool LoginFaild = false;
    string AccountNumber = "", PinCode = "";

    do
    {
        system("cls");
        ShowLoginScreen();
        if (LoginFaild)
        {
            cout << "\nInvalid Account Number / Pin Code!";
        }

        AccountNumber = ReadClientAccountNumber();
        PinCode = ReadClientPinCode();

        LoginFaild = !LoadClientInfo(AccountNumber, PinCode);

    } while (LoginFaild);

    ShowATMMenueScreen();
}

int main()
{
    Login();
    system("pause > 0");
    return 0;
}
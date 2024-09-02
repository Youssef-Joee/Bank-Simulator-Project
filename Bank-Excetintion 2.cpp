#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>

using namespace std;

struct stUser 
{
    string Username;
    string Password;
    int Permissions; 
    bool MarkForDelete = 0;
};

enum enUserChoice {
    ShowClientList = 1, AddNewClient = 2,
    DeleteClient = 3, UpdateClientInfo = 4,
    FindClient = 5, Transactions = 6,
    ManageUsers = 7 , Logout = 8
};

enum enTransactions {
    Deposit = 1, Withdraw = 2,
    TotalBalances = 3, MainMenu = 4
};

enum enManageUsers
{
    ListUsers = 1 , AddNewUser = 2,
    DeleteUser = 3, UpdateUser = 4,
    FindUser = 5 , MainMenu1 = 6 
};

struct stClientData
{
    string AccountNumber;
    string PinCode;
    string ClientName;
    string Phone;
    double Balance;
    bool MarkForDelete = false;

};

const string ClientsFileName = "Clients.txt";

void PerformTransactionOption(stUser User);

void Menu()
{
    system("cls");

    cout << "==================================================\n";
    cout << "\t\t Main Menu Screen \n";
    cout << "==================================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n"; 
    cout << "==================================================\n";

}

enUserChoice TakeResbond()
{
    short C;
    cout << "Choose what do you want to do? [1 to 8]?";
    cin >> C;
    return enUserChoice(C);
}

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord; // define a string variable
    // use find() function to get the position of the delimiters

    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }
    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }
    return vString;
}

stClientData ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    stClientData Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);
    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.ClientName = vClientData[2];
    Client.Phone = vClientData[3];
    Client.Balance = stod(vClientData[4]);//cast string to double

    return Client;
}

vector <stClientData> LoadClientsDataFromFile(string FileName)
{
    vector <stClientData> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);  // Read Mode      

    if (MyFile.is_open())
    {
        string Line;
        stClientData Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);

            vClients.push_back(Client);

        }

        MyFile.close();

    }

    return vClients;

}

vector <stClientData> vClients = LoadClientsDataFromFile(ClientsFileName);

void PrintClientRecord(stClientData Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.ClientName;
    cout << "| " << setw(14) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.Balance;
}

void PrintAllClientsData(vector <stClientData> vClients)
{
    system("cls");
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(14) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    for (stClientData& Client : vClients)
    {

        PrintClientRecord(Client);
        cout << endl;

    }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

}

bool IsCleintExist(string AccountNumber, vector <stClientData> vClients, stClientData& Client1)
{
    for (stClientData& Client : vClients)
    {
        if (Client.AccountNumber == AccountNumber)
        {
            Client1 = Client;
            return true;
        }

    }

    return false;
}

string ConvertRecordToLine(stClientData Client, string Seperator = "#//#")
{
    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.ClientName + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.Balance);
    return stClientRecord;
}

void SaveCleintsDataToFile(string FileName, vector <stClientData> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite
    string DataLine;

    if (MyFile.is_open())
    {
        for (stClientData C : vClients)
        {
            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }

}

void AddClient(string AccountNumber)
{
    stClientData NewClient;

    NewClient.AccountNumber = AccountNumber;
    cout << "Enter Pin Code? ";
    cin >> NewClient.PinCode;
    cout << "Enter Name? ";
    cin >> NewClient.ClientName;
    cout << "Enter Phone? ";
    cin >> NewClient.Phone;
    cout << "Enter Account Balance? ";
    cin >> NewClient.Balance;

    vClients.push_back(NewClient);
    SaveCleintsDataToFile(ClientsFileName, vClients);
}

void AddNewClients()
{
    system("cls");
    string AccNum;
    stClientData Client;

    cout << "-----------------------------------\n";
    cout << "\t Add New Client Screen \n";
    cout << "-----------------------------------\n";
    cout << "Adding New Client:\n";


    char Resbond = 'Y';
    do
    {
        cout << "\nEnter Accout Number? ";
        cin >> AccNum;

        bool ISExist = IsCleintExist(AccNum, vClients, Client);

        while (ISExist)
        {
            cout << "Client with [" << AccNum << "] already exists, Enter another Account Number? ";
            cin >> AccNum;
            ISExist = IsCleintExist(AccNum, vClients, Client);
        }

        AddClient(AccNum);

        cout << "Client Added Successfully, do you want to add more clients? Y/N? ";
        cin >> Resbond;

    } while (toupper(Resbond) == 'Y');

}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <stClientData>& vClients)
{
    for (stClientData& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

void PrintClientCard(stClientData Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number  : " << Client.AccountNumber;
    cout << "\nPin Code       : " << Client.PinCode;
    cout << "\nName           : " << Client.ClientName;
    cout << "\nPhone          : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.Balance;
    cout << "\n-----------------------------------\n\n";
}

void DeleteClientByAccountNumber(string AccountNumber, vector <stClientData>& vClients,
    stClientData Client)
{
    char Answer = 'n';

    PrintClientCard(Client);

    cout << "\n\nAre you sure you want delete this client? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
        SaveCleintsDataToFile(ClientsFileName, vClients);
        //Refresh Clients
        vClients = LoadClientsDataFromFile(ClientsFileName);
        cout << "\n\nClient Deleted Successfully.";
    }

}

void DeleteClients()
{
    string AccNum;
    stClientData Client;

    system("cls");
    cout << "-----------------------------------\n";
    cout << "\t Delete Client Screen \n";
    cout << "-----------------------------------\n";
    cout << "\nPlease enter Account Number? ";
    cin >> AccNum;
    if (IsCleintExist(AccNum, vClients, Client))
    {
        DeleteClientByAccountNumber(AccNum, vClients, Client);
    }
    else
    {
        cout << "Client with Account Number (" << AccNum << ") is Not Found!\n\n";
    }


}

stClientData UpdateClientData(stClientData& Client)
{
    cout << "Enter Pin Code? ";
    getline(cin >> ws, Client.PinCode);
    cout << "Enter Name? ";
    getline(cin, Client.ClientName);
    cout << "Enter Phone? ";
    cin >> Client.Phone;
    cout << "Enter Account Balance? ";
    cin >> Client.Balance;

    return Client;
}

bool FindClientByAccountNumberForUpdate(string AccountNumber, vector <stClientData>& vClients)
{
    for (stClientData& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C = UpdateClientData(C);
            return true;
        }
    }
    return false;
}

void UpdateClientByAccountNumber(string AccountNumber, vector <stClientData>& vClients
    , stClientData Client)
{
    char Answer = 'n';

    PrintClientCard(Client);

    cout << "\n\nAre you sure you want update this client? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        FindClientByAccountNumberForUpdate(AccountNumber,
            vClients);

        SaveCleintsDataToFile(ClientsFileName, vClients);

        cout << "\n\nClient Updated Successfully.";
    }

}

void UpdateClientsInfo()
{
    string AccNum;
    stClientData Client;

    system("cls");
    cout << "-----------------------------------\n";
    cout << "\t Update Client Info Screen \n";
    cout << "-----------------------------------\n";
    cout << "\nPlease enter Account Number? ";
    cin >> AccNum;

    if (IsCleintExist(AccNum, vClients, Client))
    {
        UpdateClientByAccountNumber(AccNum, vClients, Client);
    }
    else
    {
        cout << "Client with Account Number (" << AccNum << ") is Not Found!\n\n";
    }

}

void FindClientsInfo()
{
    string AccNum;
    stClientData Client;

    system("cls");
    cout << "-----------------------------------\n";
    cout << "\t  Find Client Screen \n";
    cout << "-----------------------------------\n";
    cout << "\nPlease enter Account Number? ";
    cin >> AccNum;

    if (IsCleintExist(AccNum, vClients, Client))
    {
        PrintClientCard(Client);
    }
    else
    {
        cout << "Client with Account Number (" << AccNum << ") is Not Found!\n\n";
    }


}

void End()
{
    system("cls");

    cout << "-----------------------------------\n";
    cout << "\t   Programm Ends :-) \n";
    cout << "-----------------------------------\n";
}

void GoToMainMenu(stUser User);

void PerformMainOption(stUser User); 

void TransactionMenu()
{
    system("cls");

    cout << "==================================================\n";
    cout << "\t\tTransactions Menu Screen \n";
    cout << "==================================================\n";
    cout << "\t [1] Deposit.\n";
    cout << "\t [2] Withdraw.\n";
    cout << "\t [3] Total Balances.\n";
    cout << "\t [4] Main Menu.\n";
    cout << "==================================================\n";

}

void FindClientForUpdateBalance(string AccountNumber, vector <stClientData>& vClients,
    double DepositeAmount)
{
    for (stClientData& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.Balance = C.Balance + DepositeAmount;
            cout << "Done successfully, your new Balance is: (" << C.Balance << ") \n";
            break;

        }

    }

}

void UpdataClientBalance(string AccountNumber, vector <stClientData>& vClients
    , stClientData Client, double DepositeAmount)
{
    char Resbond;
    cout << "Are you sure you want perform this transaction? Y/N? ";
    cin >> Resbond;

    if (toupper(Resbond) == 'Y')
    {
        FindClientForUpdateBalance(AccountNumber, vClients, DepositeAmount);
        SaveCleintsDataToFile(ClientsFileName, vClients);

    }

}

void DepositionScreen()
{
    string AccNum;
    stClientData Client;

    system("cls");
    cout << "-----------------------------------\n";
    cout << "\t     Deposit Screen \n";
    cout << "-----------------------------------\n";
    bool IsExist;


    do
    {
        cout << "\nPlease enter Account Number? ";
        cin >> AccNum;

        IsExist = IsCleintExist(AccNum, vClients, Client);

        if (IsExist)
        {
            PrintClientCard(Client);

            double DepositeAmount;
            cout << "Please enter Deposit amount? ";
            cin >> DepositeAmount;

            UpdataClientBalance(AccNum, vClients, Client, DepositeAmount);
            break;

        }
        else
        {
            cout << "Client with Account Number (" << AccNum << ") is Not Found!\n\n";
        }


    } while (!IsExist);

}

void GoToTransactionScreen(stUser User)
{
    cout << "\nPress any Key to go back to Transactions Menu...";
    system("pause>0");

    TransactionMenu();
    PerformTransactionOption(User);

}

bool WithdrawValidation(string AccNum, vector <stClientData> vClients, stClientData Client
    , double Withdraw)
{
    for (stClientData C : vClients)
    {
        if (C.AccountNumber == AccNum)
        {
            if (C.Balance >= Withdraw)
            {
                return true;
            }
            else
            {
                cout << "Amount Exceeds the balance, you can withdraw up to : " << C.Balance << "\n";
                return false;
            }

        }

    }

}

void WithdrawScreen()
{
    string AccNum;
    stClientData Client;

    system("cls");
    cout << "-----------------------------------\n";
    cout << "\t     Withdraw Screen \n";
    cout << "-----------------------------------\n";
    bool IsExist;


    do
    {
        cout << "\nPlease enter Account Number? ";
        cin >> AccNum;

        IsExist = IsCleintExist(AccNum, vClients, Client);

        if (IsExist)
        {
            PrintClientCard(Client);

            double WithdrawAmount;
            do
            {
                cout << "\nPlease enter withdraw amount? ";
                cin >> WithdrawAmount;

            } while (!WithdrawValidation(AccNum, vClients, Client, WithdrawAmount));

            UpdataClientBalance(AccNum, vClients, Client, -1 * WithdrawAmount);

        }
        else
        {
            cout << "Client with Account Number (" << AccNum << ") is Not Found!\n\n";
        }


    } while (!IsExist);




}

double CalcTotalBalance()
{
    double TotalBalance = 0;

    for (stClientData C : vClients)
    {
        TotalBalance += C.Balance;
    }

    return TotalBalance;
}

void PrintClientBalance(stClientData Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.ClientName;
    cout << "| " << setw(12) << left << Client.Balance;

}

void TotalBalcesScreen()
{
    system("cls");

    double TotalBalances = CalcTotalBalance();

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    for (stClientData& Client : vClients)
    {

        PrintClientBalance(Client);
        cout << endl;

    }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "\t\t\t\t Total Balances = " << TotalBalances << endl;

}

void PerformTransactionOption(stUser User)
{
    short C;
    cout << "Choose what do you want to do? [1 to 4]? ";
    cin >> C;

    enTransactions Transaction = (enTransactions)C;
    switch (Transaction)
    {
    case Deposit:
        DepositionScreen();
        GoToTransactionScreen(User);
        break;

    case Withdraw:
        WithdrawScreen();
        GoToTransactionScreen(User);
        break;

    case TotalBalances:
        TotalBalcesScreen();
        GoToTransactionScreen(User);
        break;

    case MainMenu:
        Menu();
        PerformMainOption(User);
        break;

    default:
        break;
    }


}

const string UsersFileName = "Users.txt";

stUser ConvertLinetoUserData(string Line, string Seperator = "#//#") 
{
    stUser User; 
    vector<string> vUserData;  

    vUserData = SplitString(Line, Seperator); 
    User.Username = vUserData[0]; 
    User.Password = vUserData[1]; 
    User.Permissions = stoi(vUserData[2]); 

    return User; 
}

vector <stUser> LoadUsersData(string FileName)   
{
    vector <stUser> vUsers; 

    fstream MyFile;
    MyFile.open(FileName, ios::in);  // Read Mode      

    if (MyFile.is_open())
    {
        string Line;
        stUser User;

        while (getline(MyFile, Line))
        {

            User = ConvertLinetoUserData(Line); 

            vUsers.push_back(User);  

        }

        MyFile.close();

    }

    return vUsers;  

}

vector <stUser>  vUsers = LoadUsersData(UsersFileName);   

bool IsUserExist(stUser& User1)
{
    for (stUser User : vUsers)
    {
        if (User.Username == User1.Username)
        {
            User1.Password = User.Password;
            User1.Permissions = User.Permissions;

            return true;
        }
    }

    return false;
}

bool IsUserInformationCorrect(stUser& User1) 
{
    for (stUser& User : vUsers)
    { 
        if (User.Username == User1.Username)
        { 
            if (User.Password == User1.Password)
            {
                User1.Permissions = User.Permissions;
                return true;
            }
            else
                return false;
        }
    }

    return false;
}

void ShowLoginScreen(bool IsInvalid = 0)
{
    stUser User;

    system("cls");
    cout << "-----------------------------------\n";
    cout << "\t     Login Screen \n";
    cout << "-----------------------------------\n";

    if (IsInvalid)
    {
        cout << "Invalid Username/Password!\n";
    }

    cout << "Enter Username? ";
    cin >> User.Username;  
    cout << "Enter Password? "; 
    cin >> User.Password; 

    if (IsUserInformationCorrect(User))  
    {
        Menu();
        PerformMainOption(User);
    }
    else
    {
        ShowLoginScreen(true);
    }

}

void PrintUserRecord(stUser User)
{
    cout << "| " << setw(15) << left << User.Username; 
    cout << "| " << setw(10) << left << User.Password;  
    cout << "| " << setw(40) << left << User.Permissions; 
}

void ShowUsersList()
{
    system("cls");
    cout << "\n\t\t\t\t Users List (" << vUsers.size() << ") User(s).";  
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "User Name";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(40) << "Permissions";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    for (stUser& User : vUsers) 
    {

        PrintUserRecord(User);  
        cout << endl;

    }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void ShowManageUserScreen(stUser User);
void PerfomManageOption(stUser User);

void GoBackToManageMenu(stUser User) 
{
    cout << "Press any Key to go back to Mange Menu...";

    system("pause>0");

    ShowManageUserScreen(User);

    PerfomManageOption(User); 
}

string ConvertUserRecordToLine(stUser User, string Seperator = "#//#")
{
    string UserRec = ""; 

    UserRec += User.Username + Seperator; 
    UserRec += User.Password + Seperator; 
    UserRec += to_string(User.Permissions);

    return UserRec; 
}

void SaveUsersDataToFile(string FileName, vector <stUser> vUser)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite
    string DataLine;

    if (MyFile.is_open())
    {
        for (stUser C : vUser) 
        {
            if (C.MarkForDelete == false)  
            {
                //we only write records that are not marked for delete.
                DataLine = ConvertUserRecordToLine(C);  
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }

}

void ManageUserPermissions(stUser& User)
{
    short Permission = 0;
    string Res; 

    cout << "\nDo you want to give access to : \n\n";
    cout << "Show Client List? Y/N? ";
    cin >> Res;
    if (Res == "Y" || Res == "y")
    {
        Permission++;
    }


    cout << "Add New Client? Y/N? ";
    cin >> Res;
    if (Res == "Y" || Res == "y")
    {
        Permission += 2;
    }

    cout << "Delete Client? Y/N? ";
    cin >> Res;
    if (Res == "Y" || Res == "y")
    {
        Permission+=4;
    }

    cout << "Update Client? Y/N? ";
    cin >> Res;
    if (Res == "Y" || Res == "y")
    {
        Permission += 8;
    }

    cout << "Find Client? Y/N? ";
    cin >> Res; 
    if (Res == "Y" || Res == "y")
    {
        Permission += 16;
    }

    cout << "Transactions? Y/N? ";
    cin >> Res;
    if (Res == "Y" || Res == "y")
    {
        Permission += 32;
    }

    cout << "Manage Users? Y/N? ";
    cin >> Res;
    if (Res == "Y" || Res == "y")
    {
        Permission += 64;
    }

    User.Permissions = Permission;
}

void AddUser(stUser User)
{
    string Res;
    cout << "Enter Password? ";
    cin >> User.Password;
    cout << "Do you want to give full access? Y/N? ";
    cin >> Res;
    if (Res == "Y" || Res == "y")
    {
        User.Permissions = -1; 
    }
    else
    {
        ManageUserPermissions(User); 
    }
   
    vUsers.push_back(User); 
    SaveUsersDataToFile(UsersFileName, vUsers); 
}

void AddNewUserScreen()
{
    system("cls");
   
    cout << "-----------------------------------\n";
    cout << "\t Add New User Screen \n";
    cout << "-----------------------------------\n";
    cout << "Adding New User:\n";

    stUser User;
    char Resbond = 'Y';
    do
    {
        cout << "\nEnter Username? ";
        cin >> User.Username; 

        bool ISExist = IsUserExist(User);

        while (ISExist)
        {
            cout << "Client with [" << User.Username << "] already exists, Enter another Username? ";
            cin >> User.Username;
            ISExist = IsUserExist(User);  
        }

        AddUser(User);

        cout << "User Added Successfully, do you want to add more Users? Y/N? ";
        cin >> Resbond;

    } while (toupper(Resbond) == 'Y');

}

void PrintUserCard(stUser User)
{
    cout << "\nThe following are the User details:\n";
    cout << "-----------------------------------";
    cout << "\nUsername       : " << User.Username;
    cout << "\nPassword       : " << User.Password;
    cout << "\nPermissions    : " << User.Permissions;
    cout << "\n-----------------------------------\n\n";
}

bool MarkUserForDeleteByUsername(string Username) 
{
    for (stUser& C : vUsers) 
    {
        if (C.Username == Username)   
        {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

void DeleteUserbyUsername(string Username, stUser User)
{
    char Answer = 'n';

    PrintUserCard(User);

    cout << "\n\nAre you sure you want delete this User? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        MarkUserForDeleteByUsername(Username); 
        SaveUsersDataToFile(UsersFileName, vUsers); 
        //Refresh Users vector
        vUsers = LoadUsersData(UsersFileName);  
        cout << "\n\nUser Deleted Successfully.";
    }

}

void DeleteUserFunc()
{
    stUser User;

    system("cls");
    cout << "-----------------------------------\n";
    cout << "\t Delete Users Screen \n";
    cout << "-----------------------------------\n";
    cout << "\nPlease enter Username? ";
    cin >> User.Username;  

    if (User.Username == "Admin")
    {
        cout << "\nYou can't Delete this User.\n";
    }
    else if (IsUserExist(User)) 
    {
        DeleteUserbyUsername(User.Username, User);  
    }
    else
    {
        cout << "User with Account Number (" << User.Username << ") is Not Found!\n\n";  
    }


}

bool FindUserForUpdate(stUser User)
{
    string Res; 

    for (stUser& U : vUsers)
    {
        if (U.Username == User.Username)
        {
            cout << "Enter Password? ";
            cin >> U.Password;
            cout << "Do you want to give full access? Y/N? ";
            cin >> Res;
            if (Res == "Y" || Res == "y")
            {
                U.Permissions = -1;
            }
            else
            {
                ManageUserPermissions(U);
            }

            return 1;
        }
    }
    return 0; 
}

void UpdateUserByUsername(stUser User)
{
        char Answer = 'y'; 
        string Res = "Y";  

        PrintUserCard(User);

        cout << "\n\nAre you sure you want update this User? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            FindUserForUpdate(User);

            SaveUsersDataToFile(UsersFileName, vUsers);  

            cout << "\n\nClient Updated Successfully.";
        }

}

void UpdateUserScreen()

{
    stUser User;

    system("cls");
    cout << "-----------------------------------\n";
    cout << "\t Update Users Screen \n";
    cout << "-----------------------------------\n";
    cout << "\nPlease enter Username? ";
    cin >> User.Username;

    if (User.Username == "Admin")
    {
        cout << "\nYou can't Update this User.\n";
    }
    else if (IsUserExist(User))   
    {
        UpdateUserByUsername(User);  
    }
    else
    {
        cout << "User with Account Number (" << User.Username << ") is Not Found!\n\n";
    }
}

void  FindUserScreen()
{
    stUser User;

    system("cls");
    cout << "-----------------------------------\n";
    cout << "\t Update Users Screen \n";
    cout << "-----------------------------------\n";
    cout << "\nPlease enter Username? ";
    cin >> User.Username;

    if (IsUserExist(User))
    {
        PrintUserCard(User); 
    }
    else
    {
        cout << "Client with Account Number (" << User.Username << ") is Not Found!\n\n";
    }


}

void PerfomManageOption(stUser User)
{
    short C;
    cout << "Choose what do you want to do? [1 to 6]? ";
    cin >> C;
    enManageUsers Choice;
    Choice = enManageUsers(C);

    switch (Choice)
    {
    case ListUsers:
        ShowUsersList(); 
        GoBackToManageMenu(User);
        break;

    case AddNewUser:
        AddNewUserScreen(); 
        GoBackToManageMenu(User); 
        break;

    case DeleteUser:
        DeleteUserFunc();
        GoBackToManageMenu(User);
        break;

    case UpdateUser:
       UpdateUserScreen(); 
       GoBackToManageMenu(User);
        break;

    case FindUser:
        FindUserScreen();
        GoBackToManageMenu(User);
        break;

    case MainMenu1:
        Menu();  
        PerformMainOption(User);
        break;

    default:
        break;
    }
}

void ShowManageUserScreen(stUser User)
{
    system("cls");

    cout << "==================================================\n";
    cout << "\t\t Manage Users Screen \n";
    cout << "==================================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menu.\n";
    cout << "==================================================\n";
    PerfomManageOption(User);
}

int main()
{
   
    ShowLoginScreen(); 

    return 0;
}

void GoToMainMenu(stUser User)
{
    cout << "Press any Key to go back to Main Menu...";

    system("pause>0");

    Menu();

    PerformMainOption(User);
}

void AccessDeniedScreen()
{
    system("cls");
    cout << "-----------------------------------\n";
    cout << "\t Access Denied, \n";
    cout << "You don't have permission To Do this,\n";
    cout << "Please contact your admin. \n";
    cout << "-----------------------------------\n";
}

void PerformMainOption(stUser User)
{
    enUserChoice UserChoice;
    UserChoice = TakeResbond();


    switch (UserChoice)
    {
    case ShowClientList:
        if (User.Permissions == -1 || (User.Permissions & 1) == 1)
        {
            PrintAllClientsData(vClients);
            GoToMainMenu(User);
        }
        else
        {
            AccessDeniedScreen();
            GoToMainMenu(User);
        }
        break;

    case AddNewClient:
        if (User.Permissions == -1 || (User.Permissions & 2) == 2)
        {
            AddNewClients();
            GoToMainMenu(User);
        }
        else
        {
            AccessDeniedScreen();
            GoToMainMenu(User);
        }
        break;

    case DeleteClient:
        if (User.Permissions == -1 || (User.Permissions & 4) == 4)
        {
            DeleteClients();
            GoToMainMenu(User);
        }
        else
        {
            AccessDeniedScreen();
            GoToMainMenu(User);
        }
        break;

    case UpdateClientInfo:
        if (User.Permissions == -1 || (User.Permissions & 8) == 8)
        {
            UpdateClientsInfo();
            GoToMainMenu(User);
        }
        else
        {
            AccessDeniedScreen();
            GoToMainMenu(User);
        }
        break;

    case FindClient:
        if (User.Permissions == -1 || (User.Permissions & 16) == 16)
        {
        FindClientsInfo();
        GoToMainMenu(User);
        }
        else
        {
            AccessDeniedScreen();
            GoToMainMenu(User);
        }
        break;

    case Transactions:
        if (User.Permissions == -1 || (User.Permissions & 32) == 32)
        {
        TransactionMenu();
        PerformTransactionOption(User);
        }
        else
        {
            AccessDeniedScreen();
            GoToMainMenu(User);
        }
        break;

    case ManageUsers: 
        if (User.Permissions == -1 || (User.Permissions & 64) == 64)
        {
        ShowManageUserScreen(User);
        GoToMainMenu(User);
        }
        else
        {
            AccessDeniedScreen();
            GoToMainMenu(User); 
        }
        break;

    case Logout: 
        ShowLoginScreen(); 
        break;

    default:
        break;
    }

}

// Finally............
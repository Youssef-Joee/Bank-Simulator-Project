#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>

using namespace std;

enum enUserChoice {
    ShowClientList = 1, AddNewClient = 2,
    DeleteClient = 3, UpdateClientInfo = 4,
    FindClient = 5,Transactions = 6,
    Exit = 7
};

enum enTransactions { Deposit = 1, Withdraw = 2,
    TotalBalances = 3,MainMenu = 4 };

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

void PerformTransactionOption();

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
    cout << "\t[7] Exit.\n";
    cout << "==================================================\n";

}

enUserChoice TakeResbond()
{
    short C;
    cout << "Choose what do you want to do? [1 to 7]?";
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

void GoToMainMenu();

void PerformMainOption();

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
    , stClientData Client,double DepositeAmount)
{
   char Resbond;
   cout << "Are you sure you want perform this transaction? Y/N? ";
   cin >> Resbond;

   if (toupper(Resbond) == 'Y')
   {
      FindClientForUpdateBalance(AccountNumber,vClients,DepositeAmount); 
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

void GoToTransactionScreen()
{
    cout << "\nPress any Key to go back to Transactions Menu...";
    system("pause>0");

    TransactionMenu();
    PerformTransactionOption();

}

bool WithdrawValidation(string AccNum,vector <stClientData> vClients,stClientData Client
    ,double Withdraw)
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

            UpdataClientBalance(AccNum, vClients, Client,-1* WithdrawAmount);
           
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

void PerformTransactionOption()
{
    short C;
    cout << "Choose what do you want to do? [1 to 4]? ";
    cin >> C;  

    enTransactions Transaction = (enTransactions)C;
    switch (Transaction)
    {
    case Deposit:
        DepositionScreen(); 
        GoToTransactionScreen();
        break;

    case Withdraw:
        WithdrawScreen();
        GoToTransactionScreen(); 
        break;

    case TotalBalances:
        TotalBalcesScreen();
        GoToTransactionScreen(); 
        break;

    case MainMenu:
        Menu(); 
        PerformMainOption(); 
        break;

    default:
        break;
    }


}

int main()
{
    Menu();

    PerformMainOption();


    return 0;
}

void GoToMainMenu()
{
    cout << "Press any Key to go back to Main Menu...";

    system("pause>0");

    Menu();

    PerformMainOption();
}

void PerformMainOption()
{
    enUserChoice UserChoice;
    UserChoice = TakeResbond();


    switch (UserChoice)
    {
    case ShowClientList:
        PrintAllClientsData(vClients);
        GoToMainMenu();
        break;

    case AddNewClient:
        AddNewClients();
        GoToMainMenu();
        break;

    case DeleteClient:
        DeleteClients();
        GoToMainMenu();
        break;

    case UpdateClientInfo:
        UpdateClientsInfo();
        GoToMainMenu();
        break;

    case FindClient:
        FindClientsInfo();
        GoToMainMenu();
        break;

    case Transactions:
        TransactionMenu(); 
        PerformTransactionOption();
        break;

    case Exit:
        End();
        break;

    default:
        break;
    }

}

// Finish !!!!!!
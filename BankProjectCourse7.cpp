



#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <cctype>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

string const clientsFile = "ClientsData.txt";
string const UserFileName = "users.txt";

struct sClient {
	string accountNumber;
	string pinCode;
	string name;
	string phone;
	int accountBalance = 0;
	bool markForDelete = false;
};
struct stUser
{
	string username;
	string password;
	string permissions;
	bool markForDelete = false;

};
stUser g_loggedInUser;
void login();

void setLoggedInUser(stUser user) {
	g_loggedInUser = user;
}
enum enMainMenuPermissions {

	eAll = -1
	, pListClients = 1,
	pAddNewClient = 2,
	pDeleteClient = 4,
	pUpdateClients = 8,
	pFindClient = 16,
	pTranactions = 32,
	pManageUsers = 64

};
void ShowAccessDeniedMessage() {
	cout << "\n------------------------------------\n";
	cout << "Access Denied" << endl;
	cout << "Contact The Admin To Give You The Permission" << endl;

	cout << "\n------------------------------------\n";

}

void UsersOperationsMenu();

stUser readNewUser();



bool CheckAccessPermission(enMainMenuPermissions permission) {
		if (g_loggedInUser.username.empty()) {
			return false; // no user logged in
		}

		int userPermissions = stoi(g_loggedInUser.permissions);

		return (userPermissions & permission) == permission;
	}


// 1
void mainMenu()
{

	cout << "\n==================================================\n";

	cout << "\t\t Main Menu Screen ";

	cout << "\n==================================================\n";
	cout << "\n  [1] Show Client List." << endl;
	cout << "  [2] Add New Client." << endl;
	cout << "  [3] Delete Client." << endl;
	cout << "  [4] Update Client Info ." << endl;
	cout << "  [5] Find Client." << endl;
	cout << "  [6] Transactions." << endl;
	cout << "  [7] Manage Users." << endl;
	cout << "  [8] LogOut." << endl;
	cout << "==================================================\n";

}

// ==================================================================================

void PrintClientRecord(sClient Client) {
	cout << "|" << setw(15) << left << Client.accountNumber;
	cout << "|" << setw(10) << left << Client.pinCode;
	cout << "|" << setw(40) << left << Client.name;
	cout << "|" << setw(12) << left << Client.phone;
	cout << "|" << setw(12) << left << Client.accountBalance;
}

void PrintAllClientsData(vector <sClient> vClients) {

	if (!CheckAccessPermission(enMainMenuPermissions::pListClients))
	{
		ShowAccessDeniedMessage();
		return;
	}
	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
	cout << "\n_________________________________________________________________________________\n";

	cout << "|" << left << setw(15) << "Account Number";
	cout << "|" << left << setw(10) << "Pin Code";
	cout << "|" << left << setw(40) << "Client Name";
	cout << "|" << left << setw(12) << "Phone";
	cout << "|" << left << setw(12) << "Balance";
	cout << "\n_________________________________________________________________________________\n";

	for (sClient Client : vClients) {
		PrintClientRecord(Client);
		cout << endl;
	}
	cout << "\n_________________________________________________________________________________\n";
}

void printClientCard(sClient c)
{
	cout << "Account Number : " << c.accountNumber << endl;
	cout << "Pin Code       : " << c.pinCode << endl;
	cout << "Name           : " << c.name << endl;
	cout << "Account Balance: " << c.accountBalance << endl;
}


void PrintClientRecordForTransactionsMenu(sClient Client) {
	cout << "|" << setw(15) << left << Client.accountNumber;

	cout << "|" << setw(40) << left << Client.name;

	cout << "|" << setw(12) << left << Client.accountBalance;

}


void PrintAllClientsDataForTransactionMenu(vector <sClient> vClients) {
	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
	cout << "\n_________________________________________________________________________________\n";

	cout << "|" << left << setw(15) << "Account Number";

	cout << "|" << left << setw(40) << "Client Name";

	cout << "|" << left << setw(12) << "Balance";
	cout << "\n_________________________________________________________________________________\n";

	int sum = 0;
	for (sClient Client : vClients) {
		PrintClientRecordForTransactionsMenu(Client);
		sum += Client.accountBalance;
		cout << endl;
	}
	cout << "\n_________________________________________________________________________________\n";


	cout << "\t\t\t\t\t\t Total Balances = " << sum << "$ " << endl;
}


// ==================================================================================
vector<string> split(string stLine, string sep) {
	vector<string> vString;
	int pos = 0;
	string newSt;

	while ((pos = stLine.find(sep)) != string::npos) {
		newSt = stLine.substr(0, pos);
		if (newSt != "") {
			vString.push_back(newSt);
		}
		stLine.erase(0, pos + sep.length()); // Remove processed part
	}

	if (stLine != "") {
		vString.push_back(stLine); // Add last part
	}
	return vString;
}


sClient ConvertLineToRecord(string line, string sep = "#//#")
{

	sClient c;

	vector <string> vClientData = split(line, sep);
	c.accountNumber = vClientData[0];
	c.pinCode = vClientData[1];
	c.name = vClientData[2];
	c.phone = vClientData[3];
	c.accountBalance = stod(vClientData[4]);

	return c;

}


vector <sClient> loadClientsDataFromFile(string clientsFile)

{

	fstream myfile;
	vector<sClient> vFileContent;

	myfile.open(clientsFile, ios::in); // Read Mode ; \

	if (myfile.is_open())
	{

		string line;
		sClient client;

		while (getline(myfile, line))
		{

			client = ConvertLineToRecord(line);
			vFileContent.push_back(client);
		}


		myfile.close();
	}

	return vFileContent;
}


string ConvertRecordToLine(sClient client, string seprator = "#//#")
{
	string recordLine = "";
	recordLine += client.accountNumber + seprator;
	recordLine += client.pinCode + seprator;
	recordLine += client.name + seprator;
	recordLine += client.phone + seprator;
	recordLine += to_string(client.accountBalance);
	return recordLine;
}



// ==================================================================================

bool checkIfAccNumExist(string accountNumber, string filename)
{
	fstream myfile;
	sClient c;
	vector <sClient> vClients;
	myfile.open(filename, ios::in);

	if (myfile.is_open())
	{
		string line;

		while (getline(myfile, line))
		{

			c = ConvertLineToRecord(line);

			if (c.accountNumber == accountNumber)
			{
				myfile.close();
				return true;

			}
			vClients.push_back(c);


		}

		myfile.close();
	}

	return false;

}


sClient addNewCleint()
{

	sClient c;
	cout << "\n=======================================\n";
	cout << "Add New Client Screen :" << endl;
	cout << "=======================================" << endl;

	cout << "Enter Account number :" << endl;
	getline(cin >> ws, c.accountNumber);
	while (checkIfAccNumExist(c.accountNumber, clientsFile)) {


		cout << "\nClient with [" << c.accountNumber << "] already exists, Enter another Account Number? ";
		getline(cin >> ws, c.accountNumber);


	}
	cout << "Enter Pin Code ? " << endl;
	getline(cin, c.pinCode);

	cout << "Enter Name ? " << endl;
	getline(cin, c.name);

	cout << "Enter Phone ?" << endl;
	getline(cin, c.phone);

	cout << "Enter Account Balance ? " << endl;
	cin >> c.accountBalance;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	cout << endl;
	cout << "\n=======================================\n";
	cout << "CLIENT ADDED SUCCECFULLY !" << endl;
	cout << "=======================================";
	return c;
}

// ==================================================================================




// ==================================================================================



vector <sClient> saveClientDataToFile(string filename, vector<sClient> vClients)
{


	fstream myfile;


	myfile.open(filename, ios::out);

	sClient c;
	string line;
	if (myfile.is_open())
	{

		for (sClient c : vClients)
		{

			if (c.markForDelete == false)
			{

				line = ConvertRecordToLine(c);

				myfile << line << endl;
			}


		}


		myfile.close();


	}


	return vClients;


}



// ==================================================================================
// ==================================================================================
bool searchForAccountByNumber(string accountNumber, sClient& client)
{
	vector<sClient> vClients = loadClientsDataFromFile(clientsFile);

	for (sClient c : vClients)
	{


		if (c.accountNumber == accountNumber)
		{
			client = c;
			return true;
		}

	}

	return false;

}

bool markUpforDeleteByAccountNumber(string accountNumber, vector<sClient>& vClients)
{



	for (sClient& c : vClients)
	{

		if (c.accountNumber == accountNumber)
		{

			c.markForDelete = true;
			return true;

		}



	}


	return false;

}

bool deleteAccountByNumber(string accountNumber, vector<sClient>& vClients)
{


	sClient client;


	char answer = 'n';


	if (searchForAccountByNumber(accountNumber, client))
	{

		printClientCard(client);

		cout << endl;
		cout << "Do you want to delete This Client ?" << endl;

		cin >> answer;

		if (answer == 'y' || answer == 'Y')
		{

			if (markUpforDeleteByAccountNumber(accountNumber, vClients))
			{

				cout << "\n_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\n";
				cout << "       Account Deleted Succefully !  " << endl;
				cout << "_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\n";

				saveClientDataToFile(clientsFile, vClients);
				return true;



			}
			else {


				cout << "No Account with this number ! " << endl;
				return false;
			}


		}



	}
	else {


		cout << "No Account with this number ! " << endl;
		return false;
	}


	return false;



}

// ==================================================================================
// ==================================================================================

sClient changeClientRecord(string accountNumber)
{

	sClient c;
	c.accountNumber = accountNumber;

	cout << "Enter Pin Code ? " << endl;
	getline(cin, c.pinCode);

	cout << "Enter Name ? " << endl;
	getline(cin, c.name);

	cout << "Enter Phone ?" << endl;
	getline(cin, c.phone);

	cout << "Enter Account Balance ? " << endl;
	cin >> c.accountBalance;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	return c;
}

bool searchAccountByAccNumber(string accountNumber, sClient client)
{

	vector<sClient> vClients = loadClientsDataFromFile(clientsFile);



	for (sClient c : vClients)
	{

		if (c.accountNumber == accountNumber)
		{


			client = c;
			return true;

		}




	}

	return false;





}

bool updateAccountByAccountNumber(string accountNumber, vector<sClient>& vClients)
{

	sClient c;

	if (searchForAccountByNumber(accountNumber, c))
	{

		printClientCard(c);
		char answer;

		cout << "\n Are you sure you want to update the client data ? " << endl;

		cin >> answer;
		cin.ignore(1, '\n');

		if (answer == 'y' || answer == 'Y')
		{





			for (sClient& client : vClients)
			{

				if (client.accountNumber == accountNumber)
				{
					client = changeClientRecord(accountNumber);
					break;
				}

			}

			saveClientDataToFile(clientsFile, vClients);
			cout << "\n\nClient Updated Successfully.";
			return true;

		}




	}





	return false;




}



void AddingNewClient(vector <sClient>& vClients)
{
	if (!CheckAccessPermission(enMainMenuPermissions::pAddNewClient))
	{
		ShowAccessDeniedMessage();
		return;
	}
	sClient newClient = addNewCleint();
	vClients.push_back(newClient);


	saveClientDataToFile(clientsFile, vClients);

}


void DeletingClient(vector <sClient>& vClients)
{
	if (!CheckAccessPermission(enMainMenuPermissions::pDeleteClient))
	{
		ShowAccessDeniedMessage();
		return;
	}
	string accountNumber;
	cout << "Enter The Account number you want to delete :" << endl;
	cin >> accountNumber;
	deleteAccountByNumber(accountNumber, vClients);

}


void updatingClient(vector <sClient>& vClients)
{
	if (!CheckAccessPermission(enMainMenuPermissions::pUpdateClients))
	{
		ShowAccessDeniedMessage();
		return;
	}
	string accountNumber;

	cout << "Enter the Account Number you want to update it :" << endl;

	cin >> accountNumber;

	updateAccountByAccountNumber(accountNumber, vClients);

}


void findClient()
{
	if (!CheckAccessPermission(enMainMenuPermissions::pFindClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

	vector<sClient> vClients = loadClientsDataFromFile(clientsFile);

	string accNumber;
	cout << "Enter The Account Number of the Client :" << endl;
	cin >> accNumber;
	bool found = false;

	for (sClient c : vClients)
	{


		if (c.accountNumber == accNumber)
		{

			cout << "CLIENT OF NUM (" << accNumber << ") INFO:" << endl;

			printClientCard(c);
			found = true;
		}



	}

	if (!found)
	{
		cout << "CLIENT OF NUM (" << accNumber << ") NOT FOUND !" << endl;
		cout << "PRESS ANY KEY TO BACK TO THE MENU (:" << endl;

	}

}


// ==================================================================================
//  EXTENSIONS ( PART 2 OF THE PROJECT ) 
// ==================================================================================


void TransactionsMenu()
{


	cout << "=========================================" << endl;
	cout << "\t TRANSACTIONS MENU " << endl;
	cout << "=========================================" << endl;

	cout << "\t\t[1] Deposit ." << endl;
	cout << "\t	[2] WithDraw ." << endl;
	cout << "\t	[3] Total Balances ." << endl;
	cout << "\t	[4] Main Menu ." << endl;
	cout << "=========================================" << endl;






}



int addDeposit(string accountNumber, int amount, vector <sClient>& vClients)
{


	for (sClient& c : vClients)
	{

		if (c.accountNumber == accountNumber)
		{


			c.accountBalance += amount;
			saveClientDataToFile(clientsFile, vClients);

			return c.accountBalance;
		}



	}


	return -1;


}

void Depoist(vector <sClient>& vClients)
{

	int number;
	string accountNumber;

	cout << "Enter The Account number you want to depoist in :" << endl;
	cin >> accountNumber;
	cout << "Enter the amount you want to deposit :" << endl;
	cin >> number;

	int newBalance = addDeposit(accountNumber, number, vClients);

	if (newBalance != -1)
	{

		cout << number << "$ Added Succecfully to (" << accountNumber << ") Balance" << endl;
		saveClientDataToFile(clientsFile, vClients);

	}



}



int addWithDraw(string accountNumber, int amount, vector <sClient>& vClients)
{
	for (sClient& c : vClients)
	{

		if (c.accountNumber == accountNumber)
		{


			if (c.accountBalance > 0)
			{

				c.accountBalance -= amount;
				saveClientDataToFile(clientsFile, vClients);

				return c.accountBalance;
			}


			if (c.accountBalance < amount)
			{
				cout << "NO SUCH ENOUGH AMOUNT TO WITH DRAW !" << endl;
				cout << "PRESS ANY KEY TO BACK TO THE MAIN MENU (: " << endl;
			}



		}



	}


	return -1;


}



void WithDraw(vector <sClient>& vClients)
{


	int number;
	string accountNumber;

	cout << "Enter The Account number you want to WithDraw From :" << endl;
	cin >> accountNumber;


	for (sClient& c : vClients)
	{

		if (c.accountNumber == accountNumber) {


			printClientCard(c);

		}

	}






	cout << "\nEnter the amount you want to WithDraw :" << endl;
	cin >> number;

	int newBalance = addWithDraw(accountNumber, number, vClients);

	if (newBalance != -1)
	{

		cout << number << "$ WithDrawed Succecfully to (" << accountNumber << ") Balance" << endl;


	}






}



void printTransactionTable(vector <sClient>& vClients)
{

	PrintAllClientsDataForTransactionMenu(vClients);


}




// ==================================================================================











void TranSactionsMenuOperation(vector<sClient>& vClients)
{

	if (!CheckAccessPermission(enMainMenuPermissions::pTranactions))
	{
		ShowAccessDeniedMessage();
		return;
	}





	int choice;

	do {
		TransactionsMenu();

		cout << "Choose What Do You Want To Do ? [1 to 4]" << endl;
		cin >> choice;





		system("cls");




		switch (choice)
		{

		case 1:
			Depoist(vClients);
			system("pause>0");
			system("cls");
			break;

		case 2:
			WithDraw(vClients);

			system("pause>0");
			system("cls");


		case 3:


			printTransactionTable(vClients);
			system("pause>0");
			system("cls");


		case 4:

			cout << "Returning to Main Menu..." << endl;
			cout << "Press Any Key To Go Back..." << endl;
			return;

		}




	} while (true);













}

// ==================================================================================
// ==================================================================================
// ==================================================================================
// ==================================================================================
// ==================================================================================
// ==================================================================================
// ==================================================================================
// ==================================================================================
// ==================================================================================
// ==================================================================================
// ==================================================================================
// 
// 
//				USER SECTION      USER SECTION       USER SECTION 

// ==================================================================================







void PrintUserRecord(stUser user) {
	cout << "|" << setw(15) << left << user.username;
	cout << "|" << setw(10) << left << user.password;
	cout << "|" << setw(40) << left << user.permissions;

}

void PrintAllUsersData(vector <stUser> vUser) {

	if (!CheckAccessPermission(enMainMenuPermissions::pListClients))
	{
		ShowAccessDeniedMessage();
		return;
	}

	cout << "\n\t\t\t\tUsers List (" << vUser.size() << ") Users(s).";
	cout << "\n_________________________________________________________________________________\n";

	cout << "|" << left << setw(15) << "User Name";
	cout << "|" << left << setw(10) << "Pin Code";
	cout << "|" << left << setw(40) << "Premission ";


	cout << "\n_________________________________________________________________________________\n";

	for (stUser user : vUser) {
		PrintUserRecord(user);
		cout << endl;
	}
	cout << "\n_________________________________________________________________________________\n";
}

void printUserData(stUser u)
{
	cout << "UserName: " << u.username << endl;
	cout << "PassWord  : " << u.password << endl;
	cout << "Premissions : " << u.permissions << endl;

}

// ==================================================================================
// ==================================================================================

stUser ConvertLineToRecordUsersVersion(string line, string sep = "#//#")
{
	stUser user;
	vector<string> vUsersData = split(line, sep);

	if (vUsersData.size() >= 3) {
		user.username = vUsersData[0];
		user.password = vUsersData[1];
		user.permissions = vUsersData[2];
	}
	else {
		// Handle malformed or empty line safely
		user.username = "";
		user.password = "";
		user.permissions = "";
	}

	return user;
}


string ConvertRecordToLineUserVersion(stUser user, string seprator = "#//#")
{
	string recordLine = "";
	recordLine += user.username + seprator;
	recordLine += user.password + seprator;
	recordLine += user.permissions;


	return recordLine;
}


vector <stUser> saveUsersDataToFile(string userFileName, vector<stUser>& vUsers)
{


	fstream myfile;


	myfile.open(userFileName, ios::out);


	string line;
	if (myfile.is_open())
	{

		for (stUser& user : vUsers)
		{

			if (!user.markForDelete)
			{
				string line = ConvertRecordToLineUserVersion(user);
				myfile << line << endl;
			}


		}


		myfile.close();


	}


	return vUsers;


}

vector<stUser> loadUsersDataFromFile(string usersFileName)
{
	fstream myfile;
	vector<stUser> vUserFileContent;

	myfile.open(usersFileName, ios::in);

	if (myfile.is_open())
	{
		string line;
		stUser user;

		while (getline(myfile, line))
		{
			// Ignore empty lines
			if (line == "" || line.find("#//#") == string::npos)
				continue;

			user = ConvertLineToRecordUsersVersion(line);

			// Only add valid users
			if (!user.username.empty() && !user.password.empty())
				vUserFileContent.push_back(user);
		}

		myfile.close();
	}

	return vUserFileContent;
}

// ==================================================================================
// ==================================================================================

bool searchUserByUserName(string username, stUser& user)
{
	vector <stUser> vUsers = loadUsersDataFromFile(UserFileName);

	for (stUser& u : vUsers)
	{

		if (u.username == username)
		{
			user = u;
			return true;
		}



	}


	return false;


}



// ==================================================================================
// ==================================================================================

int ReadPermissionsToSet()
{
	char Answer;
	int Permissions = 0;

	cout << "\nDo you want to give access to: \n";

	cout << "\nShow Client List? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += enMainMenuPermissions::pListClients;

	cout << "\nAdd New Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += enMainMenuPermissions::pAddNewClient;

	cout << "\nDelete Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += enMainMenuPermissions::pDeleteClient;

	cout << "\nUpdate Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += enMainMenuPermissions::pUpdateClients;

	cout << "\nFind Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += enMainMenuPermissions::pFindClient;

	cout << "\nTransactions? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += enMainMenuPermissions::pTranactions;

	cout << "\nManage Users? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
		Permissions += enMainMenuPermissions::pManageUsers;

	return Permissions;
}


stUser readNewUser()
{
	stUser user;
	cout << "Enter UserName : ";

	getline(cin >> ws, user.username);



	while (searchUserByUserName(user.username, user))
	{

		cout << "User With Name [" << user.username << "] is Already Exists !" << endl;
		cout << "\nEnter Another UserName :" << endl;
		getline(cin >> ws, user.username);
	}

	cout << "Enter PassWord : ";

	getline(cin >> ws, user.password);

	user.permissions = to_string(ReadPermissionsToSet());

	return user;



}

void AddNewUser(vector <stUser>& vUsers)
{


	stUser newUser = readNewUser();
	vUsers.push_back(newUser);
	cout << "\n_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\n";
	cout << "       User Added Succefully !  " << endl;
	cout << "_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\n";


	saveUsersDataToFile(UserFileName, vUsers);

}
// ==================================================================================
// ==================================================================================


bool markUpforDeleteByUserNameAndPassWord(string username, string passWord, vector<stUser>& vUsers)
{



	for (stUser& user : vUsers)
	{

		if (user.username == username && user.password == passWord)
		{

			user.markForDelete = true;

			return true;

		}



	}


	return false;

}


bool deleteUserByUserName(string username, vector <stUser>& vUsers)
{
	if (username == "Admin") // Protect admin
	{
		cout << "\nCannot delete the admin account!" << endl;
		return false;
	}

	stUser user;


	char answer = 'n';

	string pass;

	if (searchUserByUserName(username, user))
	{

		printUserData(user);
		cout << endl;

		cout << "Do you want to delete This User ? " << endl;
		cin >> answer;


		if (answer == 'y' || answer == 'Y')

		{
			cout << "Enter the passWord Again?" << endl;
			cin >> pass;
			if (markUpforDeleteByUserNameAndPassWord(username, pass, vUsers))
			{

				cout << "\n_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\n";
				cout << "       User Deleted Succefully !  " << endl;
				cout << "_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\n";

				saveUsersDataToFile(UserFileName, vUsers);
				return true;




			}
			else {


				cout << "No Account with this number ! " << endl;
				return false;
			}
		}

	}




	return false;




}

void deleteUser()
{
	vector<stUser> vUsers = loadUsersDataFromFile(UserFileName);
	string userName;
	cout << "Enter the user you want to delete ? " << endl;
	cin >> userName;


	if (deleteUserByUserName(userName, vUsers)) {
		// saveUsersDataToFile is already called inside deleteUserByUserName on success,
		// but if you prefer centralizing saving, you can call it here:
		// saveUsersDataToFile(UserFileName, vUsers);

		cout << "\n_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\n";
		cout << "       User Deleted Succefully !  " << endl;
		cout << "_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\n";
	}
	else {
		cout << "Delete operation failed or user not found." << endl;
	}
}

// ==================================================================================
// ==================================================================================
// ==================================================================================
stUser changeUserRecord(string username)
{

	stUser u;
	u.username = username;

	cout << "Enter PassWord ?" << endl;
	getline(cin >> ws, u.password);


	u.permissions = ReadPermissionsToSet();


	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	return u;
}



void updateUser(vector<stUser>& vUsers, string userFileName)
{
	string username;
	cout << "\nEnter the username you want to update: ";
	cin >> username;
	if (username == "admin") // Protect admin
	{
		cout << "\nCannot update the admin account!" << endl;
		return;
	}
	bool found = false;

	for (stUser& user : vUsers)
	{
		if (user.username == username)
		{
			found = true;

			cout << "\nUser found:\n";
			cout << "Username: " << user.username << endl;
			cout << "Current password: " << user.password << endl;
			cout << "Current permissions: " << user.permissions << endl;

			cout << "\nEnter new password (or press Enter to keep current): ";
			cin.ignore();
			string newPassword;
			getline(cin, newPassword);

			if (!newPassword.empty())
				user.password = newPassword;

			cout << "Enter new permissions (or press Enter to keep current): ";
			string newPermissions;
			getline(cin, newPermissions);

			if (!newPermissions.empty())
				user.permissions = newPermissions;

			cout << "\nUser updated successfully.\n";


			saveUsersDataToFile(userFileName, vUsers);

			break;
		}
	}

	if (!found)
	{
		cout << "\nUser not found.\n";
	}

	cout << "\nPress Enter to return to the main menu...";
	cin.get();
}


void findUser(vector<stUser>& vUsers, string UserFileName)
{


	string username;
	cout << "\nEnter the username you want to Find : ";
	getline(cin >> ws, username);




	for (stUser& u : vUsers)
	{

		if (u.username == username)
		{

			cout << "Here is The User Info :" << endl;
			cout << "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-" << endl;
			printUserData(u);
			cout << "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-" << endl;
			break;
		}



	}



	cout << "\nUser not found!\n";






}



















void mainOperation()
{
	vector<sClient> vClients = loadClientsDataFromFile(clientsFile);
	int choice;

	do {
		mainMenu();
		cout << "\nLogin Successful! Welcome, " << g_loggedInUser.username << endl;
		cout << "\n\nChoose What Do You Want To Do ? [1 to 8] : ";
		cin >> choice;
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); // fix input buffer
		system("cls");

		switch (choice)
		{
		case 1:
			PrintAllClientsData(vClients);
			cout << "\nPress any key to return to the main menu...";
			cin.get();
			system("cls");
			break;

		case 2:
			AddingNewClient(vClients);
			cout << "\nPress any key to return to the main menu...";
			cin.get();
			system("cls");
			break;

		case 3:
			DeletingClient(vClients);
			cout << "\nPress any key to return to the main menu...";
			cin.get();
			system("cls");
			break;

		case 4:
			updatingClient(vClients);
			cout << "\nPress any key to return to the main menu...";
			cin.get();
			system("cls");
			break;

		case 5:
			findClient();
			cout << "\nPress any key to return to the main menu...";
			cin.get();
			system("cls");
			break;

		case 6:
			TranSactionsMenuOperation(vClients);
			cout << "\nPress any key to return to the main menu...";
			cin.get();
			system("cls");
			break;

		case 7:
			UsersOperationsMenu();
			cout << "\nPress any key to return to the main menu...";
			cin.get();
			system("cls");
			break;

		case 8:
			cout << "\nLogging out...\n";
			
			break;

		default:
			cout << "Invalid choice, please try again...\n";
			system("pause>0");
			system("cls");
			break;
		}

	} while (choice != 8);

	cout << "Thank You For Using My Program <3" << endl;
}

void login() {
	cout << "-_-_--_-_--_-_--_-_-_--_-_-_--_-_-" << endl;
	cout <<    "  LOGIN SCREEN   " << endl;
	cout << "-_-_--_--_--_-_-_--_-_-_--_-_- -_-" << endl;

	string username, password;
	stUser user;

	cout << "Enter Username: ";
	getline(cin >> ws, username);
	cout << "Enter Password: ";
	getline(cin >> ws, password);

	if (searchUserByUserName(username, user) && user.password == password) {
		g_loggedInUser = user;
		
		system("cls");
	}
	else {
		cout << "\nInvalid username or password! Try again." << endl;
		system("cls");
		login(); 
	}
}



void UsersOperationsMenu()
{
	vector<stUser> vUsers = loadUsersDataFromFile(UserFileName);
	short choice;

	do {
		cout << "=====================================\n";
		cout << "    Manage User Menu Screen \n";
		cout << "=====================================\n";
		cout << "[1] List Users\n";
		cout << "[2] Add Users\n";
		cout << "[3] Delete Users\n";
		cout << "[4] Update Users\n";
		cout << "[5] Find Users\n";
		cout << "[6] Return to Main Menu\n";
		cout << "=====================================\n";
		cout << "Enter The Operation you want to do [1 to 6] : ";
		cin >> choice;
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
		system("cls");

		switch (choice)
		{
		case 1:
			PrintAllUsersData(vUsers);
			cout << "\nPress any key to return...";
			cin.get();
			system("cls");
			break;

		case 2:
			AddNewUser(vUsers);
			cout << "\nPress any key to return...";
			cin.get();
			system("cls");
			break;

		case 3:
			deleteUser();
			cout << "\nPress any key to return...";
			cin.get();
			system("cls");
			break;

		case 4:
			updateUser(vUsers, UserFileName);
			cout << "\nPress any key to return...";
			cin.get();
			system("cls");
			break;

		case 5:
			findUser(vUsers, UserFileName);
			cout << "\nPress any key to return...";
			cin.get();
			system("cls");
			break;

		case 6:
			cout << "Returning to Main Menu...\n";
			break;

		default:
			cout << "Invalid choice, please try again...\n";
			system("pause>0");
			system("cls");
			break;
		}

	} while (choice != 6);
}


int main()
{

	login();
	mainOperation();
	system("pause>0");
	return 0;
}
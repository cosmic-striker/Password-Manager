#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <exception>

using namespace std;

struct Account {
    string appName;
    string userName;
    string password;
    string type;
};

class PasswordManager {
protected:
    vector<Account> accounts;
    string choose;

    void loadAccounts() {
        try {
            ifstream file("accounts.txt");
            if (!file) throw runtime_error("Unable to open file for reading.");

            Account account;
            while (file >> account.type >> account.appName >> account.userName >> account.password) {
                accounts.push_back(account);
            }
            file.close();
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void saveAccounts() {
        try {
            ofstream file("accounts.txt");
            if (!file) throw runtime_error("Unable to open file for writing.");

            for (const auto& account : accounts) {
                file << account.type << " " << account.appName << " " << account.userName << " " << account.password << endl;
            }
            file.close();
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    bool validatePassword(const string& password) {
        return password.length() >= 8;
    }

public:
    PasswordManager() {
        loadAccounts();
    }

    void createAccount(const string& type) {
        try {
            Account newAccount;
            newAccount.type = type;

            cout << "Enter " << type << " name: ";
            cin >> newAccount.appName;
            cout << "Enter user name: ";
            cin >> newAccount.userName;

            string password, confirmPassword;
            cout << "Enter password (minimum 8 characters): ";
            cin >> password;
            while (!validatePassword(password)) {
                cout << "Password too short. Enter password (minimum 8 characters): ";
                cin >> password;
            }
            cout << "Confirm password: ";
            cin >> confirmPassword;
            while (password != confirmPassword) {
                cout << "Passwords do not match. Enter password again: ";
                cin >> password;
                cout << "Confirm password: ";
                cin >> confirmPassword;
            }
            newAccount.password = password;
            cout << "Would you like to save the password (yes/no): ";
            cin >> choose;
            if (choose == "yes") {
                accounts.push_back(newAccount);
                saveAccounts();
                cout << "Account created successfully!" << endl << endl;
            } else {
                cout << "The account and the password are not saved" << endl;
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void accessAccount(const string& type) {
        try {
            string appName, userName, password;
            cout << "Enter " << type << " name: ";
            cin >> appName;
            cout << "Enter user name: ";
            cin >> userName;
            cout << "Enter password: ";
            cin >> password;

            bool found = false;
            for (const auto& account : accounts) {
                if (account.type == type && account.appName == appName && account.userName == userName && account.password == password) {
                    cout << "Access granted for " << type << " " << appName << "!" << endl << endl;
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "Access denied. Incorrect " << type << " name, user name, or password." << endl << endl;
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void listAppsOrWebpages(const string& type) {
        try {
            cout << "List of " << (type == "app" ? "Apps:" : "Webpages:") << endl;
            for (const auto& account : accounts) {
                if (account.type == type) {
                    cout << "Name: " << account.appName << ", User: " << account.userName << endl;
                }
            }
            cout << endl;
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void searchByUserName(const string& type) {
        try {
            string userName;
            cout << "Enter user name: ";
            cin >> userName;

            bool found = false;
            for (const auto& account : accounts) {
                if (account.type == type && account.userName == userName) {
                    cout << "Found " << type << " - Name: " << account.appName << ", User: " << account.userName << endl;
                    found = true;
                }
            }
            if (!found) {
                cout << "No " << type << " account found with user name: " << userName << endl << endl;
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    virtual void updateAccount(const string& type) = 0;
    virtual void deleteAccount(const string& type) = 0;

    void adminAccess() {
        try {
            string adminPassword;
            cout << "Enter admin password: ";
            cin >> adminPassword;
            if (adminPassword == "admin123") {
                cout << setw(10) << "Type" << setw(20) << "App/Web Name" << setw(20) << "User Name" << setw(15) << "Password" << endl;
                for (const auto& account : accounts) {
                    cout << setw(10) << account.type << setw(20) << account.appName << setw(20) << account.userName << setw(15) << account.password << endl;
                }
                cout << endl << endl;
                else {
                    cout << "Invalid admin password." << endl << endl;
                }
            } catch (const exception& e) {
                cerr << "Error: " << e.what() << endl;
            }
        }

        void exitProgram() {
            cout << "Exiting program..." << endl;
            exit(0);
        }
    };

    class AppPasswordManager : public PasswordManager {
    public:
        void updateAccount(const string& type) override {
            try {
                string appName, userName, password;
                cout << "Enter " << type << " name: ";
                cin >> appName;
                cout << "Enter user name: ";
                cin >> userName;
                cout << "Enter password: ";
                cin >> password;

                bool found = false;
                for (auto& account : accounts) {
                    if (account.type == type && account.appName == appName && account.userName == userName && account.password == password) {
                        found = true;
                        cout << "Enter new " << type << " name (or '.' to skip): ";
                        string newAppName;
                        cin >> newAppName;
                        if (newAppName != ".") account.appName = newAppName;

                        cout << "Enter new user name (or '.' to skip): ";
                        string newUserName;
                        cin >> newUserName;
                        if (newUserName != ".") account.userName = newUserName;

                        cout << "Enter new password (minimum 8 characters, or '.' to skip): ";
                        string newPassword;
                        cin >> newPassword;
                        if (newPassword != ".") {
                            while (!validatePassword(newPassword)) {
                                cout << "Password too short. Enter new password (minimum 8 characters, or '.' to skip): ";
                                cin >> newPassword;
                                if (newPassword == ".") break;
                            }
                            if (newPassword != ".") account.password = newPassword;
                        }
                        saveAccounts();
                        cout << type << " account updated successfully!" << endl << endl;
                        break;
                    }
                }
                if (!found) {
                    cout << "Account not found or incorrect password." << endl << endl;
                }
            } catch (const exception& e) {
                cerr << "Error: " << e.what() << endl;
            }
        }

        void deleteAccount(const string& type) override {
            try {
                string appName, userName, password;
                cout << "Enter " << type << " name: ";
                cin >> appName;
                cout << "Enter user name: ";
                cin >> userName;
                cout << "Enter password: ";
                cin >> password;

                for (auto it = accounts.begin(); it != accounts.end(); ++it) {
                    if (it->type == type && it->appName == appName && it->userName == userName && it->password == password) {
                        cout << "Would you like to delete this user? (yes/no): ";
                        string confirmation;
                        cin >> confirmation;
                        if (confirmation == "yes") {
                            accounts.erase(it);
                            saveAccounts();
                            cout << type << " account deleted successfully!" << endl << endl;
                        } else {
                            cout << "Deletion cancelled." << endl << endl;
                        }
                        return;
                    }
                }
                cout << "Account not found or incorrect password." << endl << endl;
            } catch (const exception& e) {
                cerr << "Error: " << e.what() << endl;
            }
        }
    };

    class WebpagePasswordManager : public PasswordManager {
    public:
        void updateAccount(const string& type) override {
            try {
                string appName, userName, password;
                cout << "Enter " << type << " name: ";
                cin >> appName;
                cout << "Enter user name: ";
                cin >> userName;
                cout << "Enter password: ";
                cin >> password;

                bool found = false;
                for (auto& account : accounts) {
                    if (account.type == type && account.appName == appName && account.userName == userName && account.password == password) {
                        found = true;
                        cout << "Enter new " << type << " name (or '.' to skip): ";
                        string newAppName;
                        cin >> newAppName;
                        if (newAppName != ".") account.appName = newAppName;

                        cout << "Enter new user name (or '.' to skip): ";
                        string newUserName;
                        cin >> newUserName;
                        if (newUserName != ".") account.userName = newUserName;

                        cout << "Enter new password (minimum 8 characters, or '.' to skip): ";
                        string newPassword;
                        cin >> newPassword;
                        if (newPassword != ".") {
                            while (!validatePassword(newPassword)) {
                                cout << "Password too short. Enter new password (minimum 8 characters, or '.' to skip): ";
                                cin >> newPassword;
                                if (newPassword == ".") break;
                            }
                            if (newPassword != ".") account.password = newPassword;
                        }
                        saveAccounts();
                        cout << type << " account updated successfully!" << endl << endl;
                        break;
                    }
                }
                if (!found) {
                    cout << "Account not found or incorrect password." << endl << endl;
                }
            } catch (const exception& e) {
                cerr << "Error: " << e.what() << endl;
            }
        }

        void deleteAccount(const string& type) override {
            try {
                string appName, userName, password;
                cout << "Enter " << type << " name: ";
                cin >> appName;
                cout << "Enter user name: ";
                cin >> userName;
                cout << "Enter password: ";
                cin >> password;

                for (auto it = accounts.begin(); it != accounts.end(); ++it) {
                    if (it->type == type && it->appName == appName && it->userName == userName && it->password == password) {
                        cout << "Would you like to delete this user? (yes/no): ";
                        string confirmation;
                        cin >> confirmation;
                        if (confirmation == "yes") {
                            accounts.erase(it);
                            saveAccounts();
                            cout << type << " account deleted successfully!" << endl << endl;
                        } else {
                            cout << "Deletion cancelled." << endl << endl;
                        }
                        return;
                    }
                }
                cout << "Account not found or incorrect password." << endl << endl;
            } catch (const exception& e) {
                cerr << "Error: " << e.what() << endl;
            }
        }
    };

    int main() {
        try {
            AppPasswordManager appManager;
            WebpagePasswordManager webpageManager;
            int choice;
            while (true) {
                cout << "1. Manage App" << endl;
                cout << "2. Manage Webpage" << endl;
                cout << "3. Admin Access" << endl;
                cout << "4. Exit" << endl;
                cout << "Enter your choice: ";
                cin >> choice;
                cout << endl;

                string type = (choice == 1) ? "app" : "webpage";

                switch (choice) {
                    case 1:
                    case 2: {
                        int subChoice;
                        cout << "1. Create new " << type << " account" << endl;
                        cout << "2. Access " << type << " account" << endl;
                        cout << "3. List all " << type << "s" << endl;
                        cout << "4. Search by user name" << endl;
                        cout << "5. Update " << type << " account" << endl;
                        cout << "6. Delete " << type << " account" << endl;
                        cout << "Enter your choice: ";
                        cin >> subChoice;
                        cout << endl;

                        PasswordManager* manager = (choice == 1) ? static_cast<PasswordManager*>(&appManager) : static_cast<PasswordManager*>(&webpageManager);

                        if (subChoice == 1) {
                            manager->createAccount(type);
                        } else if (subChoice == 2) {
                            manager->accessAccount(type);
                        } else if (subChoice == 3) {
                            manager->listAppsOrWebpages(type);
                        } else if (subChoice == 4) {
                            manager->searchByUserName(type);
                        } else if (subChoice == 5) {
                            manager->updateAccount(type);
                        } else if (subChoice == 6) {
                            manager->deleteAccount(type);
                        } else {
                            cout << "Invalid choice." << endl << endl;
                        }
                        break;
                    }
                    case 3:
                        appManager.adminAccess();
                        break;
                    case 4:
                        appManager.exitProgram();
                        break;
                    default:
                        cout << "Invalid choice. Please try again." << endl << endl;
                }
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
        return 0;
    }
//I Hope this code help you in your endeavours....

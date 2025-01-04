#include <iostream>
#include <cstring>  // For strcmp
#include <iomanip>
#include <cmath>
#include <vector>
#include<fstream>

using namespace std;

// Exception class for handling restaurant-related errors
class RestaurantException : public exception {
public:
    const char* what() const noexcept override {
        return "Restaurant System Exception Occurred!";
    }
};

// Class for Menu Items
class MenuItem {
private:
    string name;
    float price;
public:
    MenuItem() {}
    MenuItem(string n, float p) : name(n), price(p) {}

    void display() const {
        cout << "Menu Item: " << name << " | Price: Rs." << fixed << setprecision(2) << price << endl;
    }

    string getName() const { return name; }
    float getPrice() const { return price; }
};

// Class for Table Reservations
class Table {
private:
    int tableNumber;
    bool reserved;
public:
    Table(int num = 0) : tableNumber(num), reserved(false) {}

    void reserve() {
        if (!reserved) {
            reserved = true;
            cout << "Table " << tableNumber << " reserved.\n";
        } else {
            cout << "Table " << tableNumber << " is already reserved.\n";
        }
    }

    bool isReserved() const { return reserved; }
    int getTableNumber() const { return tableNumber; }
};

// Class for managing Orders
class Order {
private:
    vector<MenuItem> items;
    float total;

public:
    Order() : total(0) {}

    void addItem(const MenuItem& item) {
        items.push_back(item);
        total += item.getPrice();
    }

    void showOrder() const {
        cout << "Order Summary:\n";
        for (const auto& item : items) {
            item.display();
        }
        cout << "Total: Rs" << fixed << setprecision(2) << total << endl;
    }

    float getTotal() const {
        return total;
    }
};

// Class for generating Bills
class Billing {
public:
    static void generateBill(const Order& order) {
        float total = order.getTotal();
        float taxAmount = total * 0.05; // 5% tax
        float serviceCharge = total * 0.10; // 10% service charge
        float finalAmount = total + taxAmount + serviceCharge;

        cout << "Generating Bill...\n";
        cout << "Subtotal: Rs" << fixed << setprecision(2) << total << endl;
        cout << "Tax (5%): Rs" << taxAmount << endl;
        cout << "Service Charge (10%): Rs" << serviceCharge << endl;
        cout << "Total Amount: Rs" << finalAmount << endl;
    }
};

// Class for handling Feedback
class Feedback {
private:
    string message;
public:
    Feedback(string msg = "") : message(msg) {}

    void saveFeedback() {
        ofstream file("feedback.txt", ios::app);
        if (!file) {
            throw RestaurantException();
        }
        file << message << endl;
        file.close();
        cout << "Feedback saved.\n";
    }

    void loadFeedback() {
        ifstream file("feedback.txt");
        if (!file) {
            throw RestaurantException();
        }
        string line;
        cout << "Feedback:\n";
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }
};

// Class for Admin (with password check)
char username[20], password[20];

class Admin {
public:
    void display() {
        cout << "I am admin" << endl;
    }
} a;

class User {
public:
    void display() {
        cout << "I am user" << endl;
    }
} u;

void login() {
    if (strcmp(username, "dipesh") == 0 && strcmp(password, "12345") == 0) {
        a.display();
    } else {
        u.display();
    }
}

// Function to display the menu
void displayMenu(const vector<MenuItem>& menu) {
    cout << "Menu:\n";
    for (size_t i = 0; i < menu.size(); ++i) {
        cout << (i + 1) << ". ";
        menu[i].display();
    }
}

// Function to add menu items
void addMenuItem(vector<MenuItem>& menu) {
    string name;
    float price;
    cout << "Enter the name of the new menu item: ";
    cin >> name;
    cout << "Enter the price of the new menu item: ";
    cin >> price;
    menu.push_back(MenuItem(name, price));
    cout << "Menu item added successfully.\n";
}

// Function to delete menu items
void deleteMenuItem(vector<MenuItem>& menu) {
    int index;
    cout << "Enter the index of the menu item to delete (1 to " << menu.size() << "): ";
    cin >> index;
    if (index > 0 && index <= static_cast<int>(menu.size())) {
        menu.erase(menu.begin() + index - 1);
        cout << "Menu item deleted successfully.\n";
    } else {
        cout << "Invalid index!\n";
    }
}

// Function to handle admin actions
void adminActions(vector<MenuItem>& menu) {
    int choice;
    do {
        cout << "\nAdmin Actions:\n";
        cout << "1. Add Menu Item\n";
        cout << "2. Delete Menu Item\n";
        cout << "3. Display Menu\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addMenuItem(menu);
                break;
            case 2:
                deleteMenuItem(menu);
                break;
            case 3:
                displayMenu(menu);
                break;
            case 4:
                cout << "Exiting admin actions...\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 4);
}

// Function to display user actions
void userActions(const vector<MenuItem>& menu, vector<Table>& tables) {
    Order order;
    char addMore;

    cout << "\nDo you want to reserve a table? (y/n): ";
    cin >> addMore;
    if (addMore == 'y' || addMore == 'Y') {
        int tableNum;
        cout << "Enter table number (1-" << tables.size() << "): ";
        cin >> tableNum;
        if (tableNum > 0 && tableNum <= static_cast<int>(tables.size())) {
            tables[tableNum - 1].reserve();
        } else {
            cout << "Invalid table number!\n";
        }
    }

    do {
        int index;
        cout << "\nChoose a menu item to add to your order (by index starting from 1): ";
        cin >> index;
        if (index > 0 && index <= static_cast<int>(menu.size())) {
            order.addItem(menu[index - 1]);
        } else {
            cout << "Invalid index!" << endl;
        }
        cout << "Do you want to add more items? (y/n): ";
        cin >> addMore;
    } while (addMore == 'y' || addMore == 'Y');

    order.showOrder();
    Billing::generateBill(order);
}

// Main Function
int main() {
    try {
        // Sample Menu Items
        vector<MenuItem> menuItems = {
            MenuItem("Biryani", 625),
            MenuItem("Burger", 300),
            MenuItem("Pasta", 250),
            MenuItem("Salad", 400),
            MenuItem("Soda", 120)
        };

        // Sample Tables
        vector<Table> tables(10);
        for (int i = 0; i < 10; ++i) {
            tables[i] = Table(i + 1);
        }

        // User Login (Admin/User)
        cout << "Enter username and password for login:\n";
        cin >> username >> password;
        login();

        if (strcmp(username, "dipesh") == 0 && strcmp(password, "12345") == 0) {
            adminActions(menuItems);
        } else {
            userActions(menuItems, tables);
        }

        // Feedback System
        string feedbackMsg;
        cout << "\nEnter your feedback: ";
        cin.ignore(); // Clear input buffer
        getline(cin, feedbackMsg);
        Feedback feedback(feedbackMsg);
        feedback.saveFeedback();
        feedback.loadFeedback();

    } catch (const RestaurantException& e) {
        cout << e.what() << endl;
    }

    return 0;
}

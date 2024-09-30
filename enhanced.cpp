#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>

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
    MenuItem() : name("Unknown"), price(0.0) {}
    MenuItem(string n, float p) : name(n), price(p) {}

    void display() const {
        cout << "Menu Item: " << name << " | Price: Rs" << fixed << setprecision(2) << price << endl;
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
        reserved = true;
        cout << "Table " << tableNumber << " reserved.\n";
    }

    bool isReserved() const { return reserved; }
    int getTableNumber() const { return tableNumber; }
};

// Base class for User management
class User {
protected:
    string username;
    string password;
public:
    User(string u = "guest", string p = "password") : username(u), password(p) {}

    virtual bool login(const string& u, const string& p) {
        return (username == u && password == p);
    }

    virtual void displayInfo() const {
        cout << "User: " << username << endl;
    }
};

// Derived class for Admin Users
class Admin : public User {
public:
    Admin(string u, string p) : User(u, p) {}

    void displayInfo() const override {
        cout << "Admin: " << username << endl;
    }
};

// Derived class for Member Users
class Member : public User {
public:
    Member(string u, string p) : User(u, p) {}

    void displayInfo() const override {
        cout << "Member: " << username << endl;
    }
};

// Class for managing Orders
class Order {
private:
    MenuItem items[100]; // Fixed size for simplicity
    int itemCount;
    float total;

public:
    Order() : itemCount(0), total(0) {}

    void addItem(const MenuItem& item) {
        if (itemCount < 100) {
            items[itemCount++] = item;
            total += item.getPrice();
        } else {
            cout << "Order limit reached!" << endl;
        }
    }

    void showOrder() const {
        cout << "Order Summary:\n";
        for (int i = 0; i < itemCount; ++i) {
            items[i].display();
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

// Function to display the menu
void displayMenu(MenuItem menu[], int size) {
    cout << "Menu:\n";
    for (int i = 0; i < size; ++i) {
        cout << (i + 1) << ". ";
        menu[i].display();
    }
}

// Function to add menu items
void addMenuItem(MenuItem menu[], int& size) {
    if (size < 100) {
        string name;
        float price;
        cout << "Enter the name of the new menu item: ";
        cin >> name;
        cout << "Enter the price of the new menu item: ";
        cin >> price;
        menu[size++] = MenuItem(name, price);
        cout << "Menu item added successfully.\n";
    } else {
        cout << "Menu is full, cannot add more items.\n";
    }
}

// Function to delete menu items
void deleteMenuItem(MenuItem menu[], int& size) {
    int index;
    cout << "Enter the index of the menu item to delete (1 to " << size << "): ";
    cin >> index;
    if (index > 0 && index <= size) {
        for (int i = index - 1; i < size - 1; ++i) {
            menu[i] = menu[i + 1];
        }
        size--;
        cout << "Menu item deleted successfully.\n";
    } else {
        cout << "Invalid index!\n";
    }
}

// Function to handle admin actions
void adminActions(MenuItem menu[], int& menuSize) {
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
                addMenuItem(menu, menuSize);
                break;
            case 2:
                deleteMenuItem(menu, menuSize);
                break;
            case 3:
                displayMenu(menu, menuSize);
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
void userActions(const MenuItem menu[], int menuSize) {
    Order order;
    char addMore;
    do {
        int index;
        cout << "\nChoose a menu item to add to your order (by index starting from 1): ";
        cin >> index;
        if (index > 0 && index <= menuSize) {
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
        MenuItem menuItems[100];
        int numMenuItems = 0;

        // Predefined menu items
        menuItems[numMenuItems++] = MenuItem("Biryani", 625);
        menuItems[numMenuItems++] = MenuItem("Burger", 300);
        menuItems[numMenuItems++] = MenuItem("Pasta", 250);
        menuItems[numMenuItems++] = MenuItem("Salad", 400);
        menuItems[numMenuItems++] = MenuItem("Soda", 120);

        // User Login
        string username, password;
        cout << "Enter username for user login: ";
        cin >> username;
        cout << "Enter password for user login: ";
        cin >> password;

        User user(username, password);
        if (user.login(username, password)) {
            user.displayInfo();
            userActions(menuItems, numMenuItems);
        } else {
            cout << "Invalid credentials!" << endl;
            return 1;
        }

        // Admin Login
        string adminUsername, adminPassword;
        cout << "\nEnter admin username: ";
        cin >> adminUsername;
        cout << "Enter admin password: ";
        cin >> adminPassword;

        Admin admin(adminUsername, adminPassword);
        if (admin.login(adminUsername, adminPassword)) {
            admin.displayInfo();
            adminActions(menuItems, numMenuItems);
        } else {
            cout << "Invalid admin credentials!" << endl;
            return 1;
        }

        // Feedback System
        string feedbackMsg;
        cout << "\nEnter your feedback: ";
        cin.ignore(); // Clear input buffer
        getline(cin, feedbackMsg);
        Feedback feedback(feedbackMsg);
        feedback.saveFeedback();
        feedback.loadFeedback();

        // Exception Handling Demo
        throw RestaurantException();
    }
    catch (const RestaurantException& e) {
        cout << e.what() << endl;
    }

    return 0;
}
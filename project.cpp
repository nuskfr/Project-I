#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

// Base class for menu items
class MenuItem {
protected:
    string name;
    double price;
public:
    MenuItem(string n, double p) : name(n), price(p) {}
    virtual void display() {
        cout << name << ": Rs. " << price << endl;
    }
    double getPrice() {
        return price;
    }
    string getName() {
        return name;
    }
};

// Derived class for specific food items
class FoodItem : public MenuItem {
public:
    FoodItem(string n, double p) : MenuItem(n, p) {}
    void display() override {
        cout << name << ": Rs. " << price << endl;
    }
};

// Table reservation class
class Table {
private:
    int tableNumber;
    bool reserved;
public:
    Table(int num) : tableNumber(num), reserved(false) {}
    bool isReserved() {
        return reserved;
    }
    void reserve() {
        reserved = true;
        cout << "Table " << tableNumber << " reserved." << endl;
    }
};

// Order class to manage orders
class Order {
private:
    vector<MenuItem*> items;
public:
    void addItem(MenuItem* item) {
        items.push_back(item);
    }
    void showOrder() {
        cout << "Current Order:" << endl;
        for (auto item : items) {
            item->display();
        }
    }
    double calculateTotal() {
        double total = 0;
        for (auto item : items) {
            total += item->getPrice();
        }
        return total;
    }
    vector<MenuItem*> getItems() {
        return items;
    }
};

// Billing system
class Billing {
public:
    static void generateBill(Order& order) {
        cout << "Generating Bill..." << endl;
        order.showOrder();
        double total = order.calculateTotal();
        cout << "Total Amount: Rs. " << total << endl;
    }
};

// Customer feedback class
class Feedback {
public:
    static void saveFeedback(const string& feedback) {
        ofstream file("feedback.txt", ios::app);
        file << feedback << endl;
        file.close();
        cout << "Feedback saved." << endl;
    }
};

// Staff management class
class Staff {
private:
    string name;
    string position;
public:
    Staff(string n, string p) : name(n), position(p) {}
    void display() {
        cout << "Staff - Name: " << name << ", Position: " << position << endl;
    }
};

// Function to save orders to file
void saveOrderToFile(Order& order) {
    ofstream file("orders.txt", ios::app);
    file << "Order Details:" << endl;
    double total = 0;
    for (auto item : order.getItems()) {
        file << item->getName() << ": Rs. " << item->getPrice() << endl;
        total += item->getPrice();
    }
    file << "Total: Rs. " << total << endl << endl;
    file.close();
    cout << "Order saved to file." << endl;
}

// Main function
int main() {
    vector<MenuItem*> menu;
    menu.push_back(new FoodItem("Noodles", 120));
    menu.push_back(new FoodItem("Pizza", 650));
    menu.push_back(new FoodItem("Pasta", 240));
    menu.push_back(new FoodItem("Burger", 200));
    menu.push_back(new FoodItem("Biryani", 725));
    menu.push_back(new FoodItem("Chicken", 400));
    
    // Menu Management
    cout << "Welcome to Khwopa Canteen!" << endl;
    cout << "Menu Items:" << endl;
    for (int i = 0; i < menu.size(); ++i) {
        cout << i + 1 << ". ";
        menu[i]->display();
    }

    // Table Reservation
    int tableNum;
    cout << "Enter the table number you want to reserve (1-10): ";
    cin >> tableNum;
    Table table1(tableNum);
    table1.reserve();

    // Order Management
    Order order;
    char choice;
    do {
        int itemChoice;
        cout << "Enter the item number to add to your order (1-" << menu.size() << "): ";
        cin >> itemChoice;
        if (itemChoice > 0 && itemChoice <= menu.size()) {
            order.addItem(menu[itemChoice - 1]);
            cout << "Item added to order." << endl;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
        cout << "Do you want to add more items? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    // Show current order
    order.showOrder();

    // Billing
    Billing::generateBill(order);

    // Save Order to File
    saveOrderToFile(order);

    // Customer Feedback
    cin.ignore();  // Clear the input buffer
    string feedback;
    cout << "Enter your feedback: ";
    getline(cin, feedback);
    Feedback::saveFeedback(feedback);

    // Staff Management
    Staff staff1("Dheeraj", "Waiter");
    staff1.display();

    // Cleanup
    for (auto item : menu) {
        delete item;
    }

    return 0;
}
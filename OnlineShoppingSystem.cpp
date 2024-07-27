#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#define MaximumProducts 100
#define MaximumWishlist 100
#define MaximumReviews 100

using namespace std;

class User {
private:
    int userId;
    string name;
    string email;
    string password;
    string address;
    string phoneNumber;
    bool checklogin;

    void saveUserToFile() {
        ofstream file("user.txt");
        if (file.is_open()) {
            file << userId << "\n" << name << "\n" << email << "\n" << password << "\n" << address << "\n" << phoneNumber << "\n";
            file.close();
        }
    }

    bool loadUserFromFile() {
        ifstream file("user.txt");
        if (file.is_open()) {
            file >> userId;
            file.ignore();
            getline(file, name);
            getline(file, email);
            getline(file, password);
            getline(file, address);
            getline(file, phoneNumber);
            file.close();
            return true;
        }
        return false;
    }

public:
    User() {
        checklogin = false;
    }

    bool getCheckLogin() {
        return checklogin;
    }

    void registerUser() {
        cout << "\nEnter your details to Register:\n";
        cout << "Enter name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter email: ";
        cin >> email;
        cout << "Enter Password: ";
        cin >> password;
        cout << "Enter your Address: ";
        cin.ignore();
        getline(cin, address);
        cout << "Enter your Phone Number: ";
        cin >> phoneNumber;
        userId = rand() % 10000;
        saveUserToFile();
        cout << "\nRegistration successful. Your User ID is: " << userId << "\n";
    }

    bool login(string enteredEmail, string enteredPassword) {
        if (loadUserFromFile() && email == enteredEmail && password == enteredPassword) {
            cout << "\nLogin successful.\n";
            checklogin = true;
            return true;
        }
        cout << "\nLogin failed. Please check your email and password.\n";
        return false;
    }

    bool verification(string enteredEmail, string enteredPassword) {
        return (email == enteredEmail && password == enteredPassword);
    }
};

class Product {
private:
    string name;
    string code;
    double price;
    int quantity;
    string reviews[MaximumReviews];
    int reviewCount;

    void saveProductToFile() {
        ofstream file("products.txt", ios::app);
        if (file.is_open()) {
            file << name << "\n" << code << "\n" << price << "\n" << quantity << "\n";
            file.close();
        }
    }

    bool loadProductFromFile(string searchCode) {
        ifstream file("products.txt");
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                name = line;
                getline(file, code);
                file >> price;
                file >> quantity;
                file.ignore();
                if (code == searchCode) {
                    file.close();
                    return true;
                }
            }
            file.close();
        }
        return false;
    }

public:
    Product() {
        name = "";
        code = "";
        price = 0.0;
        quantity = 0;
        reviewCount = 0;
    }

    Product(string n, string c, double p, int q) {
        name = n;
        code = c;
        price = p;
        quantity = q;
        reviewCount = 0;
        saveProductToFile();
    }
    
    void addinfoProduct(string n, string c, double p, int q) {
        name = n;
        code = c;
        price = p;
        quantity = q;
        reviewCount = 0;
        saveProductToFile();
    }
    

    void addReview(string review) {
        if (reviewCount < MaximumReviews) {
            reviews[reviewCount++] = review;
        } else {
            cout << "\nCannot add more reviews.\n";
        }
    }

    void viewReviews() {
        cout << "Reviews for " << name << ":\n";
        for (int i = 0; i < reviewCount; i++) {
            cout << i + 1 << ". " << reviews[i] << "\n";
        }
    }

    string getCode() {
        return code;
    }

    string getName() {
        return name;
    }

    double getPrice() {
        return price;
    }

    int getQuantity() {
        return quantity;
    }

    static void viewAllProducts() {
        ifstream file("products.txt");
        if (file.is_open()) {
            string name, code;
            double price;
            int quantity;
            cout << "\nAvailable Products:\n";
            while (getline(file, name)) {
                getline(file, code);
                file >> price;
                file >> quantity;
                file.ignore();
                cout << "Name: " << name << ", Code: " << code << ", Price: $" << price << ", Quantity: " << quantity << "\n";
            }
            file.close();
        } else {
            cout << "\nNo products available.\n";
        }
    }
};

class Cart {
private:
    Product products[MaximumProducts];
    int productCount;

    void saveCartToFile() {
        ofstream file("cart.txt");
        if (file.is_open()) {
            file << productCount << "\n";
            for (int i = 0; i < productCount; i++) {
                file << products[i].getName() << "\n" << products[i].getCode() << "\n" << products[i].getPrice() << "\n" << products[i].getQuantity() << "\n";
            }
            file.close();
        }
    }

    void loadCartFromFile() {
        ifstream file("cart.txt");
        if (file.is_open()) {
            file >> productCount;
            file.ignore();
            for (int i = 0; i < productCount; i++) {
                string name, code;
                double price;
                int quantity;
                getline(file, name);
                getline(file, code);
                file >> price;
                file >> quantity;
                file.ignore();
                products[i] = Product(name, code, price, quantity);
            }
            file.close();
        } else {
            productCount = 0;
        }
    }

public:
    Cart() {
        loadCartFromFile();
    }

    ~Cart() {
        saveCartToFile();
    }

    void addToCart(Product p) {
        if (productCount < MaximumProducts) {
            products[productCount++] = p;
            cout << "\nProduct added to cart.\n";
        } else {
            cout << "\nCart is full.\n";
        }
    }

    void removeFromCart(string code) {
        for (int i = 0; i < productCount; i++) {
            if (products[i].getCode() == code) {
                for (int j = i; j < productCount - 1; j++) {
                    products[j] = products[j + 1];
                }
                productCount--;
                cout << "\nProduct removed from cart.\n";
                return;
            }
        }
        cout << "\nProduct not found in cart.\n";
    }

    void viewCart() {
        cout << "\nYour Cart:\n";
        for (int i = 0; i < productCount; i++) {
            cout << products[i].getName() << " - $" << products[i].getPrice() << " x " << products[i].getQuantity() << "\n";
        }
    }

    void showBill() {
        double total = 0.0;
        cout << "\nBill:\n";
        for (int i = 0; i < productCount; i++) {
            cout << products[i].getName() << " - $" << products[i].getPrice() << " x " << products[i].getQuantity() << "\n";
            total += products[i].getPrice() * products[i].getQuantity();
        }
        cout << "Total: $" << total << "\n";
    }

    void addProductReview(string code, string review) {
        for (int i = 0; i < productCount; i++) {
            if (products[i].getCode() == code) {
                products[i].addReview(review);
                return;
            }
        }
        cout << "\nProduct not found in cart.\n";
    }

    void viewProductReviews(string code) {
        for (int i = 0; i < productCount; i++) {
            if (products[i].getCode() == code) {
                products[i].viewReviews();
                return;
            }
        }
        cout << "\nProduct not found in cart.\n";
    }

    bool getProductDetails(string code, string &name, int &quantity, double &price) {
        for (int i = 0; i < productCount; i++) {
            if (products[i].getCode() == code) {
                name = products[i].getName();
                quantity = products[i].getQuantity();
                price = products[i].getPrice();
                return true;
            }
        }
        return false;
    }
};

class Wishlist {
private:
    Product products[MaximumWishlist];
    int productCount;

    void saveWishlistToFile() {
        ofstream file("wishlist.txt");
        if (file.is_open()) {
            file << productCount << "\n";
            for (int i = 0; i < productCount; i++) {
                file << products[i].getName() << "\n" << products[i].getCode() << "\n" << products[i].getPrice() << "\n" << products[i].getQuantity() << "\n";
            }
            file.close();
        }
    }

    void loadWishlistFromFile() {
        ifstream file("wishlist.txt");
        if (file.is_open()) {
            file >> productCount;
            file.ignore();
            for (int i = 0; i < productCount; i++) {
                string name, code;
                double price;
                int quantity;
                getline(file, name);
                getline(file, code);
                file >> price;
                file >> quantity;
                file.ignore();
                products[i] = Product(name, code, price, quantity);
            }
            file.close();
        } else {
            productCount = 0;
        }
    }

public:
    Wishlist() {
        loadWishlistFromFile();
    }

    ~Wishlist() {
        saveWishlistToFile();
    }

    void addToWishlist(Product p) {
        if (productCount < MaximumWishlist) {
            products[productCount++] = p;
            cout << "\nProduct added to wishlist.\n";
        } else {
            cout << "\nWishlist is full.\n";
        }
    }

    void removeFromWishlist(string code) {
        for (int i = 0; i < productCount; i++) {
            if (products[i].getCode() == code) {
                for (int j = i; j < productCount - 1; j++) {
                    products[j] = products[j + 1];
                }
                productCount--;
                cout << "\nProduct removed from wishlist.\n";
                return;
            }
        }
        cout << "\nProduct not found in wishlist.\n";
    }

    void viewWishlist() {
        cout << "\nWishlist:\n";
        for (int i = 0; i < productCount; i++) {
            cout << products[i].getName() << " - $" << products[i].getPrice() << " x " << products[i].getQuantity() << "\n";
        }
    }
};

class Support {
public:
    void contactSupport() {
        cout << "\nContacting support...\n";
        cout << "Support Email: support@example.com\n";
        cout << "Support Phone: 123-456-7890\n";
    }
};

class Admin : public User, public Product{ // there is possibilty to inherit user class for adding more info to admin
private:
	
    string adminUsername;
    string adminPassword;

    void saveAdminToFile() {
        ofstream file("admin.txt");
        if (file.is_open()) {
            file << adminUsername << "\n" << adminPassword << "\n";
            file.close();
        }
    }

    bool loadAdminFromFile() {
        ifstream file("admin.txt");
        if (file.is_open()) {
            getline(file, adminUsername);
            getline(file, adminPassword);
            file.close();
            return true;
        }
        return false;
    }

public:
    Admin() {
        adminUsername = "admin";
        adminPassword = "admin123";
        saveAdminToFile();
    }

    bool login(string enteredUsername, string enteredPassword) {
        if (loadAdminFromFile() && adminUsername == enteredUsername && adminPassword == enteredPassword) {
            cout << "\nAdmin login successful.\n";
            return true;
        }
        cout << "\nAdmin login failed. Please check your username and password.\n";
        return false;
    }

    void addProduct() {
        string name, code;
        int quantity;
        double price;
        cout << "Enter product name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter product code: ";
        cin >> code;
        cout << "Enter product price: ";
        cin >> price;
        cout << "Enter product quantity: ";
        cin >> quantity;
        addinfoProduct(name, code, price, quantity);
		cout << "\nProduct added successfully.\n";
    }
};

int main() {
    cout << endl;
    cout << "\t*****************************************************\n"
         << "\t**-------------------------------------------------**\n"
         << "\t**                                                 **\n"
         << "\t**        Welcome to Online Shopping System        **\n"
         << "\t**                                                 **\n"
         << "\t**-------------------------------------------------**\n"
         << "\t*****************************************************\n";
    User user;
    Cart cart;
    Wishlist wishlist;
    Support support;
    Admin admin;

    int choice;
    while (true) {
        cout << "\n1. Login\n";
        cout << "2. Signup\n";
        cout << "3. Admin Login\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string email, password;
            cout << "Enter email: ";
            cin >> email;
            cout << "Enter password: ";
            cin >> password;
            if (user.login(email, password)) {
                break;
            }
        } else if (choice == 2) {
            user.registerUser();
            cout << "Now login your account to proceed...\n";
        } else if (choice == 3) {
            string username, password;
            cout << "Enter admin username: ";
            cin >> username;
            cout << "Enter admin password: ";
            cin >> password;
            if (admin.login(username, password)) {
                while (true) {
                    cout << "\nAdmin Menu:\n";
                    cout << "1. Add Product\n";
                    cout << "2. Logout\n";
                    cout << "Enter your choice: ";
                    cin >> choice;
                    if (choice == 1) {
                        admin.addProduct();
                    } else if (choice == 2) {
                        break;
                    } else {
                        cout << "Invalid choice. Please try again.\n";
                    }
                }
            }
        } else if (choice == 4) {
            cout << "Exiting...\n";
            return 0;
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. View all available products\n";
        cout << "2. Add product to cart\n";
        cout << "3. Remove product from cart\n";
        cout << "4. View cart\n";
        cout << "5. Show bill\n";
        cout << "6. Checkout\n";
        cout << "7. Add product to wishlist\n";
        cout << "8. View wishlist\n";
        cout << "9. Remove product from wishlist\n";
        cout << "10. Add product review\n";
        cout << "11. View product reviews\n";
        cout << "12. Customer support\n";
        cout << "13. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                Product::viewAllProducts();
                break;
            }
            case 2: {
                string name, code;
                int quantity;
                double price;
                cout << "Enter product name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter product code: ";
                cin >> code;
                cout << "Enter product price: ";
                cin >> price;
                cout << "Enter product quantity: ";
                cin >> quantity;
                cart.addToCart(Product(name, code, price, quantity));
                break;
            }
            case 3: {
                string code;
                cout << "Enter product code to remove: ";
                cin >> code;
                cart.removeFromCart(code);
                break;
            }
            case 4: {
                cart.viewCart();
                break;
            }
            case 5: {
                cart.showBill();
                break;
            }
            case 6: {
                cart.showBill();
                break;
            }
            case 7: {
                string code, name;
                int quantity;
                double price;
                cout << "Enter product code to add to wishlist: ";
                cin >> code;
                bool found = cart.getProductDetails(code, name, quantity, price);
                if (!found) {
                    cout << "Product code not found. Try again.\n";
                    continue;
                }
                wishlist.addToWishlist(Product(name, code, price, quantity));
                break;
            }
            case 8: {
                wishlist.viewWishlist();
                break;
            }
            case 9: {
                string code;
                cout << "Enter product code to remove from wishlist: ";
                cin >> code;
                wishlist.removeFromWishlist(code);
                break;
            }
            case 10: {
                string code, review;
                cout << "Enter product code to add review: ";
                cin >> code;
                cout << "Enter your review: ";
                cin.ignore();
                getline(cin, review);
                cart.addProductReview(code, review);
                break;
            }
            case 11: {
                string code;
                cout << "Enter product code to view reviews: ";
                cin >> code;
                cart.viewProductReviews(code);
                break;
            }
            case 12: {
                support.contactSupport();
                break;
            }
            case 13: {
                cout << "Logging out...\n";
                return main();
            }
            default: {
                cout << "Invalid choice. Please try again.\n";
                break;
            }
        }
    }

    return 0;
}

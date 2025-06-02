#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

struct Discount {
    string code = "";
    int amount = 0;
};

class Exception {
private:
    string type;
    string reason;
public:
    Exception(string typ, string reas): type(typ), reason(reas) {}
    string send_message() { return type + " failed: " + reason + '\n'; }
};

class Admin {
private:
    string username;
    string password;
    vector<Discount> discounts;
    bool is_log;
public:
    Admin(string name, string pass): username(name), password(pass), is_log(false) {}
    void set_login(bool situation) { is_log = situation; }
    bool does_match_user_pass(string name, string pass) { return name == username && pass == password; }
    void add_discount(Discount discount) { discounts.push_back(discount); }
    vector<Discount> get_discounts() { return discounts; }
    bool get_login_situation() { return is_log; }
};

class Food {
private:
    string name;
    string category;
    int price;
    int cost;
public:
    Food(string nam, string cat, int pr, int cos): name(nam), category(cat), price(pr), cost(cos) {}
    string get_name() { return name; }
};

struct Items {
    string restaurant_name;
    Food food;
    int number = 0;
};

class User {
protected:
    string username;
    string password;
    int budget;
    bool is_log;
public:
    User(string name, string pass): username(name), password(pass), budget(0), is_log(false) {}
    bool does_pass_match(string pass) { return pass == password; }
    void increase_budget(int amount) { budget += amount; }
    void set_login(bool situation) { is_log = situation; }
    void set_new_password(string new_pass) { password = new_pass; }
    string get_username() { return username; }
    int get_budget() { return budget; }
    bool get_login_situation() { return is_log; }
};

class Customer: public User {
private:
    vector<Discount> discounts;
    vector<Items> cart;
public:
    Customer(string name, string pass): User(name, pass) {}
    void set_discount(Discount discount) { discounts.push_back(discount); }
    vector<Discount> get_discounts() { return discounts; }
};

class Restaurant: public User {
private:
    string type;
    vector<Food*> menu;
public:
    Restaurant(string name, string pass, string typ): User(name, pass) { type = typ; }
    Food* find_food_by_name(string name);
    void add_food_to_menu(Food* food) { menu.push_back(food); }
    void delete_food(string name);
    string get_type() { return type; }
};

class Snappfood {
private:
    bool is_run;
    Admin* admin;
    string menu;
    vector<Customer*> customers;
    vector<Restaurant*> restaurants;

    void go_to_menu(vector<string> words);
    void change_menu(string mnu) { menu = mnu; }
    User* find_user(string name);
    User* find_online_user();
    Customer* find_customer(string name);
    User* find_online_customer();
    Restaurant* find_restaurant(string name);
    User* find_online_restaurant();
    Customer* find_customer_by_discount(string code);
    void delete_user(string name);
public:
    Snappfood(): is_run(true), menu("login menu")  {}
    bool get_run_situation() { return is_run; }
    void set_admin_information();
    void get_command();
    void register_customer(string name, string pass);
    void login_user(string name, string pass);
    void change_password(string name, string old_pass, string new_pass);
    void remove_account(string name, string pass);
    void logout();
    void add_restaurant(string name, string pass, string type);
    void show_restaurant(vector<string> words);
    void remove_restaurant(string name);
    void set_discount(string name, int amount, string code);
    void show_discounts();
    void charge_account(int amount);
    void add_food(string name, string cat, int price, int cost);
    void remove_food(string name);
};

vector<string> separate_words(string line)
{
    string word = "";
    vector<string> words;
    for (int i = 0; i < line.size(); i ++)
    {
        if (line[i] != ' ' && line[i] != '\n')
            word += line[i];
        if (i == line.size() - 1 || (line[i] == ' ' && word.size() != 0))
        {
            words.push_back(word);
            word = "";
        }
    }
    return words;
}

bool is_user_pass_correct(string word)
{
    for (int i = 0; i < word.size(); i ++)
        if (!(word[i] ==  95 || (word[i] >= 48 && word[i] <= 57) || (word[i] >= 65 && word[i] <= 90) || (word[i] >= 97 && word[i] <= 122)))
            return false;
    return true;
}

bool is_type_correct(string type)
{
    for (int i = 0; i < type.size(); i ++)
        if (!(type[i] ==  95 || (type[i] >= 97 && type[i] <= 122)))
            return false;
    return true;
}

bool is_pass_strong(string word)
{
    int number = 0, small = 0, big = 0;
    for (int i = 0; i < word.size(); i ++)
    {
        if (word[i] >= 48 && word[i] <= 57)
            number ++;
        else if (word[i] >= 65 && word[i] <= 90)
            small ++;
        else if (word[i] >= 97 && word[i] <= 122)
            big ++;
    }
    if (word.size() < 5 || number < 1 || small < 1 || big < 1)
        return false;
    else
        return true;
}

Food* Restaurant::find_food_by_name(string name)
{
    for (int i = 0; i < menu.size(); i ++)
        if (name == menu[i]->get_name())
            return menu[i];
    return NULL;
}

void Restaurant::delete_food(string name)
{
    for (int i = 0; i < menu.size(); i ++)
    {
        if (name == menu[i]->get_name())
        {
            menu.erase(menu.begin() + i);
            return;
        }
    }
}

void Snappfood::go_to_menu(vector<string> words)
{
    if (words[1] == "customer" && words[2] == "menu")
    {
        if (find_online_customer() == NULL || menu != "main menu")
            throw Exception("enter menu", "access denied");
        cout << "enter menu successful. You are in the customer menu!" << endl;
        change_menu("customer menu");
    }
    else if (words[1] == "restaurant" && words[2] == "admin" && words[3] == "menu")
    {
        if (find_online_restaurant() == NULL || menu != "main menu")
            throw Exception("enter menu", "access denied");
        cout << "enter menu successful. You are in the restaurant admin menu!" << endl;
        change_menu("restaurant admin menu");
    }
    else if (words[1] == "Snappfood" && words[2] == "admin" && words[3] == "menu")
    {
        if (!admin->get_login_situation() || menu != "main menu")
            throw Exception("enter menu", "access denied");
        cout << "enter menu successful. You are in the Snappfood admin menu!" << endl;
        change_menu("Snappfood admin menu");
    }
    else
        throw Exception("enter menu", "invalid menu name");
}

User* Snappfood::find_user(string name)
{
    User* founded_user = NULL;
    for (int i = 0; i < customers.size(); i ++)
        if (customers[i]->get_username() == name)
            founded_user = customers[i];
    for (int i = 0; i < restaurants.size(); i ++)
        if (restaurants[i]->get_username() == name)
            founded_user = restaurants[i];
    return founded_user;
}

User* Snappfood::find_online_user()
{
    for (int i = 0; i < customers.size(); i ++)
        if (customers[i]->get_login_situation())
            return customers[i];
    for (int i = 0; i < restaurants.size(); i ++)
        if (restaurants[i]->get_login_situation())
            return restaurants[i];
    return NULL;
}

Customer* Snappfood::find_customer(string name)
{
    Customer* founded_customer = NULL;
    for (int i = 0; i < customers.size(); i ++)
        if (customers[i]->get_username() == name)
            founded_customer = customers[i];
    return founded_customer;
}

User* Snappfood::find_online_customer()
{
    for (int i = 0; i < customers.size(); i ++)
        if (customers[i]->get_login_situation())
            return customers[i];
    return NULL;
}

Restaurant* Snappfood::find_restaurant(string name)
{
    Restaurant* founded_restaurant = NULL;
    for (int i = 0; i < restaurants.size(); i ++)
        if (restaurants[i]->get_username() == name)
            founded_restaurant = restaurants[i];
    return founded_restaurant;
}

User* Snappfood::find_online_restaurant()
{
    for (int i = 0; i < restaurants.size(); i ++)
        if (restaurants[i]->get_login_situation())
            return restaurants[i];
    return NULL;
}

Customer* Snappfood::find_customer_by_discount(string code)
{
    for (int i = 0; i < customers.size(); i ++)
    {
        vector<Discount> discounts = customers[i]->get_discounts();
        for (int j = 0; j < discounts.size(); j ++)
            if (code == discounts[j].code)
                return customers[i];
    }
    return NULL;
}

void Snappfood::delete_user(string name)
{
    for (int i = 0; i < customers.size(); i ++)
    {
        if (customers[i]->get_username() == name)
        {
            customers.erase(customers.begin() + i);
            return;
        }
    }
    for (int i = 0; i < restaurants.size(); i ++)
    {
        if (restaurants[i]->get_username() == name)
        {
            restaurants.erase(restaurants.begin() + i);
            return;
        }
    }
}

void Snappfood::set_admin_information()
{
    string name, pass, trash;
    cin >> name >> pass;
    getline(cin, trash);
    admin = new Admin(name, pass);
}

void Snappfood::get_command()
{
    string line;
    getline(cin, line);
    vector<string> words = separate_words(line);
    if (words.size() == 2)
    {
        if (words[0] == "show" && words[1] == "restaurant")
        {
            if (menu != "Snappfood admin menu" && menu != "customer menu")
                cout << "show restaurant faild: access denied" << endl;
            else
                show_restaurant(words);
        }
        else if (words[0] == "show" && words[1] == "discounts")
        {
            try {
                show_discounts();
            }
            catch(Exception& exp) {
                cout << exp.send_message();
            }
        }
        else if (words[0] == "show" && words[1] == "balance")
        {
            User* founded_user = find_online_user();
            if (menu != "restaurant admin menu" && menu != "customer menu")
                cout << "show balance failed: access denied" << endl;
            else
                cout << founded_user->get_budget() << endl;
        }
        else
            cout << "invalid command!" << endl;
    }
    else if (words.size() == 3)
    {
        if (words[0] == "show" && words[1] == "current" && words[2] == "menu")
            cout << menu << endl;
        else if (words[0] == "register")
        {
            try {
                register_customer(words[1], words[2]);
                cout << "register successful" << endl;
                change_menu("login menu");
            }
            catch(Exception& exp) {
                cout << exp.send_message();
            }
        }
        else if (words[0] == "login")
        {
            try {
                login_user(words[1], words[2]);
                cout << "login successful" << endl;
                change_menu("main menu");
            }
            catch(Exception& exp) {
                cout << exp.send_message();
            }
        }
        else if (words[0] == "enter")
        {
            try {
                go_to_menu(words);
            }
            catch(Exception& exp) {
                cout << exp.send_message();
            }
        }
        else if (words[0] == "remove" && words[1] == "restaurant")
        {
            try {
                remove_restaurant(words[2]);
                cout << "remove restaurant successful" << endl;
            }
            catch(Exception& exp) {
                cout << exp.send_message();
            }
        }
        else if (words[0] == "charge" && words[1] == "account")
        {
            try {
                stringstream ss;
                int amount;
                ss << words[2];
                ss >> amount;
                charge_account(amount);
                cout << "charge account successful" << endl;
            }
            catch(Exception& exp) {
                cout << exp.send_message();
            }
        }
        else if (words[0] == "remove" && words[1] == "food")
        {
            try {
                remove_food(words[2]);
                cout << "remove food successful" << endl;
            }
            catch(Exception& exp) {
                cout << exp.send_message();
            }
        }
        else
            cout << "invalid command!" << endl;
    }
    else if (words.size() == 4)
    {
        if (words[0] == "show" && words[1] == "restaurant")
        {
            if (menu != "Snappfood admin menu" && menu != "customer menu")
                cout << "show restaurant faild: access denied" << endl;
            else
                show_restaurant(words);
        }
        else if (words[0] == "remove" && words[1] == "account")
        {
            try {
                remove_account(words[2], words[3]);
                cout << "remove account successful" << endl;
                change_menu("login menu");
            }
            catch(Exception& exp) {
                cout << exp.send_message();
            }
        }
        else if (words[0] == "enter")
        {
            try {
                go_to_menu(words);
            }
            catch(Exception& exp) {
                cout << exp.send_message();
            }
        }
        else
            cout << "invalid command!" << endl;
    }
    else if (words.size() == 5)
    {
        if (words[0] == "change" && words[1] == "password")
        {
            try {
                change_password(words[2], words[3], words[4]);
                cout << "password change successful" << endl;
                change_menu("login menu");
            }
            catch(Exception& exp) {
                cout << exp.send_message();
            }
        }
        else if (words[0] == "add" && words[1] == "restaurant")
        {
            try {
                add_restaurant(words[2], words[3], words[4]);
                cout << "add restaurant successful" << endl;
            }
            catch(Exception& exp) {
                cout << exp.send_message();
            }
        }
        else if (words[0] == "set" && words[1] == "discount")
        {
            try {
                stringstream ss;
                int amount;
                ss << words[3];
                ss >> amount;
                set_discount(words[2], amount, words[4]);
                cout << "set discount successful" << endl;
            }
            catch(Exception& exp) {
                cout << exp.send_message();
            }
        }
        else
            cout << "invalid command!" << endl;
    }
    else if (words.size() == 6)
    {
        if (words[0] == "add" && words[1] == "food")
        {
            try {
                stringstream ss1, ss2;
                int price, cost;
                ss1 << words[4];
                ss1 >> price;
                ss2 << words[5];
                ss2 >> cost;
                add_food(words[2], words[3], price, cost);
                cout << "add food successful" << endl;
            }
            catch(Exception& exp) {
                cout << exp.send_message();
            }
        }
        else
            cout << "invalid command!" << endl;
    }
    else if (words[0] == "logout")
    {
        try {
            logout();
            cout << "loggout successful" << endl;
            change_menu("login menu");
        }
        catch(Exception& exp) {
            cout << exp.send_message();
        }
    }
    else if (words[0] == "exit")
    {
        is_run = false;
        cout << "goodbye!";
    }
    else
        cout << "invalid command!" << endl;
}

void Snappfood::register_customer(string name, string pass)
{
    if (menu != "login menu")
        throw Exception("register", "access denied");
    else if (!is_user_pass_correct(name))
        throw Exception("register", "invalid username format");
    else if (find_user(name) != NULL || admin->does_match_user_pass(name, pass))
        throw Exception("register", "username already exists");
    else if (!is_user_pass_correct(pass))
        throw Exception("register", "invalid password format");
    else if (!is_pass_strong(pass))
        throw Exception("register", "weak password");
    else if (find_online_user() != NULL || admin->get_login_situation())
        throw Exception("register", "already logged in");
    Customer* new_customer = new Customer(name, pass);
    customers.push_back(new_customer);
}

void Snappfood::login_user(string name, string pass)
{
    if (menu != "login menu")
        throw Exception("login", "access denied");
    if (admin->does_match_user_pass(name, pass))
    {
        if (find_online_user() != NULL || admin->get_login_situation() == true)
            throw Exception("login", "already logged in");
        admin->set_login(true);
        return;
    }
    User* founded_user = find_user(name);
    if (founded_user == NULL)
        throw Exception("login", "username not found");
    else if (!founded_user->does_pass_match(pass))
        throw Exception("login", "incorrect password");
    else if (find_online_user() != NULL || admin->get_login_situation() == true)
        throw Exception("login", "already logged in");
    founded_user->set_login(true);
}

void Snappfood::change_password(string name, string old_pass, string new_pass)
{
    User* founded_user = find_user(name);
    if (menu != "login menu")
        throw Exception("password change", "access denied");
    else if (founded_user == NULL)
        throw Exception("password change", "username not found");
    else if (!founded_user->does_pass_match(old_pass))
        throw Exception("password change", "incorrect password");
    else if (!is_user_pass_correct(new_pass))
        throw Exception("password change", "invalid new password");
    else if (!is_pass_strong(new_pass))
        throw Exception("password change", "weak new password");
    else if (find_online_user() != NULL || admin->get_login_situation())
        throw Exception("password change", "already logged in");
    founded_user->set_new_password(new_pass);
}

void Snappfood::remove_account(string name, string pass)
{
    User* founded_user = find_user(name);
    if (menu != "login menu")
        throw Exception("remove account", "access denied");
    else if (founded_user == NULL)
        throw Exception("remove account", "username not found");
    else if (!founded_user->does_pass_match(pass))
        throw Exception("remove account", "incorrect password");
    else if (find_online_user() != NULL || admin->get_login_situation())
        throw Exception("remove account", "already logged in");
    delete_user(name);
    delete founded_user;
}

void Snappfood::logout()
{
    User* founded_user = find_online_user();
    if (founded_user == NULL && !admin->get_login_situation())
        throw Exception("logout", "access denied");
    if (founded_user != NULL)
        founded_user->set_login(false);
    else if (admin->get_login_situation())
        admin->set_login(false);
    change_menu("login menu");
}

void Snappfood::add_restaurant(string name, string pass, string type)
{
    if (menu != "Snappfood admin menu")
        throw Exception("add restaurant", "access denied");
    else if (!is_user_pass_correct(name))
        throw Exception("add restaurant", "invalid username format");
    else if (find_user(name) != NULL || admin->does_match_user_pass(name, pass))
        throw Exception("add restaurant", "username already exists");
    else if (!is_user_pass_correct(pass))
        throw Exception("add restaurant", "invalid password format");
    else if (!is_pass_strong(pass))
        throw Exception("add restaurant", "weak password");
    else if (!is_type_correct(type))
        throw Exception("add restaurant", "invalid type format");
    Restaurant* new_restaurant = new Restaurant(name, pass, type);
    restaurants.push_back(new_restaurant);
}

void Snappfood::show_restaurant(vector<string> words)
{
    if (words.size() == 2)
        for (int i = 0; i < restaurants.size(); i ++)
        {
            cout << i + 1 << ") " << restaurants[i]->get_username() << ": type=" << restaurants[i]->get_type();
            if (menu == "Snappfood admin menu")
                cout << " balance=" << restaurants[i]->get_budget();
            cout << endl;
        }
    else if (words.size() == 4 && words[2] == "-t")
    {
        int index = 1;
        for (int i = 0; i < restaurants.size(); i ++)
            if (restaurants[i]->get_type() == words[3])
            {
                cout << index << ") " << restaurants[i]->get_username() << ": type=" << restaurants[i]->get_type();
                if (menu == "Snappfood admin menu")
                    cout << " balance=" << restaurants[i]->get_budget();
                cout << endl;
                index ++;
            }
    }
    else
        cout << "invalid command!" << endl;
}

void Snappfood::remove_restaurant(string name)
{
    User* founded_user = find_user(name);
    if (menu != "Snappfood admin menu")
        throw Exception("remove restaurant", "access denied");
    else if (founded_user == NULL)
        throw Exception("remove restaurant", "restaurant not found");
    delete_user(name);
    delete founded_user;
}

void Snappfood::set_discount(string name, int amount, string code)
{
    Customer* founded_customer = find_customer(name);
    if (menu != "Snappfood admin menu")
        throw Exception("set discount", "access denied");
    else if (founded_customer == NULL)
        throw Exception("set discount", "username not found");
    else if (amount <= 0)
        throw Exception("set discount", "invalid amount");
    else if (!is_user_pass_correct(code))
        throw Exception("set discount", "invalid code format");
    Discount new_discount;
    new_discount.code = code;
    new_discount.amount = amount;
    admin->add_discount(new_discount);
    founded_customer->set_discount(new_discount);
}

void Snappfood::show_discounts()
{   
    if (menu != "Snappfood admin menu")
        throw Exception("show discounts", "access denied");
    vector<Discount> discounts = admin->get_discounts();
    for (int i = 0; i < discounts.size(); i ++)
    {
        Customer* founded_customer = find_customer_by_discount(discounts[i].code);
        cout << i + 1 << ") " << discounts[i].code << " | amount=" << discounts[i].amount << " --> user=" << founded_customer->get_username() << endl;
    }
}

void Snappfood::charge_account(int amount)
{
    if (menu != "restaurant admin menu" && menu != "customer menu")
        throw Exception("charge account", "access denied");
    else if (amount <= 0)
        throw Exception("charge account", "invalid cost or amount");
    User* founded_user = find_online_user();
    founded_user->increase_budget(amount);
}

void Snappfood::add_food(string name, string cat, int price, int cost)
{
    if (menu != "restaurant admin menu")
        throw Exception("add food", "access denied");
    User* founded_user = find_online_restaurant();
    Restaurant* founded_restaurant = find_restaurant(founded_user->get_username());
    if (cat != "starter" && cat != "entree" && cat != "dessert")
        throw Exception("add food", "invalid category");
    else if (!is_type_correct(name))
        throw Exception("add food", "invalid food name");
    else if (founded_restaurant->find_food_by_name(name) != NULL)
        throw Exception("add food", "food already exists");
    else if (price <= 0 || cost <= 0)
        throw Exception("add food", "invalid cost or price");
    Food* new_food = new Food(name, cat, price, cost);
    founded_restaurant->add_food_to_menu(new_food);
}

void Snappfood::remove_food(string name)
{
    if (menu != "restaurant admin menu")
        throw Exception("remove food", "access denied");
    User* founded_user = find_online_restaurant();
    Restaurant* founded_restaurant = find_restaurant(founded_user->get_username());
    Food* founded_food = founded_restaurant->find_food_by_name(name);
    if (founded_food == NULL)
        throw Exception("remove food", "food not found");
    founded_restaurant->delete_food(name);
    delete founded_food;
}

int main()
{
    Snappfood snappfood;
    snappfood.set_admin_information();
    while(snappfood.get_run_situation() == true)
        snappfood.get_command();
}
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <functional>
#include <algorithm> // for remove_if

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

using namespace std;

// 清屏函数
void clearScreen() {
#ifdef _WIN32
    system("cls"); // windows bat 清屏命令
#else
    system("clear"); // 类unix shell 清屏命令
#endif
}

/*
 *  ============================================
 *                  简易文件数据库实现
 *  ============================================
 */

// 用户类
class User {
    private:
    string account; // 账户
    string password; // 密码
    string role; // 角色: admin-管理员, user-用户
    public:
    // 获取私有成员
    string getAccount() const { return account; }
    string getPassword() const { return password; }
    string getRole() const { return role; }
    // 设置私有成员
    void setAccount(const string& acc) { account = acc; }
    void setPassword(const string& pwd) { password = pwd; }
    void setRole(const string& rl) { role = rl; }
};

class Book {
    private:
    string name; // 书籍名称
    string isbn; // ISBN/ISSN编号
    string author; // 作者
    string publish; // 出版社
    string price; // 价格
    int capacity; // 库存
    int borrow; // 借阅次数
    public:
    // 获取私有成员
    string getName() const { return name; }
    string getISBN() const { return isbn; }
    string getAuthor() const { return author; }
    string getPublish() const { return publish; }
    string getPrice() const { return price; }
    int getCapacity() const { return capacity; }
    int getBorrow() const { return borrow; }
    // 设置私有成员
    void setName(const string& n) { name = n; }
    void setISBN(const string& i) { isbn = i; }
    void setAuthor(const string& a) { author = a; }
    void setPublish(const string& p) { publish = p; }
    void setPrice(const string& p) { price = p; }
    void setCapacity(int c) { capacity = c; }
    void setBorrow(int b) { borrow = b; }
};

class UserBookMap {
    private:
    string account; // 借阅账户名称
    string isbn; // 借阅书籍ISBN/ISSN编号
    public:
    // 获取私有成员
    string getAccount() const { return account; }
    string getISBN() const { return isbn; }
    // 设置私有成员
    void setAccount(const string& c) { account = c; }
    void setISBN(const string& i) { isbn = i; }
};

 
class UserDb {
    private:
    string filename;
    vector<User> users;
    public:
    UserDb(const string &fn): filename(fn) {}

    bool load() {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "无法打开文件: " << filename << endl;
            return false;
        }

        string line;
        while (getline(file, line)) {
            User user;
            stringstream ss(line);
            string field;

            getline(ss, field, ',');
            user.setAccount(field);
            getline(ss, field, ',');
            user.setPassword(field);
            getline(ss, field, ',');
            user.setRole(field);

            users.push_back(user);
        }

        file.close();
        return true;
    }

    // 保存记录到文件
    bool save() {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "无法打开文件: " << filename << endl;
            return false;
        }

        for (const User& user : users) {
            file << user.getAccount() << "," << user.getPassword() << "," << user.getRole() << "\n";
        }

        file.close();
        return true;
    }

    void listUsers() {
        cout << "|                账户 |    角色 |" << endl;
        cout << "|  ================== | ======= |" << endl;
        for (User& user : users) {
            cout << "|" << setw(20) << user.getAccount() << " |" << setw(8) << user.getRole() << " |" << endl;
        }
        cout << "共 " << users.size() << " 条记录" << endl;
    }

    void findUser(const string& account) {
        for (User& user : users) {
            if (user.getAccount() == account) {
                cout << "|                账户 |    角色 |" << endl;
                cout << "|  ================== | ======= |" << endl;
                cout << "|" << setw(20) << user.getAccount() << " |" << setw(8) << user.getRole() << " |" << endl;
                return;
            }
        }
        cout << "查询用户失败, 用户(" + account + ")不存在" << endl;
    }

    User* checkUser(const string& account) {
        for (User& user : users) {
            if (user.getAccount() == account) {
                return &user;
            }
        }
        return nullptr;
    }

    // 添加记录
    void addUser(const string& account, const string& password, const string& role) {
        if (checkUser(account) != nullptr) {
            cout << "创建用户失败, 用户(" + account + ") 已经存在" << endl;
            return;
        }
        if (role != "admin" && role != "user") {
            cout << "创建用户失败, 角色必须是admin或者user" << endl;
            return;
        }
        User user;
        user.setAccount(account);
        user.setPassword(password);
        user.setRole(role);

        users.push_back(user);
        if (save()) {
            cout << "创建用户成功" << endl;
        } else {
            cout << "创建用户失败, 文件写入异常" << endl;
        }
    }

    // 删除记录
    void deleteUser(const string& account) {
        if (checkUser(account) == nullptr) {
            cout << "删除用户失败, 用户(" + account + ")不存在" << endl;
            return;
        }
        users.erase(remove_if(users.begin(), users.end(), [account](const User& user) {
            return user.getAccount() == account;
        }), users.end());
        if (save()) {
            cout << "删除用户成功" << endl;
        } else {
            cout << "删除用户失败, 文件写入异常" << endl;
        }
    }

    // 更新记录
    void updateUser(const string& account, const string& password, const string& role) {
        if (role != "admin" && role != "user") {
            cout << "更新用户失败, 角色必须是admin或user" << endl;
            return;
        }
        for (User& user : users) {
            if (user.getAccount() == account) {
                user.setPassword(password);
                user.setRole(role);
                if (save()) {
                    cout << "更新用户成功" << endl;
                } else {
                    cout << "更新用户失败, 文件写入异常" << endl;
                }
                return;
            }
        }
        cerr << "更新失败, 用户(" + account + ")不存在: " << endl;
    }
};

class BookDb {
    private:
    string filename;
    vector<Book> books;
    public:
    BookDb(const string &fn): filename(fn) {}

    bool load() {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "无法打开文件: " << filename << endl;
            return false;
        }

        string line;
        while (getline(file, line)) {
            Book book;
            stringstream ss(line);
            string field;

            getline(ss, field, ',');
            book.setName(field);
            getline(ss, field, ',');
            book.setISBN(field);
            getline(ss, field, ',');
            book.setAuthor(field);
            getline(ss, field, ',');
            book.setPublish(field);
            getline(ss, field, ',');
            book.setPrice(field);
            getline(ss, field, ',');
            book.setCapacity(stoi(field));
            getline(ss, field, ',');
            book.setBorrow(stoi(field));

            books.push_back(book);
        }

        file.close();
        return true;
    }

    // 保存记录到文件
    bool save() {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "无法打开文件: " << filename << endl;
            return false;
        }

        for (const Book& book : books) {
            file << book.getName() << "," << book.getISBN() << "," << book.getAuthor() << "\n";
        }

        file.close();
        return true;
    }

    void printHeader() {
        cout << "|            书籍名称 |                ISBN |      作者 |              出版社 |      价格 |    库存 | 借阅次数 |" << endl;
        cout << "|  ================== | =================== | ========= | =================== | ========= | ======= | ======== |" << endl;
    }
    void printBody(Book& book) {
        cout << "|" << setw(20) << book.getName() << " |" << setw(20) << book.getISBN();
        cout << " |" << setw(10) << book.getAuthor() << " |" << setw(20) << book.getPublish();
        cout << " |" << setw(10) << book.getPrice() << " |" << setw(8) << book.getCapacity();
        cout << " |" << setw(9) << book.getBorrow() << " |" << endl;
    }
    void printTotal(int total) {
        cout << "共 " << total << " 条记录" << endl;
    }

    void listBooks() {
        printHeader();
        for (Book& book : books) { printBody(book); }
        printTotal(int(books.size()));
    }

    void listBooksForName(const string& name) {
        int count = 0;
        printHeader();
        for (Book& book : books) {
            if (book.getName() != name) { continue; }
            printBody(book);
            count++;
        }
        printTotal(count);
    }

    void findBookForISBN(const string& isbn) {
        for (Book& book : books) {
            if (book.getISBN() == isbn) {
                printHeader();
                printBody(book);
                return;
            }
        }
        cout << "查询书籍失败, 书籍ISBN/ISSN编号(" << isbn << ")不存在" << endl;
    }

    void listBookForAuthor(const string& author) {
        int count = 0;
        printHeader();
        for (Book& book : books) {
            if (book.getAuthor() != author) { continue; }
            printBody(book);
            count++;
        }
        printTotal(count);
    }

    void listBooksByBorrow(int limit) {
        vector<Book> copy(books);
        sort(copy.begin(), copy.end(), [](Book a, Book b) {
            return a.getBorrow() < b.getBorrow();
        });
        int count = 0;
        printHeader();
        for (size_t i = 0; i < min(int(copy.size()), limit) ; ++i) {
            printBody(copy[i]);
            count++;
        }
        printTotal(count);
    }

    Book* checkBook(const string& isbn) {
        for (Book& book : books) {
            if (book.getISBN() == isbn) {
                return &book;
            }
        }
        return nullptr;
    }

    // 添加记录
    void addBook(const string& name, const string& isbn, const string& author, const string& publish, const string& price, int capacity) {
        if (checkBook(isbn) != nullptr) {
            cout << "新增书籍失败, 书籍(" << isbn << ")已经存在" << endl;
            return;
        }
        Book book;
        book.setName(name);
        book.setISBN(isbn);
        book.setAuthor(author);
        book.setPublish(publish);
        book.setPrice(price);
        book.setCapacity(capacity);
        book.setBorrow(0);

        books.push_back(book);
        if (save()) {
            cout << "新增书籍成功" << endl;
        } else {
            cout << "新增书籍失败, 文件写入异常" << endl;
        }
    }

    // 删除记录
    void deleteBook(const string& isbn) {
        if (checkBook(isbn) == nullptr) {
            cout << "删除书籍失败, 书籍(" << isbn << ")不存在" << endl;
            return;
        }
        books.erase(remove_if(books.begin(), books.end(), [isbn](const Book& book) {
            return book.getISBN() == isbn;
        }), books.end());
        if (save()) {
            cout << "删除书籍成功" << endl;
        } else {
            cout << "删除书籍失败, 文件写入异常" << endl;
        }
    }

    // 更新记录
    void updateBook(const string& name, const string& isbn, const string& author, const string& publish, const string& price, int capacity) {
        for (Book& book : books) {
            if (book.getISBN() == isbn) {
                book.setName(name);
                book.setAuthor(author);
                book.setPublish(publish);
                book.setPrice(price);
                book.setCapacity(capacity);
                if (save()) {
                    cout << "更新书籍成功" << endl;
                } else {
                    cout << "更新书籍失败, 文件写入异常" << endl;
                }
                return;
            }
        }
        cerr << "更新书籍失败, 书籍(" << isbn << ")未找到: " << endl;
    }

    bool borrowBook(const string& isbn) {
        for (Book& book : books) {
            if (book.getISBN() == isbn) {
                if (book.getCapacity() == 0) {
                    cout << "借阅书籍失败, 库存不足" << endl;
                    return false;
                }
                book.setCapacity(book.getCapacity() - 1);
                book.setBorrow(book.getBorrow() + 1);
                if (save()) {
                    cout << "借阅书籍成功" << endl;
                } else {
                    cout << "借阅书籍失败, 文件写入异常" << endl;
                    return false;
                }
                return true;
            }
        }
        cerr << "借阅书籍失败, 书籍(" << isbn << ")未找到: " << endl;
        return false;
    }

    bool returnBook(const string& isbn) {
        for (Book& book : books) {
            if (book.getISBN() == isbn) {
                book.setCapacity(book.getCapacity() + 1);
                if (save()) {
                    cout << "归还书籍成功" << endl;
                } else {
                    cout << "归还书籍失败, 文件写入异常" << endl;
                    return false;
                }
                return true;
            }
        }
        cerr << "归还书籍失败, 书籍(" << isbn << ")未找到: " << endl;
        return false;
    }
};

class UserBookMapDb {
    private:
    string filename;
    vector<UserBookMap> userBookMaps;
    public:
    UserBookMapDb(const string &fn): filename(fn) {}

    bool load() {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "无法打开文件: " << filename << endl;
            return false;
        }

        string line;
        while (getline(file, line)) {
            UserBookMap userBookMap;
            stringstream ss(line);
            string field;

            getline(ss, field, ',');
            userBookMap.setAccount(field);
            getline(ss, field, ',');
            userBookMap.setISBN(field);

            userBookMaps.push_back(userBookMap);
        }

        file.close();
        return true;
    }

    // 保存记录到文件
    bool save() {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "无法打开文件: " << filename << endl;
            return false;
        }

        for (const UserBookMap& userBookMap : userBookMaps) {
            file << userBookMap.getAccount() << "," << userBookMap.getISBN()  << "\n";
        }

        file.close();
        return true;
    }

    UserBookMap* checkUserBookMap(const string& account, const string& isbn) {
        for (UserBookMap& userBookMap : userBookMaps) {
            if (userBookMap.getAccount() == account && userBookMap.getISBN() == isbn) {
                return &userBookMap;
            }
        }
        return nullptr;
    }

    void listBooksForUser(const string& account) {
        cout << "|    ISBN  |" << endl;
        cout << "| =======  |" << endl;
        int count = 0;
        for (UserBookMap& userBookMap : userBookMaps) {
            if (userBookMap.getAccount() == account) {
                cout << "|" << setw(8) << userBookMap.getISBN() << " |" << endl;
                count++;
            }
        }
        cout << "共 " << count << " 条记录" << endl;
    }

    // 添加记录
    void addUserBookMap(const string& account, const string& isbn) {
        if (checkUserBookMap(account, isbn) != nullptr) {
            cout << "登记借阅失败, 用户(" + account + ") 已经拥有书籍("<< isbn << ")" << endl;
            return;
        }
        UserBookMap userBookMap;
        userBookMap.setAccount(account);
        userBookMap.setISBN(isbn);

        userBookMaps.push_back(userBookMap);
        if (save()) {
            cout << "登记借阅成功" << endl;
        } else {
            cout << "登记借阅失败, 文件写入异常" << endl;
        }
    }

    // 删除记录
    void deleteUserBookMap(const string& account, const string& isbn) {
        if (checkUserBookMap(account, isbn) == nullptr) {
            cout << "登记归还失败, 用户(" + account + ") 未借阅书籍("<< isbn << ")" << endl;
            return;
        }
        userBookMaps.erase(remove_if(userBookMaps.begin(), userBookMaps.end(), [account, isbn](const UserBookMap& userBookMap) {
            return userBookMap.getAccount() == account && userBookMap.getISBN() == isbn;
        }), userBookMaps.end());
        if (save()) {
            cout << "登记归还成功" << endl;
        } else {
            cout << "登记归还失败, 文件写入异常" << endl;
        }
    }

};

/*
 *  ============================================
 *                  多级菜单实现
 *  ============================================
 */

// 菜单项结构
struct MenuItem {
    string name;                      // 菜单项名称
    function<void()> action;          // 菜单项对应的操作

    MenuItem(const string &name, function<void()> action)
        : name(name), action(action) {}
};

// 菜单类
class Menu {
    private:
    string title;                                    // 菜单标题
    vector<MenuItem> items;                         // 当前菜单的选项
    Menu* parentMenu;                                    // 指向父菜单的指针

    public:
    Menu(const string &title, Menu* parent = nullptr)
        : title(title), parentMenu(parent) {}

    ~Menu() {
    }

    // 添加菜单项
    void addItem(const string &name, function<void()> action) {
        items.emplace_back(name, action);
    }

    // 删除菜单项
    void removeItem(const string &name) {
        items.erase(remove_if(items.begin(), items.end(),
                    [&name](const MenuItem &item) { return item.name == name; }),
                    items.end());
    }

    // 显示菜单
    void showMenu(string who = "guest") {
        cout << "\n-------------------------------" << endl;
        for (size_t i = 0; i < items.size(); ++i) {
            cout << i + 1 << ". " << items[i].name << endl;
        }
        if (parentMenu) {
            cout << "0. 返回上一级" << endl;
        } else {
            cout << "0. 退出系统" << endl;
        }
        cout << "\n" << "[" << who << "]" << title << ">: ";
    }

    Menu* handleChoice(int choice) {
        clearScreen();

        if (choice == 0) {
            if (parentMenu) {
                return parentMenu; // 返回父菜单
            } else {
                exit(0);
            }
        } else if (choice > 0 && choice <= items.size()) {
            items[choice - 1].action();
        } else {
            cout << "无效的选择，请重试。" << endl;
        }
        return nullptr;
    }
};


/*
 *  ============================================
 *                  系统运行逻辑
 *  ============================================
 */


class BookSys {
    private:
    Menu* menu;
    UserDb* userDb;
    BookDb* bookDb;
    UserBookMapDb *userBookMapDb;
    User* currentUser;

    public:
    BookSys(UserDb *ud, BookDb *bd, UserBookMapDb *ubmd) {
        userDb = ud;
        bookDb = bd;
        userBookMapDb = ubmd;

        currentUser = new User();
        currentUser->setAccount("guest");

        addMainMenu();
    }

    string input(string desc) {
        string temp;
        cout << "请输入" << desc << ": ";
        getline(cin, temp);
        return temp;
    }

    void login() {
        cin.ignore();
        string account = input("账户名称");
        string password = input("账户" + account + "的密码");
        clearScreen();

        User* user = userDb->checkUser(account);
        if (user == nullptr) {
            cout << "登录失败, 账号不存在" << endl;
            return;
        }
        string pwd = user->getPassword();
        if ( pwd != password) {
            cout << "登录失败, 密码错误" << endl;
            return;
        }

        cout << "登录成功，您的角色为：" << user->getRole() << endl;
        currentUser = user;
        menu->removeItem("登录");
        if (user->getRole() == "admin") {
            addUserMgrMenu();
            addBookMgrMenu();
            addBookSearchMenu();
        } else {
            addBookSearchMenu();
            addUserBookMapMgrMenu();
        }
    }

    void addMainMenu() {
        menu = new Menu("主菜单");
        menu->addItem("登录", [this]() { this->login(); });
    }

    void addUserMgrMenu() {
        Menu *userMenu = new Menu("账户管理", menu);
        userMenu->addItem("列出所有账户", [this]() {
            this->userDb->listUsers();
        });
        userMenu->addItem("查询账户", [this]() {
            cin.ignore();
            string account = input("账户名称");
            clearScreen();
            this->userDb->findUser(account);
        });
        userMenu->addItem("新增账户", [this]() {
            cin.ignore();
            string account = input("账户名称");
            string password = input("账户" + account + "的密码");
            string role = input("账户" + account + "的role");
            clearScreen();
            this->userDb->addUser(account, password, role);
        });
        userMenu->addItem("删除账户", [this]() {
            cin.ignore();
            string account = input("账户名称");
            clearScreen();
            this->userDb->deleteUser(account);
        });
        userMenu->addItem("更新账户", [this]() {
            cin.ignore();
            string account = input("账户名称");
            string password = input("账户" + account + "的密码");
            string role = input("账户" + account + "的role");
            clearScreen();
            this->userDb->updateUser(account, password, role);
        });
        menu->addItem("账户管理", [this, userMenu]() {
            cout << "开始账户管理" << endl;
            this->menu=userMenu;
        });

    }

    void addBookMgrMenu() {
        Menu *bookMgrMenu = new Menu("书籍管理", menu);
        bookMgrMenu->addItem("列出所有书籍", [this]() {
            this->bookDb->listBooks();
        });
        bookMgrMenu->addItem("新增书籍", [this]() {
            cin.ignore();
            string isbn = input("书籍ISBN号");
            string name = input("书籍(" + isbn + ")的名称");
            string author = input("书籍(" + isbn + ")的作者");
            string publish = input("书籍(" + isbn + ")的出版社");
            string price = input("书籍(" + isbn + ")的价格");
            string capacity = input("书籍(" + isbn + ")的库存");
            clearScreen();
            this->bookDb->addBook(name, isbn, author, publish, price, stoi(capacity));
        });
        bookMgrMenu->addItem("删除书籍", [this]() {
            cin.ignore();
            string isbn = input("请输入书籍ISBN/ISSN编号");
            clearScreen();
            this->bookDb->deleteBook(isbn);
        });
        bookMgrMenu->addItem("更新书籍", [this]() {
            cin.ignore();
            string isbn = input("书籍ISBN号");
            string name = input("书籍(" + isbn + ")的名称");
            string author = input("书籍(" + isbn + ")的作者");
            string publish = input("书籍(" + isbn + ")的出版社");
            string price = input("书籍(" + isbn + ")的价格");
            string capacity = input("书籍(" + isbn + ")的库存");
            clearScreen();
            this->bookDb->updateBook(name, isbn, author, publish, price, stoi(capacity));
        });
        menu->addItem("书籍管理", [this, bookMgrMenu]() {
            cout << "开始书籍管理" << endl;
            this->menu=bookMgrMenu;
        });
    }

    void addBookSearchMenu() {
        Menu *bookSearchMenu = new Menu("书籍查找", menu);
        bookSearchMenu->addItem("书名查找", [this]() {
            cin.ignore();
            string name = input("书籍名称");
            clearScreen();
            this->bookDb->listBooksForName(name);
        });
        bookSearchMenu->addItem("ISBN/ISSN编号查找", [this]() {
            cin.ignore();
            string isbn = input("书籍ISBN号");
            clearScreen();
            this->bookDb->findBookForISBN(isbn);
        });
        bookSearchMenu->addItem("作者查找", [this]() {
            cin.ignore();
            string author = input("作者");
            clearScreen();
            this->bookDb->listBookForAuthor(author);
        });
        bookSearchMenu->addItem("借阅排行榜", [this]() {
            this->bookDb->listBooksByBorrow(10);
        });
        menu->addItem("书籍查找", [this, bookSearchMenu]() {
            cout << "开始书籍查找管理" << endl;
            this->menu=bookSearchMenu;
        });
    }

    void addUserBookMapMgrMenu() {
        Menu *borrowMenu = new Menu("书籍借阅管理", menu);
        borrowMenu->addItem("本人书籍借阅情况", [this]() {
            this->userBookMapDb->listBooksForUser(currentUser->getAccount());
        });
        borrowMenu->addItem("书籍借阅", [this]() {
            cin.ignore();
            string isbn = input("书籍ISBN/ISSN编号");
            clearScreen();
            this->bookDb->borrowBook(isbn);
            this->userBookMapDb->addUserBookMap(currentUser->getAccount(), isbn);
        });
        borrowMenu->addItem("书籍归还", [this]() {
            cin.ignore();
            string isbn = input("书籍ISBN/ISSN编号");
            clearScreen();
            this->userBookMapDb->deleteUserBookMap(currentUser->getAccount(), isbn);
            this->bookDb->returnBook(isbn);
        });
        menu->addItem("借阅管理", [this, borrowMenu]() {
            cout << "开始借阅管理" << endl;
            this->menu=borrowMenu;
        });
    }

    void run() {
        clearScreen();
        cout << "欢迎使用图书管理系统，请先登录" << endl;
        while (true) {
            string who = currentUser == nullptr ? "guest" : currentUser->getAccount();
            menu->showMenu(who);

            int choice;
            cin >> choice;
            Menu* newMenu = menu->handleChoice(choice);
            if (newMenu != nullptr) {
                menu = newMenu;
            }
        }
    }
};


int main() {
    UserDb *userDb = new UserDb("./user.csv");
    if (!userDb->load()) {
        cout << "初始化账户数据库失败，账户文件不存在" << endl;
        return 1;
    }
    BookDb *bookDb = new BookDb("./book.csv");
    if (!bookDb->load()) {
        cout << "初始化书籍数据库失败，书籍文件不存在" << endl;
        return 1;
    }
    UserBookMapDb *userBookMapDb = new UserBookMapDb("./book_user_map.csv");
    if (!userBookMapDb->load()) {
        cout << "初始化借阅信息数据库失败，借阅信息文件不存在" << endl;
        return 1;
    }


    BookSys *bookSys = new BookSys(userDb, bookDb, userBookMapDb);
    bookSys->run();

    return 0;
}

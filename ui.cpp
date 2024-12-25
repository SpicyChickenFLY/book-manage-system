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
    string name; // 账户
    string isbn; // 密码
    string author; // 角色: admin-管理员, user-用户
    public:
    // 获取私有成员
    string getName() const { return name; }
    string getISBN() const { return isbn; }
    string getAuthor() const { return author; }

    // 设置私有成员
    void setName(const string& n) { name = n; }
    void setISBN(const string& i) { isbn = i; }
    void setAuthor(const string& a) { author = a; }
};

User* currentUser = nullptr;
 
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
        cout << "查询用户失败, 用户(" << account << ")不存在" << endl;
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
            cout << "创建用户失败, 用户(" << account << ") 已经存在" << endl;
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
            cout << "创建用户失败" << endl;
        }
    }

    // 删除记录
    void deleteUser(const string& account) {
        if (checkUser(account) == nullptr) {
            cout << "删除用户失败, 用户(" << account << ")不存在" << endl;
            return;
        }
        users.erase(remove_if(users.begin(), users.end(), [account](const User& user) {
            return user.getAccount() == account;
        }), users.end());
        if (save()) {
            cout << "删除用户成功" << endl;
        } else {
            cout << "删除用户失败" << endl;
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
                    cout << "更新用户失败" << endl;
                }
                return;
            }
        }
        cerr << "更新失败, 用户(" << account << ")不存在: " << endl;
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

    void listBooks() {
        cout << "|            书籍名称 |   作者 |" << endl;
        cout << "|  ================== | ======= |" << endl;
        for (Book& book : books) {
            cout << "|" << setw(20) << book.getName() << " |" << setw(8) << book.getAuthor() << " |" << endl;
        }
        cout << "共 " << books.size() << " 条记录" << endl;
    }

    void findBook(const string& name) {
        for (Book& book : books) {
            if (book.getName() == name) {
                cout << "|             name |    ISBN  |  author |" << endl;
                cout << "|  ================== | =======  | ======= |" << endl;
                cout << "|" << setw(20) << book.getName() << " |" << setw(8) << book.getISBN() << " |" << setw(8) << book.getAuthor() << " |" << endl;
                return;
            }
        }
        cout << "查询书籍失败, 数据名称(" << name << ")不存在" << endl;
    }

    Book* checkBook(const string& name) {
        for (Book& book : books) {
            if (book.getName() == name) {
                return &book;
            }
        }
        return nullptr;
    }

    // 添加记录
    void addBook(const string& name, const string& isbn, const string& author) {
        if (checkBook(name) != nullptr) {
            cout << "create failed, book(" << name << ") alreday ex" << endl;
            return;
        }
        Book book;
        book.setName(name);
        book.setISBN(isbn);
        book.setAuthor(author);

        books.push_back(book);
        if (save()) {
            cout << "create succes" << endl;
        } else {
            cout << "create failed" << endl;
        }
    }

    // 删除记录
    void deleteBook(const string& name) {
        if (checkBook(name) == nullptr) {
            cout << "delete failed, book(" << name << ") not ex" << endl;
            return;
        }
        books.erase(remove_if(books.begin(), books.end(), [name](const Book& book) {
            return book.getName() == name;
        }), books.end());
        if (save()) {
            cout << "delete succes" << endl;
        } else {
            cout << "delete failed" << endl;
        }
    }

    // 更新记录
    void updateBook(const string& name, const string& isbn, const string& author) {
        if (author != "admin" && author != "book") {
            cout << "create failed, author must be admin or book" << endl;
            return;
        }
        for (Book& book : books) {
            if (book.getName() == name) {
                book.setISBN(isbn);
                book.setAuthor(author);
                if (save()) {
                    cout << "update succes" << endl;
                } else {
                    cout << "update failed" << endl;
                }
                return;
            }
        }
        cerr << "book" << name << " 未找到: " << endl;
    }
};

/*
 *  ============================================
 *                  多级菜单实现
 *  ============================================
 */

// 清屏函数
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

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
    vector<Menu*> subMenus;                         // 子菜单的集合

    public:
    Menu(const string &title, Menu* parent = nullptr)
        : title(title), parentMenu(parent) {}

    ~Menu() {
        for (size_t i = 0; i < subMenus.size(); ++i) {
            delete subMenus[i];
        }
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
    void showMenu(User *user) {
        cout << "\n-------------------------------" << endl;
        for (size_t i = 0; i < items.size(); ++i) {
            cout << i + 1 << ". " << items[i].name << endl;
        }
        if (parentMenu) {
            cout << "0. 返回上一级" << endl;
        } else {
            cout << "0. 退出系统" << endl;
        }
        string who = user == nullptr ? "guest" : user->getAccount();
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

    public:
    BookSys(UserDb *ud, BookDb *bd) {
        userDb = ud;
        bookDb = bd;

        menu = new Menu("主菜单");
        menu->addItem("关于", []() { cout << "这是一个用 C++ 实现的图书管理系统。" << endl; });
        menu->addItem("登录", [this]() {this->login();});
    }

    void login() {
        string account;
        cout << "请输入账户名称: ";
        cin.ignore();
        getline(cin, account);

        string password;
        cout << "请输入账户" << account << "的密码: ";
        getline(cin, password);
        clearScreen();

        User* user = userDb->checkUser(account);
        if (user == nullptr) {
            cout << "登录失败，user not ex" << endl;
            return;
        }
        string pwd = user->getPassword();
        if ( pwd != password) {
            cout << pwd <<  password << endl;
            cout << "登录失败，password error" << endl;
            return;
        }

        cout << "登录成功，您的角色为：" << user->getRole() << endl;
        currentUser = user;
        menu->removeItem("登录");
        if (user->getRole() == "admin") {
            loginAsAdmin();
        } else {
            loginAsUser();
        }
    }

    void loginAsAdmin() {
        Menu *userMenu = new Menu("账户管理", menu);
        userMenu->addItem("列出所有账户", [this]() {
            this->userDb->listUsers();
        });
        userMenu->addItem("查询账户", [this]() {
            string account;
            cout << "请输入账户名称: ";
            cin.ignore();
            getline(cin, account);
            clearScreen();

            this->userDb->findUser(account);
        });
        userMenu->addItem("新增账户", [this]() {
            string account;
            cout << "请输入账户名称: ";
            cin.ignore();
            getline(cin, account);

            string password;
            cout << "请输入账户" << account << "的密码: ";
            getline(cin, password);

            string role;
            cout << "请输入账户" << account << "的role: ";
            getline(cin, role);
            clearScreen();

            this->userDb->addUser(account, password, role);
        });
        userMenu->addItem("删除账户", [this]() {
            string account;
            cout << "请输入账户名称: ";
            cin.ignore();
            getline(cin, account);
            clearScreen();

            this->userDb->deleteUser(account);
        });
        userMenu->addItem("更新账户", [this]() {
            string account;
            cout << "请输入账户名称: ";
            cin.ignore();
            getline(cin, account);

            string password;
            cout << "请输入账户" << account << "的密码: ";
            getline(cin, password);

            string role;
            cout << "请输入账户" << account << "的role: ";
            getline(cin, role);
            clearScreen();

            this->userDb->updateUser(account, password, role);
        });
        menu->addItem("账户管理", [this, userMenu]() {this->menu=userMenu; });

        Menu *bookMenu = new Menu("书籍管理", menu);
        bookMenu->addItem("列出所有书籍", [this]() {
            this->bookDb->listBooks();
        });
        bookMenu->addItem("查询书籍", [this]() {
            string name;
            cout << "请输入账户名称: ";
            cin.ignore();
            getline(cin, name);
            clearScreen();

            this->bookDb->findBook(name);
        });
        bookMenu->addItem("新增书籍", [this]() {
            string name;
            cout << "请输入书籍名称: ";
            cin.ignore();
            getline(cin, name);

            string isbn;
            cout << "请输入书籍(" << name << ")的ISBN号: ";
            getline(cin, isbn);

            string author;
            cout << "请输入书籍(" << name << ")的作者名: ";
            getline(cin, author);
            clearScreen();

            this->bookDb->addBook(name, isbn, author);
        });
        bookMenu->addItem("删除书籍", [this]() {
            string name;
            cout << "请输入书籍名称: ";
            cin.ignore();
            getline(cin, name);
            clearScreen();

            this->bookDb->deleteBook(name);
        });
        bookMenu->addItem("更新书籍", [this]() {
            string name;
            cout << "请输入书籍名称: ";
            cin.ignore();
            getline(cin, name);

            string isbn;
            cout << "请输入书籍(" << name << ")的ISBN号: ";
            getline(cin, isbn);

            string author;
            cout << "请输入书籍(" << name << ")的作者名: ";
            getline(cin, author);
            clearScreen();

            this->bookDb->updateBook(name, isbn, author);
        });
        menu->addItem("书籍管理", [this, bookMenu]() {this->menu=bookMenu; });
    }

    void loginAsUser() {
        // Menu *userMenu = new Menu("find book by name", menu);
        // Menu *userMenu = new Menu("find book by isbn", menu);
        // Menu *userMenu = new Menu("list book by author", menu);
        // Menu *userMenu = new Menu("borrow book", menu);
        // Menu *userMenu = new Menu("return book", menu);
        // Menu *userMenu = new Menu("rank", menu);
    }

    void run() {
        clearScreen();
        while (true) {
            menu->showMenu(currentUser);

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

    BookSys *bookSys = new BookSys(userDb, bookDb);
    bookSys->run();

    return 0;
}

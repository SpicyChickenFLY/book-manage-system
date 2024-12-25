#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <algorithm> // for remove_if

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

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
    std::string name;                      // 菜单项名称
    std::function<void()> action;          // 菜单项对应的操作

    MenuItem(const std::string &name, std::function<void()> action)
        : name(name), action(action) {}
};

// 菜单类
class Menu {
private:
    std::string title;                                    // 菜单标题
    std::vector<MenuItem> items;                         // 当前菜单的选项
    Menu* parentMenu;                                    // 指向父菜单的指针
    std::vector<Menu*> subMenus;                         // 子菜单的集合

public:
    Menu(const std::string &title, Menu* parent = nullptr)
        : title(title), parentMenu(parent) {}

    ~Menu() {
        for (size_t i = 0; i < subMenus.size(); ++i) {
            delete subMenus[i];
        }
    }

    // 添加菜单项
    void addItem(const std::string &name, std::function<void()> action) {
        items.emplace_back(name, action);
    }

    // 删除菜单项
    void removeItem(const std::string &name) {
        items.erase(std::remove_if(items.begin(), items.end(),
                    [&name](const MenuItem &item) { return item.name == name; }),
                    items.end());
    }

    // 添加子菜单
    Menu* addSubMenu(const std::string &name) {
        Menu* subMenu = new Menu(name, this);
        subMenus.push_back(subMenu);
        items.emplace_back(name, [subMenu]() { subMenu->showMenu(); });
        return subMenu;
    }

    // 删除子菜单
    void removeSubMenu(const std::string &name) {
        auto it = std::remove_if(subMenus.begin(), subMenus.end(),
                    [&name](Menu* subMenu) { return subMenu->title == name; });
        if (it != subMenus.end()) {
            delete *it;
            subMenus.erase(it);
        }
        removeItem(name);
    }

    // 显示菜单
    void showMenu() {
        items[0].action();
        while (true) {
            std::cout << "\n-------------------------------" << std::endl;
            for (size_t i = 0; i < items.size(); ++i) {
                std::cout << i + 1 << ". " << items[i].name << std::endl;
            }
            if (parentMenu) {
                std::cout << "0. 返回上一级" << std::endl;
            } else {
                std::cout << "0. 退出系统" << std::endl;
            }

            int choice;
            std::cout << "\n" << title << ">: ";
            std::cin >> choice;

            clearScreen();

            if (choice == 0) {
                if (parentMenu) {
                    return; // 返回父菜单
                } else {
                    std::cout << "正在退出系统，再见！" << std::endl;
                    return;
                }
            } else if (choice > 0 && choice <= items.size()) {
                items[choice - 1].action();
            } else {
                std::cout << "无效的选择，请重试。" << std::endl;
            }

            std::cout << "\n" << std::endl;
        }
    }
};


// 用户类
class User {
private:
    string account; // 账户
    string password; // 密码
    string role; // 角色: admin-管理员, user-用户

public:
    // 构造函数
    User(const string &acc, const string &pwd, const string &rl)
        : account(acc), password(pwd), role(rl) {}

    // 获取私有成员
    string getAccount() const { return account; }
    string getRole() const { return role; }

    // 验证密码
    bool validatePassword(const string &pwd) const { return password == pwd; }
    // 修改密码
    void changePassword(const string &newPwd) { password = newPwd; }

    // 打印用户信息
    void printUserInfo() const {
        cout << "Account: " << account << "\nRole: " << role << endl;
    }
};

int main() {
    // 创建主菜单
    Menu* mainMenu = new Menu("主菜单");

    // 添加主菜单项
    mainMenu->addItem("打招呼", []() { std::cout << "你好，欢迎使用系统！" << std::endl; });
    mainMenu->addItem("关于", []() { std::cout << "这是一个用 C++ 实现的交互式菜单系统。" << std::endl; });

    // 创建子菜单
    Menu* subMenu = mainMenu->addSubMenu("进入子菜单");
    subMenu->addItem("子菜单选项 1", []() { std::cout << "你选择了子菜单选项 1。" << std::endl; });
    subMenu->addItem("子菜单选项 2", []() { std::cout << "你选择了子菜单选项 2。" << std::endl; });

    // 动态添加功能示例
    mainMenu->addItem("动态添加选项", [mainMenu]() {
        std::string newOption;
        std::cout << "请输入新选项的名称: ";
        std::cin.ignore();
        std::getline(std::cin, newOption);
        mainMenu->addItem(newOption, [newOption]() { std::cout << "你选择了: " << newOption << std::endl; });
        std::cout << "新选项已添加！" << std::endl;
    });

    // 动态删除功能示例
    mainMenu->addItem("动态删除选项", [mainMenu]() {
        std::string removeOption;
        std::cout << "请输入要删除的选项名称: ";
        std::cin.ignore();
        std::getline(std::cin, removeOption);
        mainMenu->removeItem(removeOption);
        std::cout << "选项已删除（如果存在）。" << std::endl;
    });

    // 显示主菜单
    mainMenu->showMenu();

    // 清理内存
    delete mainMenu;

    return 0;
}

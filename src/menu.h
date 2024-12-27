/*
 *  ============================================
 *                  多级菜单实现
 *  ============================================
 */

#ifndef MENU_H
#define MENU_H

#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

// 菜单项结构
struct MenuItem {
  std::string name; // 菜单项名称
  int action;       // 菜单项对应的操作

  MenuItem(const std::string &name, int action) : name(name), action(action) {}
};

// 菜单类
class Menu {
private:
  std::string title;           // 菜单标题
  std::vector<MenuItem> items; // 当前菜单的选项
  Menu *parentMenu;            // 指向父菜单的指针

public:
  Menu(const std::string &title, Menu *parent = nullptr)
      : title(title), parentMenu(parent) {}
  ~Menu() {}

  // 添加菜单项
  void addItem(const std::string &name, int action) {
    items.emplace_back(name, action);
  }

  // 删除菜单项
  void removeItem(const std::string &name) {
    items.erase(
        remove_if(items.begin(), items.end(),
                  [&name](const MenuItem &item) { return item.name == name; }),
        items.end());
  }

  // 显示菜单
  void showMenu(std::string who) {
    std::cout << "\n-------------------------------" << std::endl;
    for (size_t i = 0; i < items.size(); ++i) {
      std::cout << i + 1 << ". " << items[i].name << std::endl;
    }
    if (parentMenu) {
      std::cout << "0. 返回上一级" << std::endl;
    } else {
      std::cout << "0. 退出系统" << std::endl;
    }
    std::cout << "\n" << "[" << who << "]" << title << ">: ";
  }

  int handleChoice(int choice) {
    if (choice == 0) {
      return 0;
    } else if (choice > 0 && choice <= items.size()) {
      std::cout << items[choice - 1].name << std::endl;
      return items[choice - 1].action;
    } else {
      std::cout << "无效的选择，请重试。" << std::endl;
      return -1;
    }
  }
};

#endif // !MENU_H

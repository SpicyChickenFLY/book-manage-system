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
  std::string gobackDesc;      //

public:
  Menu(const std::string &title, Menu *parent, std::string gobackDesc)
      : title(title), parentMenu(parent), gobackDesc(gobackDesc) {}
  ~Menu() {}

  std::string getGobackDesc() { return gobackDesc; }

  void addItem(const std::string &name, int action); // 添加菜单项
  void removeItem(const std::string &name);          // 删除菜单项
  void showMenu(std::string who);                    // 显示菜单
  int handleChoice(int choice);                      //
};

#endif // !MENU_H

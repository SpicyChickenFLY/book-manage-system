/*
 *  ============================================
 *                  多级菜单实现
 *  ============================================
 */

#include "menu.h"

// 添加菜单项
void Menu::addItem(const std::string &name, int action) {
  items.emplace_back(name, action);
}

// 删除菜单项
void Menu::removeItem(const std::string &name) {
  items.erase(
      remove_if(items.begin(), items.end(),
                [&name](const MenuItem &item) { return item.name == name; }),
      items.end());
}

// 显示菜单
void Menu::showMenu(std::string who) {
  std::cout << "\n-------------------------------" << std::endl;
  for (size_t i = 0; i < items.size(); ++i) {
    std::cout << i + 1 << ". " << items[i].name << std::endl;
  }
  if (parentMenu) {
    std::cout << "0. 返回" << parentMenu->getTitle() << std::endl;
  } else {
    std::cout << "0. 退出系统" << std::endl;
  }
  std::cout << "[" << who << "]" << title << ">: ";
}

int Menu::handleChoice(int choice) {
  std::cout << std::endl;
  if (choice == 0) {
    if (parentMenu == nullptr) {
      std::cout << "退出系统，再见" << std::endl;
      exit(0);
    }
    std::cout << "返回" << parentMenu->getTitle() << std::endl;
    return 0;
  } else if (choice > 0 && choice <= items.size()) {
    std::cout << items[choice - 1].name << std::endl;
    return items[choice - 1].action;
  } else {
    std::cout << "无效的选择，请重试。" << std::endl;
    return -1;
  }
}

/*
 *  ============================================
 *                  系统运行逻辑
 *  ============================================
 */

#ifndef SYS_H
#define SYS_H

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <cstdlib>
#include <unistd.h>
#endif

#include "entity.h"
#include "menu.h"
#include "service.h"

#define ADMIN_LOGIN 1
#define READER_LOGIN 1
#define LOGOUT 1

#define LIST_USER 2
#define FIND_USER 2
#define ADD_USER 2
#define DEL_USER 2
#define UPD_USER 2

#define LIST_BOOK 2
#define FIND_BOOK 2
#define ADD_BOOK 2
#define DEL_BOOK 2
#define UPD_BOOK 2

#define FIND_BOOK_NAME 3
#define FIND_BOOK_ISBN 3
#define FIND_BOOK_AUTHOR 3
#define LIST_BOOK_BORROW 3

#define LIST_USER_BOOK 4
#define ADD_USER_BOOK 4
#define DEL_USER_BOOK 4
#define UPD_USER_BOOK 4

// 图书管理系统类
class BookSys {
private:
  Menu *currentMenu; // 指示当前所处的菜单
  User *currentUser; // 指示当前登录的用户

  UserService *userService;               // 用户数据库
  BookService *bookService;               // 书籍数据库
  UserBookMapService *userBookMapService; // 借阅数据库

public:
  BookSys(UserService *us, BookService *bs, UserBookMapService *ubms);

  // 显示需要输入的内容，并获取用户输入，支持明文/密码两种方式
  std::string input(std::string desc, bool echo = true);

  bool init(); // 加载所有文件数据

  Menu *createLoginMenu();                          // 登录菜单
  Menu *createMainMenuForReader();                  // 读者主菜单
  Menu *createMainMenuForAdmin();                   // 管理员主菜单
  Menu *createUserMgrMenu(Menu *parentMenu);        // 动态增加账户管理菜单
  Menu *createBookMgrMenu(Menu *parentMenu);        // 动态增加书籍管理菜单
  Menu *createBookSearchMenu(Menu *parentMenu);     // 动态增加数据搜索菜单
  Menu *createUserBookMapMgrMenu(Menu *parentMenu); // 动态增加借阅管理菜单

  void login(); // 处理用户登录逻辑

  void execAction(int action);

  // 系统的循环运行逻辑
  void run();
};

#endif // !SYS_H

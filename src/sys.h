/*
 *  ============================================
 *                  系统运行逻辑
 *  ============================================
 */

#ifndef SYS_H
#define SYS_H

#include <vector>
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

#define GOBACK 0

#define LOGOUT 1
#define ADMIN_LOGIN 2
#define READER_LOGIN 3

#define MGR_USER 4
#define MGR_BOOK 5
#define FIND_BOOK 6
#define MGR_USER_BOOK 7

#define LIST_USER 8
#define FIND_USER 9
#define ADD_USER 10
#define DEL_USER 11
#define UPD_USER 12

#define LIST_BOOK 13
#define ADD_BOOK 14
#define DEL_BOOK 15
#define UPD_BOOK 16

#define FIND_BOOK_NAME 17
#define FIND_BOOK_ISBN 18
#define FIND_BOOK_AUTHOR 19
#define LIST_BOOK_BORROW 20

#define LIST_USER_BOOK 21
#define ADD_USER_BOOK 22
#define DEL_USER_BOOK 23

// 图书管理系统类
class System {
private:
  Menu *currentMenu; // 指示当前所处的菜单
  User *currentUser; // 指示当前登录的用户

  UserService *userService;               // 用户服务
  BookService *bookService;               // 书籍服务
  BorrowService *borrowService; // 借阅服务

public:
  System(UserService *us, BookService *bs, BorrowService *brs);

  bool init(); // 加载所有文件数据

  std::string getInput(std::string desc,
                       bool echo = true); // 获取用户输入，支持明文/密码两种方式
  std::string inputOneStr(std::string desc,
                          bool echo = true);        // 获取单个单字段输入
  User *inputUserAuthInfo();                        // 用户输入账密信息
  User *inputUser();                                // 用户输入完整账户信息
  Book *inputBook();                                // 用户输入完整书籍信息
  void outputUser(User *user);                      // 输出账户信息
  void outputUsers(std::vector<User> user);         // 输出账户列表信息
  void outputBook(Book *book);                      // 输出书籍信息
  void outputBooks(std::vector<Book> books);        // 输出书籍列表信息
  void outputUserBookMap(UserBookMap *userBookMap); // 输出书籍信息
  void
  outputUserBookMaps(std::vector<UserBookMap> userBookMaps); // 输出书籍列表信息

  Menu *createLoginMenu();                          // 登录菜单
  Menu *createMainMenuForReader();                  // 读者主菜单
  Menu *createMainMenuForAdmin();                   // 管理员主菜单
  Menu *createUserMgrMenu(Menu *parentMenu);        // 账户管理菜单
  Menu *createBookMgrMenu(Menu *parentMenu);        // 书籍管理菜单
  Menu *createBookSearchMenu(Menu *parentMenu);     // 数据搜索菜单
  Menu *createUserBookMapMgrMenu(Menu *parentMenu); // 借阅管理菜单

  Menu *execAction(int action); // 各个菜单项命令实际执行逻辑
  void run();                   // 系统的循环运行逻辑
};

#endif // !SYS_H

#include "sys.h"
#include <iostream>

// 显示需要输入的内容，并获取用户输入，支持明文/密码两种方式
std::string System::getInput(std::string desc, bool echo) {
  std::cout << "请输入" << desc << ": ";
  if (echo) {
    std::string temp;
    getline(std::cin, temp);
    return temp;
  } else {
#ifdef _WIN32
    std::string temp;
    while (true) {
      char ch = getch();
      if (ch == '\r' || ch == '\n')
        break;
      temp += ch;
    }
    return temp;
#else
    return getpass("");
#endif
  }
}

System::System(UserService *us, BookService *bs, UserBookMapService *ubms)
    : userService(us), bookService(bs), userBookMapService(ubms) {
  currentUser = new User();
  currentUser->setAccount("游客");
  currentMenu = createLoginMenu();
}

bool System::init() {
  // 初始化数据服务
  if (!userService->load())
    return false;
  if (!bookService->load())
    return false;
  if (!userBookMapService->load())
    return false;

  // 初始化系统菜单

  return true;
}

std::string System::inputOneStr(std::string desc, bool echo) {
  std::cin.ignore();
  std::string str = getInput(desc, echo);
  std::cout << std::endl;
  return str;
}

User *System::inputUserAuthInfo() {
  std::cin.ignore();
  std::string account = getInput("账户名称");
  std::string password = getInput("账户" + account + "的密码", false);
  std::cout << std::endl;
  return new User(account, password, "");
}

User *System::inputUser() {
  std::cin.ignore();
  std::string account = getInput("账户名称");
  std::string password = getInput("账户" + account + "的密码", false);
  std::string role = getInput("账户" + account + "的role");
  std::cout << std::endl;
  return new User(account, password, role);
}

Book *System::inputBook() {
  std::cin.ignore();
  std::string isbn = getInput("书籍ISBN号");
  std::string name = getInput("书籍(" + isbn + ")的名称");
  std::string author = getInput("书籍(" + isbn + ")的作者");
  std::string publish = getInput("书籍(" + isbn + ")的出版社");
  std::string price = getInput("书籍(" + isbn + ")的价格");
  std::string capacity = getInput("书籍(" + isbn + ")的库存");
  std::cout << std::endl;
  return new Book(name, isbn, author, publish, price, stoi(capacity));
}

void System::outputUser(User *user) {
  if (user)
    std::cout << "账户名称:" << user->getAccount() << std::endl
              << "角色:" << user->getRole() << std::endl
              << std::endl;
  else
    std::cout << "结果不存在" << std::endl;
}

void System::outputUsers(std::vector<User> users) {
  for (User &user : users) {
    outputUser(&user);
  }
  std::cout << "共 " << users.size() << " 条记录" << std::endl;
}

void System::outputBook(Book *book) {
  if (book)
    std::cout << "书籍名称:" << book->getName() << std::endl
              << "ISBN:" << book->getISBN() << std::endl
              << "作者:" << book->getAuthor() << std::endl
              << "出版社:" << book->getPublish() << std::endl
              << "价格:" << book->getPrice() << std::endl
              << "库存:" << book->getCapacity() << std::endl
              << "借阅次数:" << book->getBorrow() << std::endl
              << std::endl;
  else
    std::cout << "结果不存在" << std::endl;
}

void System::outputBooks(std::vector<Book> books) {
  for (Book &book : books) {
    outputBook(&book);
  }
  std::cout << "共 " << books.size() << " 条记录" << std::endl;
}

void System::outputUserBookMap(UserBookMap *userBookMap) {
  if (userBookMap)
    std::cout << "账户名称:" << userBookMap->getAccount() << std::endl
              << "ISBN:" << userBookMap->getISBN() << std::endl
              << std::endl;
  else
    std::cout << "结果不存在" << std::endl;
}

void System::outputUserBookMaps(std::vector<UserBookMap> userBookMaps) {
  for (UserBookMap &userBookMap : userBookMaps) {
    outputUserBookMap(&userBookMap);
  }
  std::cout << "共 " << userBookMaps.size() << " 条记录" << std::endl;
}

// 动态增加主菜单
Menu *System::createLoginMenu() {
  Menu *loginMenu = new Menu("登录菜单", nullptr);
  loginMenu->addItem("管理员登录", ADMIN_LOGIN);
  loginMenu->addItem("读者登录", READER_LOGIN);
  return loginMenu;
}

Menu *System::createMainMenuForAdmin() {
  Menu *adminMenu = new Menu("管理员菜单", nullptr);
  adminMenu->addItem("账户管理", MGR_USER);
  adminMenu->addItem("书籍管理", MGR_BOOK);
  adminMenu->addItem("书籍查询", FIND_BOOK);
  adminMenu->addItem("用户登出", LOGOUT);
  return adminMenu;
}

Menu *System::createMainMenuForReader() {
  Menu *readerMenu = new Menu("读者菜单", nullptr);
  readerMenu->addItem("书籍查询", FIND_BOOK);
  readerMenu->addItem("书籍借阅归还", MGR_USER_BOOK);
  readerMenu->addItem("用户登出", LOGOUT);
  return readerMenu;
}

// 动态增加账户管理菜单
Menu *System::createUserMgrMenu(Menu *parentMenu) {
  Menu *userMenu = new Menu("账户管理", parentMenu);
  // userMenu->addItem("列出所有账户", LIST_USER);
  userMenu->addItem("查询账户", FIND_USER);
  userMenu->addItem("新增账户", ADD_USER);
  userMenu->addItem("删除账户", DEL_USER);
  userMenu->addItem("更新账户", UPD_USER);
  return userMenu;
}

// 动态增加书籍管理菜单
Menu *System::createBookMgrMenu(Menu *parentMenu) {
  Menu *bookMgrMenu = new Menu("书籍管理", parentMenu);
  bookMgrMenu->addItem("新增书籍", ADD_BOOK);
  bookMgrMenu->addItem("删除书籍", DEL_BOOK);
  bookMgrMenu->addItem("更新书籍", UPD_BOOK);
  return bookMgrMenu;
}

// 动态增加数据搜索菜单
Menu *System::createBookSearchMenu(Menu *parentMenu) {
  Menu *bookSearchMenu = new Menu("书籍查找", parentMenu);
  // bookSearchMenu->addItem("列出所有书籍", LIST_BOOK);
  bookSearchMenu->addItem("书名查找", FIND_BOOK_NAME);
  bookSearchMenu->addItem("ISBN/ISSN编号查找", FIND_BOOK_ISBN);
  bookSearchMenu->addItem("作者查找", FIND_BOOK_AUTHOR);
  bookSearchMenu->addItem("借阅排行榜", LIST_BOOK_BORROW);
  return bookSearchMenu;
}

// 动态增加借阅管理菜单
Menu *System::createUserBookMapMgrMenu(Menu *parentMenu) {
  Menu *borrowMenu = new Menu("书籍借阅管理", parentMenu);
  borrowMenu->addItem("本人书籍借阅情况", LIST_USER_BOOK);
  borrowMenu->addItem("书籍借阅", ADD_USER_BOOK);
  borrowMenu->addItem("书籍归还", DEL_USER_BOOK);
  return borrowMenu;
}

Menu *System::execAction(int action) {

  // 登录菜单
  if (action == GOBACK) {
    return currentMenu->getParentMenu();
  } else if (action == ADMIN_LOGIN) {
    User *user = userService->checkUserLogin(inputUserAuthInfo());
    if (!user) {
      std::cout << "登录失败, 账号密码错误" << std::endl;
      return nullptr;
    }
    if (user->getRole() != "admin") {
      std::cout << "登录失败, 用户权限不足" << std::endl;
      return nullptr;
    }
    currentUser = user;
    std::cout << "登录成功" << std::endl;
    return createMainMenuForAdmin();
  } else if (action == READER_LOGIN) {
    User *user = userService->checkUserLogin(inputUserAuthInfo());
    if (!user) {
      std::cout << "登录失败, 账号密码错误" << std::endl;
      return nullptr;
    }
    currentUser = user;
    std::cout << "登录成功" << std::endl;
    return createMainMenuForReader();

    // 主菜单
  } else if (action == MGR_USER) {
    return createUserMgrMenu(currentMenu);
  } else if (action == MGR_BOOK) {
    return createBookMgrMenu(currentMenu);
  } else if (action == MGR_USER_BOOK) {
    return createUserBookMapMgrMenu(currentMenu);
  } else if (action == LOGOUT) {
    currentUser = new User();
    currentUser->setAccount("游客");
    return createLoginMenu();
  }

  // 账户管理菜单
  else if (action == LIST_USER) {
    outputUsers(userService->listUsers());
  } else if (action == FIND_USER) {
    outputUser(userService->findUser(inputOneStr("账户名称")));
  } else if (action == ADD_USER) {
    userService->addUser(inputUser());
  } else if (action == DEL_USER) {
    userService->deleteUser(inputOneStr("账户名称"));
  } else if (action == UPD_USER) {
    userService->updateUser(inputUser());
  }

  // 书籍管理
  else if (action == LIST_BOOK) {
    outputBooks(bookService->listBooks());
  } else if (action == FIND_BOOK) {
    return createBookSearchMenu(currentMenu);
  } else if (action == ADD_BOOK) {
    bookService->addBook(inputBook());
  } else if (action == DEL_BOOK) {
    bookService->deleteBook(inputOneStr("书籍ISBN/ISSN编号"));
  } else if (action == UPD_BOOK) {
    bookService->updateBook(inputBook());
  }

  // 书籍查询
  else if (action == FIND_BOOK_NAME) {
    outputBooks(bookService->listBooksForName(inputOneStr("书籍名称")));
  } else if (action == FIND_BOOK_AUTHOR) {
    outputBooks(bookService->listBookForAuthor(inputOneStr("作者")));
  } else if (action == LIST_BOOK_BORROW) {
    outputBooks(bookService->listBooksByBorrow(10));
  } else if (action == FIND_BOOK_ISBN) {
    outputBook(bookService->findBookForISBN(inputOneStr("书籍ISBN/ISSN编号")));
  }

  // 借阅管理
  else if (action == LIST_USER_BOOK) {
    outputUserBookMaps(
        userBookMapService->listBooksForUser(currentUser->getAccount()));
  } else if (action == ADD_USER_BOOK) {
    std::string isbn = inputOneStr("书籍ISBN/ISSN编号");
    bookService->borrowBook(isbn);
    userBookMapService->addUserBookMap(currentUser->getAccount(), isbn);
  } else if (action == DEL_USER_BOOK) {
    std::string isbn = inputOneStr("书籍ISBN/ISSN编号");
    userBookMapService->deleteUserBookMap(currentUser->getAccount(), isbn);
    bookService->returnBook(isbn);
  }

  else {
    std::cout << "无效命令！" << std::endl;
  }

  return nullptr;
}

// 系统的循环运行逻辑
void System::run() {
  for (int i = 0; i < 50; i++) {
    std::cout << std::endl;
  }
  std::cout << "欢迎使用图书管理系统，请先登录" << std::endl;
  while (true) {
    currentMenu->showMenu(currentUser->getAccount());

    int choice;
    std::cin >> choice;

    int action = currentMenu->handleChoice(choice); // 指定命令
    Menu *menu = execAction(action);                // 执行命令
    if (menu != nullptr)
      currentMenu = menu; // 更新当前目录
  }
}

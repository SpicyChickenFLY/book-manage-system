#include "sys.h"

// 清屏函数
void BookSys::clearScreen() {
#ifdef _WIN32
  system("cls"); // windows bat 清屏命令
#else
  system("clear"); // 类unix shell 清屏命令
#endif
}

// 显示需要输入的内容，并获取用户输入，支持明文/密码两种方式
std::string BookSys::getInput(std::string desc, bool echo) {
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

BookSys::BookSys(UserService *us, BookService *bs, UserBookMapService *ubms)
    : userService(us), bookService(bs), userBookMapService(ubms) {
  User *currentUser = new User();
  currentUser->setAccount("guest");
  currentMenu = createLoginMenu();
}

bool BookSys::init() {
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

std::string BookSys::inputOneStr(std::string desc, bool echo) {
  std::cin.ignore();
  return getInput(desc, echo);
}

User *BookSys::inputUserAccountPassword() {
  std::cin.ignore();
  std::string account = input("账户名称");
  std::string password = input("账户" + account + "的密码", false);
  return new User(account, password, "");
}

User *BookSys::inputUser() {
  std::cin.ignore();
  std::string account = input("账户名称");
  std::string password = input("账户" + account + "的密码", false);
  std::string role = input("账户" + account + "的role");
  return new User(account, password, role);
}

Book *BookSys::inputBook() {
  std::cin.ignore();
  std::string isbn = input("书籍ISBN号");
  std::string name = input("书籍(" + isbn + ")的名称");
  std::string author = input("书籍(" + isbn + ")的作者");
  std::string publish = input("书籍(" + isbn + ")的出版社");
  std::string price = input("书籍(" + isbn + ")的价格");
  std::string capacity = input("书籍(" + isbn + ")的库存");
  return new Book(name, isbn, author, publish, price, stoi(capacity));
}

// 动态增加主菜单
Menu *BookSys::createLoginMenu() {
  Menu *loginMenu = new Menu("登录菜单", nullptr, "");
  loginMenu->addItem("管理员登录", ADMIN_LOGIN);
  loginMenu->addItem("读者登录", READER_LOGIN);
  return loginMenu;
}

Menu *BookSys::createMainMenuForReader() { return nullptr; }

Menu *BookSys::createMainMenuForAdmin() { return nullptr; }

// 动态增加账户管理菜单
Menu *BookSys::createUserMgrMenu(Menu *parentMenu) {
  Menu *userMenu = new Menu("账户管理", parentMenu, "");
  userMenu->addItem("列出所有账户", LIST_USER);
  userMenu->addItem("查询账户", FIND_USER);
  userMenu->addItem("新增账户", ADD_USER);
  userMenu->addItem("删除账户", DEL_USER);
  userMenu->addItem("更新账户", UPD_USER);
  return userMenu;
}

// 动态增加书籍管理菜单
Menu *BookSys::createBookMgrMenu(Menu *parentMenu) {
  Menu *bookMgrMenu = new Menu("书籍管理", parentMenu, "");
  bookMgrMenu->addItem("列出所有书籍", LIST_BOOK);
  bookMgrMenu->addItem("新增书籍", ADD_BOOK);
  bookMgrMenu->addItem("删除书籍", DEL_BOOK);
  bookMgrMenu->addItem("更新书籍", UPD_BOOK);
  return bookMgrMenu;
}

// 动态增加数据搜索菜单
Menu *BookSys::createBookSearchMenu(Menu *parentMenu) {
  Menu *bookSearchMenu = new Menu("书籍查找", parentMenu, "");
  bookSearchMenu->addItem("书名查找", FIND_BOOK_NAME);
  bookSearchMenu->addItem("ISBN/ISSN编号查找", FIND_BOOK_ISBN);
  bookSearchMenu->addItem("作者查找", FIND_BOOK_AUTHOR);
  bookSearchMenu->addItem("借阅排行榜", LIST_BOOK_BORROW);
  return bookSearchMenu;
}

// 动态增加借阅管理菜单
Menu *BookSys::createUserBookMapMgrMenu(Menu *parentMenu) {
  Menu *borrowMenu = new Menu("书籍借阅管理", parentMenu, "");
  borrowMenu->addItem("本人书籍借阅情况", LIST_USER_BOOK);
  borrowMenu->addItem("书籍借阅", ADD_USER_BOOK);
  borrowMenu->addItem("书籍归还", DEL_USER_BOOK);
  return borrowMenu;
}

void BookSys::execAction(int action) {
  if (action == GOBACK) {
  } else if (action == ADMIN_LOGIN) {
    User *entity = inputUserAccountPassword();
    User *user = userService->checkUserLogin(entity);
    if (user) {
      std::cout << "登录成功" << std::endl;
      currentUser = user;
    } else {
      std::cout << "登录失败, 账号密码错误" << std::endl;
      // TODO: 目录回退
    }
  } else if (action == READER_LOGIN) {
    User *entity = inputUserAccountPassword();
    User *user = userService->checkUserLogin(entity);
    if (user) {
      std::cout << "登录成功" << std::endl;
      currentUser = user;
    } else {
      std::cout << "登录失败, 账号密码错误" << std::endl;
      // TODO: 目录回退
    }

  } else if (action == LIST_USER) {
    this->userService->listUsers();
  } else if (action == FIND_USER) {
    this->userService->findUser(inputOneStr("账户名称"));
  } else if (action == ADD_USER) {
    this->userService->addUser(inputUser());
  } else if (action == DEL_USER) {
    this->userService->deleteUser(inputOneStr("账户名称"));
  } else if (action == UPD_USER) {
    this->userService->updateUser(inputUser());
  }
  // 
  else if (action == LIST_BOOK) {
    this->bookService->listBooks();
  } else if (action == ADD_BOOK) {
    this->bookService->addBook(inputBook());
  } else if (action == DEL_BOOK) {
    std::cin.ignore();
    std::string isbn = input("请输入书籍ISBN/ISSN编号");
    this->bookService->deleteBook(isbn);
  } else if (action == UPD_BOOK) {
    this->bookService->updateBook(inputBook());

  } else if (action == FIND_BOOK_NAME) {
    this->bookService->listBooksForName(inputOneStr("书籍名称"));
  } else if (action == FIND_BOOK_ISBN) {
    this->bookService->findBookForISBN(inputOneStr("书籍ISBN号"));
  } else if (action == FIND_BOOK_AUTHOR) {
    this->bookService->listBookForAuthor(inputOneStr("作者"));
  } else if (action == LIST_BOOK_BORROW) {
    this->bookService->listBooksByBorrow(10);
  }
  // 
  else if (action == ADD_USER_BOOK) {
    this->userBookMapService->listBooksForUser(currentUser->getAccount());
  } else if (action == DEL_USER_BOOK) {
    std::cin.ignore();
    std::string isbn = input("书籍ISBN/ISSN编号");
    this->bookService->borrowBook(isbn);
    this->userBookMapService->addUserBookMap(currentUser->getAccount(), isbn);
  } else if (action == UPD_USER_BOOK) {
    std::cin.ignore();
    std::string isbn = input("书籍ISBN/ISSN编号");
    this->userBookMapService->deleteUserBookMap(currentUser->getAccount(), isbn);
    this->bookService->returnBook(isbn);
  }
}

// 系统的循环运行逻辑
void BookSys::run() {
  clearScreen();
  std::cout << "欢迎使用图书管理系统，请先登录" << std::endl;
  while (true) {
    std::string who =
        currentUser == nullptr ? "guest" : currentUser->getAccount();
    currentMenu->showMenu(who);

    int choice;
    std::cin >> choice;
    int action = currentMenu->handleChoice(choice);

    execAction(action); // 执行命令

    // TODO:
    // currentMenu = newMenu; // 跳转目录
  }
}

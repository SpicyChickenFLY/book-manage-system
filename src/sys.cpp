#include "sys.h"

// 清屏函数
#ifdef _WIN32
void clearScreen() {
  system("cls"); // windows bat 清屏命令
}
std::string inputPwd() {
  std::string temp;
  while (true) {
    char ch = getch();
    if (ch == '\r' || ch == '\n')
      break;
    temp += ch;
  }
  return temp;
}
#else
void clearScreen() {
  // system("clear"); // 类unix shell 清屏命令
  std::cout << std::endl;
}

std::string inputPwd() { return getpass(""); }
#endif

std::string inputStr() {
  std::string temp;
  getline(std::cin, temp);
  return temp;
}

// 显示需要输入的内容，并获取用户输入，支持明文/密码两种方式
std::string input(std::string desc, bool echo = true) {
  std::cout << "请输入" << desc << ": ";
  return echo ? inputStr() : inputPwd();
}

BookSys::BookSys(UserService *us, BookService *bs, UserBookMapService *ubms)
    : userService(us), bookService(bs), userBookMapService(ubms) {
  User *currentUser = new User();
  currentUser->setAccount("guest");
  currentMenu = createLoginMenu();
}

bool BookSys::init() {
  return (userService->load() && bookService->load() &&
          userBookMapService->load());
}

// 处理用户登录逻辑
void BookSys::login() {
  std::string account = input("账户名称");
  std::string password = input("账户" + account + "的密码", false);
  clearScreen();

  User *user = userService->checkUser(account);
  if (user == nullptr || user->getPassword() != password) {
    std::cout << "登录失败, 账号密码错误" << std::endl;
    return;
  }

  std::cout << "登录成功，您的角色为：" << user->getRole() << std::endl;
  currentUser = user;

  // 切换为对应角色的主菜单
  currentMenu->removeItem("登录");
  if (user->getRole() == "admin") {
    addUserMgrMenu(currentMenu);
    addBookMgrMenu(currentMenu);
    addBookSearchMenu(currentMenu);
  } else {
    addBookSearchMenu(currentMenu);
    addUserBookMapMgrMenu(currentMenu);
  }
}

// 动态增加主菜单
Menu *BookSys::createLoginMenu() {
  Menu *loginMenu = new Menu("账户管理", nullptr);
  loginMenu = new Menu("登录菜单");
  loginMenu->addItem("管理员登录", ADMIN_LOGIN);
  loginMenu->addItem("读者登录", READER_LOGIN);
  return loginMenu;
}

Menu *BookSys::createMainMenuForReader() { return nullptr; }

Menu *BookSys::createMainMenuForAdmin() { return nullptr; }

// 动态增加账户管理菜单
Menu *BookSys::addUserMgrMenu(Menu *parentMenu) {
  Menu *userMenu = new Menu("账户管理", parentMenu);
  userMenu->addItem("列出所有账户", LIST_USER);
  userMenu->addItem("查询账户", FIND_USER);
  userMenu->addItem("新增账户", ADD_USER);
  userMenu->addItem("删除账户", DEL_USER);
  userMenu->addItem("更新账户", UPD_USER);
  return userMenu;
}

// 动态增加书籍管理菜单
Menu *BookSys::addBookMgrMenu(Menu *parentMenu) {
  Menu *bookMgrMenu = new Menu("书籍管理", parentMenu);
  bookMgrMenu->addItem("列出所有书籍", LIST_BOOK);
  bookMgrMenu->addItem("新增书籍", ADD_BOOK);
  bookMgrMenu->addItem("删除书籍", DEL_BOOK);
  bookMgrMenu->addItem("更新书籍", UPD_BOOK);
  return bookMgrMenu;
}

// 动态增加数据搜索菜单
Menu *BookSys::addBookSearchMenu(Menu *parentMenu) {
  Menu *bookSearchMenu = new Menu("书籍查找", parentMenu);
  bookSearchMenu->addItem("书名查找", FIND_BOOK_NAME);
  bookSearchMenu->addItem("ISBN/ISSN编号查找", FIND_BOOK_ISBN);
  bookSearchMenu->addItem("作者查找", FIND_BOOK_AUTHOR);
  bookSearchMenu->addItem("借阅排行榜", LIST_BOOK_BORROW);
  return bookSearchMenu;
}

// 动态增加借阅管理菜单
Menu *BookSys::addUserBookMapMgrMenu(Menu *parentMenu) {
  Menu *borrowMenu = new Menu("书籍借阅管理", parentMenu);
  borrowMenu->addItem("本人书籍借阅情况", LIST_USER_BOOK);
  borrowMenu->addItem("书籍借阅", ADD_USER_BOOK);
  borrowMenu->addItem("书籍归还", DEL_USER_BOOK);
  return borrowMenu;
}

void BookSys::execAction(int action) {
  if (action == ADMIN_LOGIN) {
    std::string account = input("管理员账户名称");
    std::string password = input("管理员账户" + account + "的密码", false);

    User *user = userService->checkUser(account);
    if (user == nullptr || user->getPassword() != password) {
      std::cout << "登录失败, 账号密码错误" << std::endl;
      return;
    }

    std::cout << "登录成功，您的角色为：" << user->getRole() << std::endl;
    currentUser = user;

  } else if (action == LIST_USER) {
    this->userService->listUsers();

  } else if (action == FIND_USER) {
    std::cin.ignore();
    std::string account = input("账户名称");
    this->userService->findUser(account);

  } else if (action == ADD_USER) {
    std::cin.ignore();
    std::string account = input("账户名称");
    std::string password = input("账户" + account + "的密码");
    std::string role = input("账户" + account + "的role");
    this->userService->addUser(account, password, role);

  } else if (action == DEL_USER) {
    std::cin.ignore();
    std::string account = input("账户名称");
    this->userService->deleteUser(account);

  } else if (action == UPD_USER) {
    std::cin.ignore();
    std::string account = input("账户名称");
    std::string password = input("账户" + account + "的密码");
    std::string role = input("账户" + account + "的role");
    this->userService->updateUser(account, password, role);

  } else if (action == LIST_BOOK) {
    this->bookService->listBooks();

  } else if (action == ADD_BOOK) {
    std::cin.ignore();
    std::string isbn = input("书籍ISBN号");
    std::string name = input("书籍(" + isbn + ")的名称");
    std::string author = input("书籍(" + isbn + ")的作者");
    std::string publish = input("书籍(" + isbn + ")的出版社");
    std::string price = input("书籍(" + isbn + ")的价格");
    std::string capacity = input("书籍(" + isbn + ")的库存");
    this->bookService->addBook(name, isbn, author, publish, price,
                               stoi(capacity));

  } else if (action == DEL_BOOK) {
    std::cin.ignore();
    std::string isbn = input("请输入书籍ISBN/ISSN编号");
    this->bookService->deleteBook(isbn);
  } else if (action == UPD_BOOK) {
    std::cin.ignore();
    std::string isbn = input("书籍ISBN号");
    std::string name = input("书籍(" + isbn + ")的名称");
    std::string author = input("书籍(" + isbn + ")的作者");
    std::string publish = input("书籍(" + isbn + ")的出版社");
    std::string price = input("书籍(" + isbn + ")的价格");
    std::string capacity = input("书籍(" + isbn + ")的库存");
    this->bookService->updateBook(name, isbn, author, publish, price,
                                  stoi(capacity));
  } else if (action == FIND_BOOK_NAME) {
    std::cin.ignore();
    std::string name = input("书籍名称");
    this->bookService->listBooksForName(name);
  } else if (action == FIND_BOOK_ISBN) {
    std::cin.ignore();
    std::string isbn = input("书籍ISBN号");
    this->bookService->findBookForISBN(isbn);
  } else if (action == FIND_BOOK_AUTHOR) {
    std::cin.ignore();
    std::string author = input("作者");
    this->bookService->listBookForAuthor(author);
  } else if (action == LIST_BOOK_BORROW) {
    this->bookService->listBooksByBorrow(10);
  } else if (action == ADD_USER_BOOK) {
    this->userBookMapService->listBooksForUser(currentUser->getAccount());
  } else if (action == DEL_USER_BOOK) {
    std::cin.ignore();
    std::string isbn = input("书籍ISBN/ISSN编号");
    this->bookService->borrowBook(isbn);
    this->userBookMapService->addUserBookMap(currentUser->getAccount(), isbn);
  } else if (action == UPD_USER_BOOK) {
    std::cin.ignore();
    std::string isbn = input("书籍ISBN/ISSN编号");
    this->userBookMapService->deleteUserBookMap(currentUser->getAccount(),
                                                isbn);
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

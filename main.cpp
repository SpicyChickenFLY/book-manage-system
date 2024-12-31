#include <iostream>
#include <cstdlib>
#include <string>

#include "src/service.h"
#include "src/sys.h"


// 主函数，程序入口
int main() {
    // 初始化 账户、书籍、借阅关系服务
    UserService *userService = new UserService("./data/user.csv");
    BookService *bookService = new BookService("./data/book.csv");
    BorrowService *borrowService = new BorrowService("./data/borrow.csv");

    // 运行图书管理系统
    System *system = new System(userService, bookService, borrowService);
    if (!system->init()) {
        std::cout << "图书管理系统初始化失败" << std::endl;
    }
    system->run();

    return 0;
}

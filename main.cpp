#include <iostream>
#include <cstdlib>
#include <string>

#include "src/service.h"
#include "src/sys.h"


// 主函数，程序入口
int main() {
    // 初始化 账户、书籍、借阅关系数据库
    UserService *userService = new UserService("./data/user.csv");
    BookService *bookService = new BookService("./data/book.csv");
    UserBookMapService *userBookMapService = new UserBookMapService("./data/book_user_map.csv");

    // 运行图书管理系统
    BookSys *bookSys = new BookSys(userService, bookService, userBookMapService);
    if (!bookSys->init()) {
        std::cout << "图书管理系统初始化失败" << std::endl;
    }
    bookSys->run();

    return 0;
}

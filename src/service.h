/*
 *  ============================================
 *                  文件数据查询服务
 *  ============================================
 */

#ifndef SERVICE_H
#define SERVICE_H

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "entity.h"

// 用户数据库
class UserService {
private:
  std::string filename;    // 文件数据库的文件名
  std::vector<User> users; // 文件读入内存后的用户数组
public:
  UserService(const std::string &fn); // 构造仅对私有成员赋值

  bool load(); // 数据库从文件加载用户数据
  bool save(); // 保存记录到文件

  void listUsers();
  void findUser(const std::string &account);
  User *checkUser(const std::string &account);

  void addUser(const std::string &account, const std::string &password,
               const std::string &role);       // 添加记录
  void deleteUser(const std::string &account); // 删除记录
  void updateUser(const std::string &account, const std::string &password,
                  const std::string &role);
  // 更新记录
};

// 书籍数据库
class BookService {
private:
  std::string filename;
  std::vector<Book> books;

public:
  BookService(const std::string &fn);

  bool load();
  bool save(); // 保存记录到文件

  void printBody(Book &book);
  void printTotal(int total);

  void listBooks();
  void listBooksForName(const std::string &name);
  void findBookForISBN(const std::string &isbn);
  void listBookForAuthor(const std::string &author);
  void listBooksByBorrow(int limit);
  Book *checkBook(const std::string &isbn);

  void addBook(const std::string &name, const std::string &isbn,
               const std::string &author, const std::string &publish,
               const std::string &price, int capacity); // 添加记录
  void deleteBook(const std::string &isbn); // 删除记录
  void updateBook(const std::string &name, const std::string &isbn,
                  const std::string &author, const std::string &publish,
                  const std::string &price, int capacity); // 更新记录
  bool borrowBook(const std::string &isbn);
  bool returnBook(const std::string &isbn);
};

// 用户书籍借阅关系数据库
class UserBookMapService {
private:
  std::string filename;
  std::vector<UserBookMap> userBookMaps;

public:
  UserBookMapService(const std::string &fn);

  bool load();
  bool save(); // 保存记录到文件
               //
  UserBookMap *checkUserBookMap(const std::string &account,
                                const std::string &isbn);
  void listBooksForUser(const std::string &account);

  void addUserBookMap(const std::string &account, const std::string &isbn); // 添加记录
  void deleteUserBookMap(const std::string &account, const std::string &isbn); // 删除记录
};

#endif // !SERVICE_H

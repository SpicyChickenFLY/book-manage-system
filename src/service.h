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

// 用户服务
class UserService {
private:
  std::string filename;    // 文件服务的文件名
  std::vector<User> users; // 文件读入内存后的用户数组
public:
  UserService(const std::string &fn); // 构造仅对私有成员赋值

  bool load(); // 服务从文件加载用户数据
  bool save(); // 保存记录到文件

  std::vector<User> listUsers();
  User *findUser(const std::string &account);
  User *checkUserLogin(User *entity);

  void addUser(User *user);                    // 添加记录
  void deleteUser(const std::string &account); // 删除记录
  void updateUser(User *user);                 // 更新记录
  // 更新记录
};

// 书籍服务
class BookService {
private:
  std::string filename;
  std::vector<Book> books;

public:
  BookService(const std::string &fn);

  bool load();
  bool save(); // 保存记录到文件

  std::vector<Book> listBooks();
  std::vector<Book> listBooksForName(const std::string &name);
  std::vector<Book> listBookForAuthor(const std::string &author);
  std::vector<Book> listBooksByBorrow(int limit);
  Book *findBookForISBN(const std::string &isbn);
  Book *checkBook(const std::string &isbn);

  void addBook(Book *book);                 // 添加记录
  void deleteBook(const std::string &isbn); // 删除记录
  void updateBook(Book *book);              // 更新记录
  bool borrowBook(const std::string &isbn); // 从馆藏中借阅
  bool returnBook(const std::string &isbn); // 归还至馆藏中
};

// 用户书籍借阅关系服务
class UserBookMapService {
private:
  std::string filename;
  std::vector<UserBookMap> userBookMaps;

public:
  UserBookMapService(const std::string &fn);

  bool load();
  bool save(); // 保存记录到文件

  UserBookMap *findUserBookMap(const std::string &account,
                                const std::string &isbn);
  std::vector<UserBookMap> listBooksForUser(const std::string &account);
  void addUserBookMap(const std::string &account,
                      const std::string &isbn); // 添加记录
  void deleteUserBookMap(const std::string &account,
                         const std::string &isbn); // 删除记录
};

#endif // !SERVICE_H

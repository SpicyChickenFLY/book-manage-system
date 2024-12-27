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

// TODO: 需要把所有的标准输出的功能外移
// service 只负责将正确的结果数组返回，不负责输出

// 用户数据库
class UserService {
private:
  std::string filename;    // 文件数据库的文件名
  std::vector<User> users; // 文件读入内存后的用户数组
public:
  UserService(const std::string &fn) : filename(fn) {} // 构造仅对私有成员赋值

  // 数据库从文件加载用户数据
  bool load() {
    // 使用文件输入流打开文件并准备读取
    std::ifstream file(filename);
    if (!file.is_open()) {
      std::cerr << "初始化账户数据库失败，无法打开文件: " << filename << std::endl;
      return false;
    }

    // 按行读取文件
    std::string line;
    while (getline(file, line)) {
      User user;
      std::stringstream ss(line); // 通过字符串流解析文件行内容
      std::string field;

      getline(ss, field,
              ','); // 获取行文本内容直到出现逗号（CSV文件以逗号进行分割）
      user.setAccount(field); // 将读取内容赋值到对象的对应成员中
      getline(ss, field, ',');
      user.setPassword(field);
      getline(ss, field, ',');
      user.setRole(field);

      users.push_back(user); // 用户数组添加新用户
    }

    file.close();
    return true;
  }

  // 保存记录到文件
  bool save() {
    // 使用文件输出流打开文件并准备写入
    std::ofstream file(filename);
    if (!file.is_open()) {
      std::cerr << "无法打开文件: " << filename << std::endl;
      return false;
    }

    // 遍历每个用户并输出用户的各个成员
    for (const User &user : users) {
      file << user.getAccount() << "," << user.getPassword() << ","
           << user.getRole() << "\n";
    }

    file.close();
    return true;
  }

  void listUsers() {
    std::cout << "|                账户 |    角色 |" << std::endl;
    std::cout << "|  ================== | ======= |" << std::endl;
    for (User &user : users) {
      std::cout << "|" << std::setw(20) << user.getAccount() << " |"
                << std::setw(8) << user.getRole() << " |" << std::endl;
    }
    std::cout << "共 " << users.size() << " 条记录" << std::endl;
  }

  void findUser(const std::string &account) {
    for (User &user : users) {
      if (user.getAccount() == account) {
        std::cout << "|                账户 |    角色 |" << std::endl;
        std::cout << "|  ================== | ======= |" << std::endl;
        std::cout << "|" << std::setw(20) << user.getAccount() << " |"
                  << std::setw(8) << user.getRole() << " |" << std::endl;
        return;
      }
    }
    std::cout << "查询用户失败, 用户(" + account + ")不存在" << std::endl;
  }

  User *checkUser(const std::string &account) {
    for (User &user : users) {
      if (user.getAccount() == account) {
        return &user;
      }
    }
    return nullptr;
  }

  // 添加记录
  void addUser(const std::string &account, const std::string &password,
               const std::string &role) {
    if (checkUser(account) != nullptr) {
      std::cout << "创建用户失败, 用户(" + account + ") 已经存在" << std::endl;
      return;
    }
    if (role != "admin" && role != "user") {
      std::cout << "创建用户失败, 角色必须是admin或者user" << std::endl;
      return;
    }
    User user;
    user.setAccount(account);
    user.setPassword(password);
    user.setRole(role);

    users.push_back(user);
    if (save()) {
      std::cout << "创建用户成功" << std::endl;
    } else {
      std::cout << "创建用户失败, 文件写入异常" << std::endl;
    }
  }

  // 删除记录
  void deleteUser(const std::string &account) {
    if (checkUser(account) == nullptr) {
      std::cout << "删除用户失败, 用户(" + account + ")不存在" << std::endl;
      return;
    }
    users.erase(remove_if(users.begin(), users.end(),
                          [account](const User &user) {
                            return user.getAccount() == account;
                          }),
                users.end());
    if (save()) {
      std::cout << "删除用户成功" << std::endl;
    } else {
      std::cout << "删除用户失败, 文件写入异常" << std::endl;
    }
  }

  // 更新记录
  void updateUser(const std::string &account, const std::string &password,
                  const std::string &role) {
    if (role != "admin" && role != "user") {
      std::cout << "更新用户失败, 角色必须是admin或user" << std::endl;
      return;
    }
    for (User &user : users) {
      if (user.getAccount() == account) {
        user.setPassword(password);
        user.setRole(role);
        if (save()) {
          std::cout << "更新用户成功" << std::endl;
        } else {
          std::cout << "更新用户失败, 文件写入异常" << std::endl;
        }
        return;
      }
    }
    std::cerr << "更新失败, 用户(" + account + ")不存在: " << std::endl;
  }
};

// 书籍数据库
class BookService {
private:
  std::string filename;
  std::vector<Book> books;

public:
  BookService(const std::string &fn) : filename(fn) {}

  bool load() {
    std::ifstream file(filename);
    if (!file.is_open()) {
      std::cerr << "初始化书籍数据库失败，无法打开文件: " << filename << std::endl;
      return false;
    }

    std::string line;
    while (getline(file, line)) {
      Book book;
      std::stringstream ss(line);
      std::string field;

      getline(ss, field, ',');
      book.setName(field);
      getline(ss, field, ',');
      book.setISBN(field);
      getline(ss, field, ',');
      book.setAuthor(field);
      getline(ss, field, ',');
      book.setPublish(field);
      getline(ss, field, ',');
      book.setPrice(field);
      getline(ss, field, ',');
      book.setCapacity(stoi(field));
      getline(ss, field, ',');
      book.setBorrow(stoi(field));

      books.push_back(book);
    }

    file.close();
    return true;
  }

  // 保存记录到文件
  bool save() {
    std::ofstream file(filename);
    if (!file.is_open()) {
      std::cerr << "无法打开文件: " << filename << std::endl;
      return false;
    }

    for (const Book &book : books) {
      file << book.getName() << "," << book.getISBN() << "," << book.getAuthor()
           << "," << book.getPublish() << "," << book.getPrice() << ","
           << book.getCapacity() << "," << book.getBorrow() << "\n";
    }

    file.close();
    return true;
  }

  void printBody(Book &book) {
    std::cout << "| 书籍名称:" << std::setw(15) << book.getName()
              << "| ISBN:" << std::setw(15) << book.getISBN()
              << "| 作者:" << std::setw(10) << book.getAuthor()
              << "| 出版社:" << std::setw(15) << book.getPublish()
              << "| 价格:" << std::setw(10) << book.getPrice()
              << "| 库存:" << std::setw(8) << book.getCapacity()
              << "| 借阅次数:" << std::setw(8) << book.getBorrow() << std::endl;
  }
  void printTotal(int total) {
    std::cout << "共 " << total << " 条记录" << std::endl;
  }

  void listBooks() {
    for (Book &book : books) {
      printBody(book);
    }
    printTotal(int(books.size()));
  }

  void listBooksForName(const std::string &name) {
    int count = 0;
    for (Book &book : books) {
      if (book.getName() != name) {
        continue;
      }
      printBody(book);
      count++;
    }
    printTotal(count);
  }

  void findBookForISBN(const std::string &isbn) {
    for (Book &book : books) {
      if (book.getISBN() == isbn) {
        printBody(book);
        return;
      }
    }
    std::cout << "查询书籍失败, 书籍ISBN/ISSN编号(" << isbn << ")不存在"
              << std::endl;
  }

  void listBookForAuthor(const std::string &author) {
    int count = 0;
    std::vector<Book> copy(books);
    sort(copy.begin(), copy.end(),
         [](Book a, Book b) { return a.getName().compare(b.getName()) < 0; });
    for (Book &book : copy) {
      if (book.getAuthor() != author) {
        continue;
      }
      printBody(book);
      count++;
    }
    printTotal(count);
  }

  void listBooksByBorrow(int limit) {
    std::vector<Book> copy(books);
    sort(copy.begin(), copy.end(),
         [](Book a, Book b) { return a.getBorrow() > b.getBorrow(); });
    int count = 0;
    for (size_t i = 0; i < std::min(int(copy.size()), limit); ++i) {
      printBody(copy[i]);
      count++;
    }
    printTotal(count);
  }

  Book *checkBook(const std::string &isbn) {
    for (Book &book : books) {
      if (book.getISBN() == isbn) {
        return &book;
      }
    }
    return nullptr;
  }

  // 添加记录
  void addBook(const std::string &name, const std::string &isbn,
               const std::string &author, const std::string &publish,
               const std::string &price, int capacity) {
    if (checkBook(isbn) != nullptr) {
      std::cout << "新增书籍失败, 书籍(" << isbn << ")已经存在" << std::endl;
      return;
    }
    Book book;
    book.setName(name);
    book.setISBN(isbn);
    book.setAuthor(author);
    book.setPublish(publish);
    book.setPrice(price);
    book.setCapacity(capacity);
    book.setBorrow(0);

    books.push_back(book);
    if (save()) {
      std::cout << "新增书籍成功" << std::endl;
    } else {
      std::cout << "新增书籍失败, 文件写入异常" << std::endl;
    }
  }

  // 删除记录
  void deleteBook(const std::string &isbn) {
    if (checkBook(isbn) == nullptr) {
      std::cout << "删除书籍失败, 书籍(" << isbn << ")不存在" << std::endl;
      return;
    }
    books.erase(
        remove_if(books.begin(), books.end(),
                  [isbn](const Book &book) { return book.getISBN() == isbn; }),
        books.end());
    if (save()) {
      std::cout << "删除书籍成功" << std::endl;
    } else {
      std::cout << "删除书籍失败, 文件写入异常" << std::endl;
    }
  }

  // 更新记录
  void updateBook(const std::string &name, const std::string &isbn,
                  const std::string &author, const std::string &publish,
                  const std::string &price, int capacity) {
    for (Book &book : books) {
      if (book.getISBN() == isbn) {
        book.setName(name);
        book.setAuthor(author);
        book.setPublish(publish);
        book.setPrice(price);
        book.setCapacity(capacity);
        if (save()) {
          std::cout << "更新书籍成功" << std::endl;
        } else {
          std::cout << "更新书籍失败, 文件写入异常" << std::endl;
        }
        return;
      }
    }
    std::cerr << "更新书籍失败, 书籍(" << isbn << ")未找到: " << std::endl;
  }

  bool borrowBook(const std::string &isbn) {
    for (Book &book : books) {
      if (book.getISBN() == isbn) {
        if (book.getCapacity() == 0) {
          std::cout << "借阅书籍失败, 库存不足" << std::endl;
          return false;
        }
        book.setCapacity(book.getCapacity() - 1);
        book.setBorrow(book.getBorrow() + 1);
        if (save()) {
          std::cout << "借阅书籍成功" << std::endl;
        } else {
          std::cout << "借阅书籍失败, 文件写入异常" << std::endl;
          return false;
        }
        return true;
      }
    }
    std::cerr << "借阅书籍失败, 书籍(" << isbn << ")未找到: " << std::endl;
    return false;
  }

  bool returnBook(const std::string &isbn) {
    for (Book &book : books) {
      if (book.getISBN() == isbn) {
        book.setCapacity(book.getCapacity() + 1);
        if (save()) {
          std::cout << "归还书籍成功" << std::endl;
        } else {
          std::cout << "归还书籍失败, 文件写入异常" << std::endl;
          return false;
        }
        return true;
      }
    }
    std::cerr << "归还书籍失败, 书籍(" << isbn << ")未找到: " << std::endl;
    return false;
  }
};

// 用户书籍借阅关系数据库
class UserBookMapService {
private:
  std::string filename;
  std::vector<UserBookMap> userBookMaps;

public:
  UserBookMapService(const std::string &fn) : filename(fn) {}

  bool load() {
    std::ifstream file(filename);
    if (!file.is_open()) {
      std::cerr << "初始化借阅信息数据库失败，无法打开文件: " << filename << std::endl;
      return false;
    }

    std::string line;
    while (getline(file, line)) {
      UserBookMap userBookMap;
      std::stringstream ss(line);
      std::string field;

      getline(ss, field, ',');
      userBookMap.setAccount(field);
      getline(ss, field, ',');
      userBookMap.setISBN(field);

      userBookMaps.push_back(userBookMap);
    }

    file.close();
    return true;
  }

  // 保存记录到文件
  bool save() {
    std::ofstream file(filename);
    if (!file.is_open()) {
      std::cerr << "无法打开文件: " << filename << std::endl;
      return false;
    }

    for (const UserBookMap &userBookMap : userBookMaps) {
      file << userBookMap.getAccount() << "," << userBookMap.getISBN() << "\n";
    }

    file.close();
    return true;
  }

  UserBookMap *checkUserBookMap(const std::string &account,
                                const std::string &isbn) {
    for (UserBookMap &userBookMap : userBookMaps) {
      if (userBookMap.getAccount() == account &&
          userBookMap.getISBN() == isbn) {
        return &userBookMap;
      }
    }
    return nullptr;
  }

  void listBooksForUser(const std::string &account) {
    std::cout << "|    ISBN  |" << std::endl;
    std::cout << "| =======  |" << std::endl;
    int count = 0;
    for (UserBookMap &userBookMap : userBookMaps) {
      if (userBookMap.getAccount() == account) {
        std::cout << "|" << std::setw(8) << userBookMap.getISBN() << " |"
                  << std::endl;
        count++;
      }
    }
    std::cout << "共 " << count << " 条记录" << std::endl;
  }

  // 添加记录
  void addUserBookMap(const std::string &account, const std::string &isbn) {
    if (checkUserBookMap(account, isbn) != nullptr) {
      std::cout << "登记借阅失败, 用户(" + account + ") 已经拥有书籍(" << isbn
                << ")" << std::endl;
      return;
    }
    UserBookMap userBookMap;
    userBookMap.setAccount(account);
    userBookMap.setISBN(isbn);

    userBookMaps.push_back(userBookMap);
    if (save()) {
      std::cout << "登记借阅成功" << std::endl;
    } else {
      std::cout << "登记借阅失败, 文件写入异常" << std::endl;
    }
  }

  // 删除记录
  void deleteUserBookMap(const std::string &account, const std::string &isbn) {
    if (checkUserBookMap(account, isbn) == nullptr) {
      std::cout << "登记归还失败, 用户(" + account + ") 未借阅书籍(" << isbn
                << ")" << std::endl;
      return;
    }
    userBookMaps.erase(
        remove_if(userBookMaps.begin(), userBookMaps.end(),
                  [account, isbn](const UserBookMap &userBookMap) {
                    return userBookMap.getAccount() == account &&
                           userBookMap.getISBN() == isbn;
                  }),
        userBookMaps.end());
    if (save()) {
      std::cout << "登记归还成功" << std::endl;
    } else {
      std::cout << "登记归还失败, 文件写入异常" << std::endl;
    }
  }
};

#endif // !SERVICE_H

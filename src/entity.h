#ifndef ENTITY_H
#define ENTITY_H

#include <stdatomic.h>
#include <string>

// 用户类
class User {
private:
  std::string account;  // 账户
  std::string password; // 密码
  std::string role;     // 角色: admin-管理员, user-用户
public:
  User() {}
  User(const std::string account, const std::string password, const std::string role)
      : account(account), password(password), role(role) {}
  // 获取私有成员
  std::string getAccount() const { return account; }
  std::string getPassword() const { return password; }
  std::string getRole() const { return role; }
  // 设置私有成员
  void setAccount(const std::string &acc) { account = acc; }
  void setPassword(const std::string &pwd) { password = pwd; }
  void setRole(const std::string &rl) { role = rl; }
};

// 书籍类
class Book {
private:
  std::string name;    // 书籍名称
  std::string isbn;    // ISBN/ISSN编号
  std::string author;  // 作者
  std::string publish; // 出版社
  std::string price;   // 价格
  int capacity;        // 库存
  int borrow;          // 借阅次数
public:
  Book() {}
  Book(const std::string name, const std::string isbn, const std::string author,
       const std::string publish, const std::string price, int capacity)
      : name(name), isbn(isbn), author(author), publish(publish), price(price),
        capacity(capacity) {}

  // 获取私有成员
  std::string getName() const { return name; }
  std::string getISBN() const { return isbn; }
  std::string getAuthor() const { return author; }
  std::string getPublish() const { return publish; }
  std::string getPrice() const { return price; }
  int getCapacity() const { return capacity; }
  int getBorrow() const { return borrow; }
  // 设置私有成员
  void setName(const std::string &n) { name = n; }
  void setISBN(const std::string &i) { isbn = i; }
  void setAuthor(const std::string &a) { author = a; }
  void setPublish(const std::string &p) { publish = p; }
  void setPrice(const std::string &p) { price = p; }
  void setCapacity(int c) { capacity = c; }
  void setBorrow(int b) { borrow = b; }
};

// 用户书籍借阅关系类
class UserBookMap {
private:
  std::string account; // 借阅账户名称
  std::string isbn;    // 借阅书籍ISBN/ISSN编号
public:
  // 获取私有成员
  std::string getAccount() const { return account; }
  std::string getISBN() const { return isbn; }
  // 设置私有成员
  void setAccount(const std::string &c) { account = c; }
  void setISBN(const std::string &i) { isbn = i; }
};

#endif // !ENTITY_H

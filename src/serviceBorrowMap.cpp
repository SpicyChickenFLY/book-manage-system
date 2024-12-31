/*
 *  ============================================
 *                  文件数据查询服务
 *  ============================================
 */

#include "service.h"
#include <vector>

BorrowService::BorrowService(const std::string &fn) : filename(fn) {}

bool BorrowService::load() {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "无法打开借阅文件: " << filename << std::endl;
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
bool BorrowService::save() {
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

std::vector<UserBookMap> BorrowService::listBooksForUser(const std::string &account) {
  std::vector<UserBookMap> result;
  for (UserBookMap &userBookMap : userBookMaps) {
    if (userBookMap.getAccount() == account) {
      result.push_back(userBookMap);
    }
  }
  return result;
}

UserBookMap *BorrowService::findUserBookMap(const std::string &account,
                              const std::string &isbn) {
  for (UserBookMap &userBookMap : userBookMaps) {
    if (userBookMap.getAccount() == account &&
        userBookMap.getISBN() == isbn) {
      return &userBookMap;
    }
  }
  return nullptr;
}

// 添加记录
void BorrowService::addUserBookMap(const std::string &account, const std::string &isbn) {
  if (findUserBookMap(account, isbn) != nullptr) {
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
void BorrowService::deleteUserBookMap(const std::string &account, const std::string &isbn) {
  if (findUserBookMap(account, isbn) == nullptr) {
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

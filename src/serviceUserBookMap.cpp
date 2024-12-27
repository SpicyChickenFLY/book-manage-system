/*
 *  ============================================
 *                  文件数据查询服务
 *  ============================================
 */

#include "service.h"

// TODO: 需要把所有的标准输出的功能外移
// service 只负责将正确的结果数组返回，不负责输出

UserBookMapService::UserBookMapService(const std::string &fn) : filename(fn) {}

bool UserBookMapService::load() {
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
bool UserBookMapService::save() {
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

UserBookMap *UserBookMapService::checkUserBookMap(const std::string &account,
                              const std::string &isbn) {
  for (UserBookMap &userBookMap : userBookMaps) {
    if (userBookMap.getAccount() == account &&
        userBookMap.getISBN() == isbn) {
      return &userBookMap;
    }
  }
  return nullptr;
}

void UserBookMapService::listBooksForUser(const std::string &account) {
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
void UserBookMapService::addUserBookMap(const std::string &account, const std::string &isbn) {
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
void UserBookMapService::deleteUserBookMap(const std::string &account, const std::string &isbn) {
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

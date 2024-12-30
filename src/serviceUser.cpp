/*
 *  ============================================
 *                  文件数据查询服务
 *  ============================================
 */

#include "service.h"

// TODO: 需要把所有的标准输出的功能外移
// service 只负责将正确的结果数组返回，不负责输出

UserService::UserService(const std::string &fn) : filename(fn) {} // 构造仅对私有成员赋值

// 数据库从文件加载用户数据
bool UserService::load() {
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
bool UserService::save() {
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

void UserService::listUsers() {
  std::cout << "|                账户 |    角色 |" << std::endl;
  std::cout << "|  ================== | ======= |" << std::endl;
  for (User &user : users) {
    std::cout << "|" << std::setw(20) << user.getAccount() << " |"
              << std::setw(8) << user.getRole() << " |" << std::endl;
  }
  std::cout << "共 " << users.size() << " 条记录" << std::endl;
}

void UserService::findUser(const std::string &account) {
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

User *UserService::checkUser(const std::string &account) {
  for (User &user : users) {
    if (user.getAccount() == account) {
      return &user;
    }
  }
  return nullptr;
}


User *UserService::checkUserLogin(User *entity) {
  std::string account = entity->getAccount();
  std::string password = entity->getPassword();
  for (User &user : users) {
    if (user.getAccount() == account && user.getPassword() == password) {
      return &user;
    }
  }
  return nullptr;
}

// 添加记录
void UserService::addUser(User *user) {
  std::string account = user->getAccount();
  std::string password = user->getPassword();
  std::string role = user->getRole();
  if (checkUser(account) != nullptr) {
    std::cout << "创建用户失败, 用户(" + account + ") 已经存在" << std::endl;
    return;
  }
  if (role != "admin" && role != "user") {
    std::cout << "创建用户失败, 角色必须是admin或者user" << std::endl;
    return;
  }

  users.push_back(*user);
  if (save()) {
    std::cout << "创建用户成功" << std::endl;
  } else {
    std::cout << "创建用户失败, 文件写入异常" << std::endl;
  }
}

// 删除记录
void UserService::deleteUser(const std::string &account) {
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
void UserService::updateUser(User *user) {
  std::string account = user->getAccount();
  std::string password = user->getPassword();
  std::string role = user->getRole();
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

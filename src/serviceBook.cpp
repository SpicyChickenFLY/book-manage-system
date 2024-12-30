/*
 *  ============================================
 *                  文件数据查询服务
 *  ============================================
 */

#include "service.h"

// TODO: 需要把所有的标准输出的功能外移
// service 只负责将正确的结果数组返回，不负责输出

BookService::BookService(const std::string &fn) : filename(fn) {}

bool BookService::load() {
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
bool BookService::save() {
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

void BookService::printBody(Book &book) {
  std::cout << "| 书籍名称:" << std::setw(15) << book.getName()
            << "| ISBN:" << std::setw(15) << book.getISBN()
            << "| 作者:" << std::setw(10) << book.getAuthor()
            << "| 出版社:" << std::setw(15) << book.getPublish()
            << "| 价格:" << std::setw(10) << book.getPrice()
            << "| 库存:" << std::setw(8) << book.getCapacity()
            << "| 借阅次数:" << std::setw(8) << book.getBorrow() << std::endl;
}
void BookService::printTotal(int total) {
  std::cout << "共 " << total << " 条记录" << std::endl;
}

void BookService::listBooks() {
  for (Book &book : books) {
    printBody(book);
  }
  printTotal(int(books.size()));
}

void BookService::listBooksForName(const std::string &name) {
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

void BookService::findBookForISBN(const std::string &isbn) {
  for (Book &book : books) {
    if (book.getISBN() == isbn) {
      printBody(book);
      return;
    }
  }
  std::cout << "查询书籍失败, 书籍ISBN/ISSN编号(" << isbn << ")不存在"
            << std::endl;
}

void BookService::listBookForAuthor(const std::string &author) {
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

void BookService::listBooksByBorrow(int limit) {
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

Book *BookService::checkBook(const std::string &isbn) {
  for (Book &book : books) {
    if (book.getISBN() == isbn) {
      return &book;
    }
  }
  return nullptr;
}

// 添加记录
void BookService::addBook(Book *entity) {
  std::string name = entity->getName();
  std::string isbn = entity->getISBN();
  std::string author = entity->getAuthor();
  std::string publish = entity->getPublish();
  std::string price = entity->getPrice();
  int capacity = entity->getCapacity();

  if (checkBook(isbn) != nullptr) {
    std::cout << "新增书籍失败, 书籍(" << isbn << ")已经存在" << std::endl;
    return;
  }

  books.push_back(*entity);
  if (save()) {
    std::cout << "新增书籍成功" << std::endl;
  } else {
    std::cout << "新增书籍失败, 文件写入异常" << std::endl;
  }
}

// 删除记录
void BookService::deleteBook(const std::string &isbn) {
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
void BookService::updateBook(Book *entity) {
  std::string name = entity->getName();
  std::string isbn = entity->getISBN();
  std::string author = entity->getAuthor();
  std::string publish = entity->getPublish();
  std::string price = entity->getPrice();
  int capacity = entity->getCapacity();

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

bool BookService::borrowBook(const std::string &isbn) {
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

bool BookService::returnBook(const std::string &isbn) {
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


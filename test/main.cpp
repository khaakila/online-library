#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

class Book {
private:
    std::string title;
    std::string author;
    std::string ISBN;
    bool isBorrowed;

public:
    Book(const std::string& title, const std::string& author, const std::string& ISBN)
        : title(title), author(author), ISBN(ISBN), isBorrowed(false) {}

    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    std::string getISBN() const { return ISBN; }
    bool getIsBorrowed() const { return isBorrowed; }

    void borrowBook() { isBorrowed = true; }
    void returnBook() { isBorrowed = false; }

    bool operator==(const Book& other) const { return ISBN == other.ISBN; }
};

class User {
private:
    std::string userID;
    std::string name;
    std::vector<Book> borrowedBooks;

public:
    User(const std::string& userID, const std::string& name) : userID(userID), name(name) {}

    std::string getUserID() const { return userID; }
    std::string getName() const { return name; }
    std::vector<Book> getBorrowedBooks() const { return borrowedBooks; }

    void borrowBook(Book& book) {
        book.borrowBook();
        borrowedBooks.push_back(book);
    }

    void returnBook(Book& book) {
        book.returnBook();
        borrowedBooks.erase(std::remove(borrowedBooks.begin(), borrowedBooks.end(), book), borrowedBooks.end());
    }
};

class Library {
private:
    std::vector<Book> books;
    std::vector<User> users;

public:
    void addBook(const Book& book) {
        books.push_back(book);
    }

    void removeBook(const std::string& ISBN) {
        books.erase(std::remove_if(books.begin(), books.end(), [&](Book& b) { return b.getISBN() == ISBN; }), books.end());
    }

    Book* searchBookByTitle(const std::string& title) {
        for (auto& book : books) {
            if (book.getTitle() == title) {
                return &book;
            }
        }
        return nullptr;
    }

    Book* searchBookByAuthor(const std::string& author) {
        for (auto& book : books) {
            if (book.getAuthor() == author) {
                return &book;
            }
        }
        return nullptr;
    }

    Book* searchBookByISBN(const std::string& ISBN) {
        for (auto& book : books) {
            if (book.getISBN() == ISBN) {
                return &book;
            }
        }
        return nullptr;
    }

    void addUser(const User& user) {
        users.push_back(user);
    }

    void removeUser(const std::string& userID) {
        users.erase(std::remove_if(users.begin(), users.end(), [&](User& u) { return u.getUserID() == userID; }), users.end());
    }

    User* getUser(const std::string& userID) {
        for (auto& user : users) {
            if (user.getUserID() == userID) {
                return &user;
            }
        }
        return nullptr;
    }
};

void testAddAndRemoveBook() {
    Library library;
    Book book1("Book Title 1", "Author 1", "ISBN001");
    library.addBook(book1);

    Book* foundBook = library.searchBookByISBN("ISBN001");
    if (foundBook != nullptr && foundBook->getTitle() == "Book Title 1") {
        std::cout << "Test addBook: Passed\n";
    } else {
        std::cout << "Test addBook: Failed\n";
    }

    library.removeBook("ISBN001");
    foundBook = library.searchBookByISBN("ISBN001");
    if (foundBook == nullptr) {
        std::cout << "Test removeBook: Passed\n";
    } else {
        std::cout << "Test removeBook: Failed\n";
    }

    // Negative test: Remove non-existent book
    library.removeBook("ISBN999");
    std::cout << "Test removeNonExistentBook: Passed\n";
}

void testSearchBook() {
    Library library;
    Book book1("Book Title 1", "Author 1", "ISBN001");
    Book book2("Book Title 2", "Author 2", "ISBN002");
    library.addBook(book1);
    library.addBook(book2);

    Book* foundBook = library.searchBookByTitle("Book Title 2");
    if (foundBook != nullptr && foundBook->getAuthor() == "Author 2") {
        std::cout << "Test searchBookByTitle: Passed\n";
    } else {
        std::cout << "Test searchBookByTitle: Failed\n";
    }

    foundBook = library.searchBookByAuthor("Author 1");
    if (foundBook != nullptr && foundBook->getTitle() == "Book Title 1") {
        std::cout << "Test searchBookByAuthor: Passed\n";
    } else {
        std::cout << "Test searchBookByAuthor: Failed\n";
    }

    foundBook = library.searchBookByISBN("ISBN001");
    if (foundBook != nullptr && foundBook->getTitle() == "Book Title 1") {
        std::cout << "Test searchBookByISBN: Passed\n";
    } else {
        std::cout << "Test searchBookByISBN: Failed\n";
    }

    // Negative tests: Search for non-existent book
    foundBook = library.searchBookByTitle("Non Existent Title");
    if (foundBook == nullptr) {
        std::cout << "Test searchNonExistentBookByTitle: Passed\n";
    } else {
        std::cout << "Test searchNonExistentBookByTitle: Failed\n";
    }

    foundBook = library.searchBookByAuthor("Non Existent Author");
    if (foundBook == nullptr) {
        std::cout << "Test searchNonExistentBookByAuthor: Passed\n";
    } else {
        std::cout << "Test searchNonExistentBookByAuthor: Failed\n";
    }

    foundBook = library.searchBookByISBN("ISBN999");
    if (foundBook == nullptr) {
        std::cout << "Test searchNonExistentBookByISBN: Passed\n";
    } else {
        std::cout << "Test searchNonExistentBookByISBN: Failed\n";
    }
}

void testUserManagement() {
    Library library;
    User user1("user1", "User One");
    library.addUser(user1);

    User* foundUser = library.getUser("user1");
    if (foundUser != nullptr && foundUser->getName() == "User One") {
        std::cout << "Test addUser: Passed\n";
    } else {
        std::cout << "Test addUser: Failed\n";
    }

    library.removeUser("user1");
    foundUser = library.getUser("user1");
    if (foundUser == nullptr) {
        std::cout << "Test removeUser: Passed\n";
    } else {
        std::cout << "Test removeUser: Failed\n";
    }

    // Negative test: Remove non-existent user
    library.removeUser("user999");
    std::cout << "Test removeNonExistentUser: Passed\n";
}

void testBorrowAndReturnBook() {
    Library library;
    Book book1("Book Title 1", "Author 1", "ISBN001");
    library.addBook(book1);

    User user1("user1", "User One");
    library.addUser(user1);

    User* foundUser = library.getUser("user1");
    Book* foundBook = library.searchBookByISBN("ISBN001");

    foundUser->borrowBook(*foundBook);
    if (foundBook->getIsBorrowed() && !foundUser->getBorrowedBooks().empty()) {
        std::cout << "Test borrowBook: Passed\n";
    } else {
        std::cout << "Test borrowBook: Failed\n";
    }

    foundUser->returnBook(*foundBook);
    if (!foundBook->getIsBorrowed() && foundUser->getBorrowedBooks().empty()) {
        std::cout << "Test returnBook: Passed\n";
    } else {
        std::cout << "Test returnBook: Failed\n";
    }

    // Negative test: Borrow an already borrowed book
    foundUser->borrowBook(*foundBook);
    User user2("user2", "User Two");
    library.addUser(user2);
    User* foundUser2 = library.getUser("user2");
    foundUser2->borrowBook(*foundBook); // This should not be allowed
    if (foundUser2->getBorrowedBooks().empty()) {
        std::cout << "Test borrowAlreadyBorrowedBook: Passed\n";
    } else {
        std::cout << "Test borrowAlreadyBorrowedBook: Failed\n";
    }

    // Negative test: Return a book not borrowed by the user
    Book book2("Book Title 2", "Author 2", "ISBN002");
    library.addBook(book2);
    foundUser2->borrowBook(book2);
    foundUser2->returnBook(book1); // This should not affect book1
    if (book1.getIsBorrowed()) {
        std::cout << "Test returnBookNotBorrowedByUser: Passed\n";
    } else {
        std::cout << "Test returnBookNotBorrowedByUser: Failed\n";
    }
}

int main() {
    testAddAndRemoveBook();
    testSearchBook();
    testUserManagement();
    testBorrowAndReturnBook();
    return 0;
}

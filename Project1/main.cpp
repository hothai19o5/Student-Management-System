#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <functional>

using namespace std;

const int TABLE_SIZE = 1000;

struct student
{
    string name;
    char id[9];
    int age;
    double admissionScore;
    double gpa;

    student()
    {
        name = "";
        id[0] = '\0';
        age = 0;
        admissionScore = 0;
        gpa = 0;
    }
    student(string name, char id[], int age, double admissionScore, double gpa)
    {
        this->name = name;
        strncpy(this->id, id, 8);
        this->id[8] = '\0';
        this->age = age;
        this->admissionScore = admissionScore;
        this->gpa = gpa;
    }
};

struct HashTable {
    vector<string> table[TABLE_SIZE];  // Bảng băm chứa mã sinh viên

    // Hàm băm tính toán chỉ số băm dựa trên id
    int hashFunction(const string& id) {
        int hashValue = 0;
        for (char c : id) {
            hashValue = (hashValue * 31 + c) % TABLE_SIZE;
        }
        return hashValue;
    }

    // Thêm mã sinh viên vào bảng băm
    void insert(const string& id) {
        int index = hashFunction(id);
        table[index].push_back(id);  // Chèn vào vị trí tương ứng
    }

    // Kiểm tra sinh viên có trong bảng băm không
    bool contains(const string& id) {
        int index = hashFunction(id);
        for (const string& studentId : table[index]) {
            if (studentId == id)
                return true;
        }
        return false;
    }
};

void saveStudent(student& s){
    ofstream file("students.txt", ios::app); // ofstream is used to write to a file, ios::app is used to append to the file
    file << s.name << " " << s.id << " " << s.age << " " << s.admissionScore << " " << s.gpa << endl;
    file.close();
}

class AVLTree {
public:
    struct AVLNode {
        string id;  // Mã sinh viên, khóa tìm kiếm
        student data;  // Thông tin sinh viên
        int height;
        AVLNode* left;
        AVLNode* right;

        AVLNode(string id, student data) : id(id), data(data), height(1), left(nullptr), right(nullptr) {}
    };

    AVLNode* root;

    // Các hàm xử lý cây AVL: tính chiều cao, xoay, cập nhật chiều cao, cân bằng cây
    int height(AVLNode* node) {
        return node ? node->height : 0;
    }

    void updateHeight(AVLNode* node) {
        node->height = max(height(node->left), height(node->right)) + 1;
    }

    AVLNode* rotateLeft(AVLNode* node) {
        AVLNode* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        updateHeight(node);
        updateHeight(newRoot);
        return newRoot;
    }

    AVLNode* rotateRight(AVLNode* node) {
        AVLNode* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;
        updateHeight(node);
        updateHeight(newRoot);
        return newRoot;
    }

    AVLNode* balance(AVLNode* node) {
        int balanceFactor = height(node->left) - height(node->right);
        if (balanceFactor > 1) {
            if (height(node->left->left) >= height(node->left->right))
                return rotateRight(node);  // Xoay phải
            else {
                node->left = rotateLeft(node->left);  // Xoay trái
                return rotateRight(node);  // Xoay phải
            }
        }
        if (balanceFactor < -1) {
            if (height(node->right->right) >= height(node->right->left))
                return rotateLeft(node);  // Xoay trái
            else {
                node->right = rotateRight(node->right);  // Xoay phải
                return rotateLeft(node);  // Xoay trái
            }
        }
        return node;
    }

    AVLNode* insert(AVLNode* node, string id, student data) {
        if (node == nullptr)
            return new AVLNode(id, data);

        if (id < node->id)
            node->left = insert(node->left, id, data);
        else if (id > node->id)
            node->right = insert(node->right, id, data);
        else
            return node;  // Không thêm nếu mã sinh viên trùng

        updateHeight(node);
        return balance(node);
    }

    void inorderTraversal(AVLNode* node) {
        if (node) {
            inorderTraversal(node->left);
            cout << "| ID: " << node->id << " | Name: " << node->data.name << " | Age: " << node->data.age << " | Admission Score: " << fixed << setprecision(2) << node->data.admissionScore << " | GPA: " << fixed << setprecision(2) << node->data.gpa << " |" << endl;
            cout << "|--------------------------------------------------------------------------------|" << endl;
            inorderTraversal(node->right);
        }
    }

    AVLTree() : root(nullptr) {}

    void insertStudent(string id, student data) {
        root = insert(root, id, data);
    }

    void display() {
        inorderTraversal(root);
    }

    // Hàm tìm kiếm và cập nhật sinh viên trong cây AVL
    bool updateStudentInAVL(AVLNode* node, const string& id, const student& updatedData) {
        if (!node) return false;

        if (id < node->id) {
            // Tìm trong cây con trái
            return updateStudentInAVL(node->left, id, updatedData);
        } else if (id > node->id) {
            // Tìm trong cây con phải
            return updateStudentInAVL(node->right, id, updatedData);
        } else {
            // Tìm thấy sinh viên, cập nhật thông tin
            node->data = updatedData;
            return true;
        }
    }

    // Hàm tiện ích gọi hàm cập nhật từ bên ngoài
    void updateStudent(const string& id, const student& updatedData) {
        if (updateStudentInAVL(root, id, updatedData)) {
            cout << "|--------------------------------------------------------------------------------|" << endl;
            cout << "|                    Student information updated successfully!                   |" << endl;
            cout << "|--------------------------------------------------------------------------------|" << endl;
        } else {
            cout << "|--------------------------------------------------------------------------------|" << endl;
            cout << "|                                Student not found!                              |" << endl;
            cout << "|--------------------------------------------------------------------------------|" << endl;
        }
    }

    // Hàm tìm nút nhỏ nhất trong cây con phải (dùng khi xóa nút có hai con)
    AVLNode* findMin(AVLNode* node) {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    // Hàm xóa sinh viên trong cây AVL theo ID
    AVLNode* remove(AVLNode* node, const string& id) {
        if (!node) return nullptr;

        if (id < node->id)
            node->left = remove(node->left, id);
        else if (id > node->id)
            node->right = remove(node->right, id);
        else {
            // Đã tìm thấy nút cần xóa
            if (!node->left || !node->right) {
                AVLNode* temp = node->left ? node->left : node->right;
                delete node;
                return temp;
            } else {
                // Trường hợp có hai con
                AVLNode* temp = findMin(node->right);
                node->data = temp->data;
                node->id = temp->id;
                node->right = remove(node->right, temp->id);
            }
        }

        return balance(node);
    }

    // Hàm tiện ích gọi hàm xóa từ bên ngoài
    void removeStudent(const string& id) {
        root = remove(root, id);
    }

};

student searchStudent(AVLTree& avlTree, string id){
    AVLTree::AVLNode* node = avlTree.root;
    while (node != nullptr){
        if (id < node->id)
            node = node->left;
        else if (id > node->id)
            node = node->right;
        else
            return node->data;
    }
    return student();
}

// Hàm xóa sinh viên trong file
void removeStudentFromFile(const string& id) {
    ifstream inputFile("students.txt");
    ofstream tempFile("temp.txt");

    if (!inputFile || !tempFile) {
        cerr << "Error opening file!" << endl;
        return;
    }

    string name, studentID;
    int age;
    double admissionScore, gpa;

    // Đọc từng dòng từ file và sao chép sang file tạm, trừ sinh viên cần xóa
    while (inputFile >> name >> studentID >> age >> admissionScore >> gpa) {
        if (studentID != id) {
            tempFile << name << " " << studentID << " " << age << " " << admissionScore << " " << gpa << endl;
        }
    }

    inputFile.close();
    tempFile.close();

    // Xóa file gốc và đổi tên file tạm thành file gốc
    remove("students.txt");
    rename("temp.txt", "students.txt");
}


// Hàm xóa sinh viên theo ID từ cả cây AVL và file
void deleteStudent(AVLTree& avlTree, const string& id) {
    avlTree.removeStudent(id);
    removeStudentFromFile(id);
    cout << "|--------------------------------------------------------------------------------|" << endl;
    cout << "|        Student with ID " << id << " has been removed from AVL tree and file        |" << endl;
    cout << "|--------------------------------------------------------------------------------|" << endl;
}

int main() {
    HashTable hashTable;
    AVLTree avlTree;

    // Giả sử dữ liệu được lưu trong file "students.txt"
    ifstream file("students.txt");
    if (!file) {
        cout << "|--------------------------------------------------------------------------------|" << endl;
        cout << "|                               Cannot open file!                                |" << endl;
        cout << "|--------------------------------------------------------------------------------|" << endl;
        return 1;
    }

    string name, id;
    int age;
    double admissionScore, gpa;

    // Đọc dữ liệu từ file và lưu vào bảng băm và cây AVL
    while (file >> name >> id >> age >> admissionScore >> gpa) {
        student s(name, &id[0], age, admissionScore, gpa);

        // Kiểm tra bảng băm trước khi thêm vào cây AVL
        if (!hashTable.contains(id)) {
            hashTable.insert(id);  // Thêm mã sinh viên vào bảng băm
            avlTree.insertStudent(id, s);  // Thêm sinh viên vào cây AVL
        }
    }

    string choice = "";
    while(choice != "#"){
        cout << "|--------------------------------------------------------------------------------|" << endl;
        cout << "|                           Enter your choice                                    |" << endl;
        cout << "|                           1. Search Student With Id                            |" << endl;
        cout << "|                           2. Update Student Information                        |" << endl;
        cout << "|                           3. Delete Student                                    |" << endl;
        cout << "|                           #. Exit                                              |" << endl;
        cout << "|--------------------------------------------------------------------------------|" << endl;

        cin >> choice;

        if(choice == "1"){
            string id;
            cout << "|--------------------------------------------------------------------------------|" << endl;
            cout << "|                           Enter student id to search                           |" << endl;
            cout << "|--------------------------------------------------------------------------------|" << endl;
            cin >> id;
            student s = searchStudent(avlTree, id);
            if(s.name == ""){
                cout << "|--------------------------------------------------------------------------------|" << endl;
                cout << "|                           Student not found!                                   |" << endl;
                cout << "|--------------------------------------------------------------------------------|" << endl;
            } else {
                cout << "|--------------------------------------------------------------------------------|" << endl;
                cout << "| ID: " << id << " | Name: " << s.name << " | Age: " << s.age << " | Admission Score: " << fixed << setprecision(2) << s.admissionScore << " | GPA: " << fixed << setprecision(2) << s.gpa << " |" << endl;
                cout << "|--------------------------------------------------------------------------------|" << endl;
            }
        } else if(choice == "2"){
            string id;
            cout << "|--------------------------------------------------------------------------------|" << endl;
            cout << "|                           Enter student id to update                           |" << endl;
            cout << "|--------------------------------------------------------------------------------|" << endl;
            cin >> id;
            string name;
            int age;
            double admissionScore, gpa;
            cout << "|--------------------------------------------------------------------------------|" << endl;
            cout << "|                           Enter student name                                   |" << endl;
            cout << "|--------------------------------------------------------------------------------|" << endl;
            cin >> name;
            cout << "|--------------------------------------------------------------------------------|" << endl;
            cout << "|                           Enter student age                                    |" << endl;
            cout << "|--------------------------------------------------------------------------------|" << endl;
            cin >> age;
            cout << "|--------------------------------------------------------------------------------|" << endl;
            cout << "|                           Enter student admission score                        |" << endl;
            cout << "|--------------------------------------------------------------------------------|" << endl;
            cin >> admissionScore;
            cout << "|--------------------------------------------------------------------------------|" << endl;
            cout << "|                           Enter student gpa                                    |" << endl;
            cout << "|--------------------------------------------------------------------------------|" << endl;
            cin >> gpa;
            student s(name, &id[0], age, admissionScore, gpa);
            avlTree.updateStudent(id, s);
        } else if(choice == "3"){
            string id;
            cout << "|--------------------------------------------------------------------------------|" << endl;
            cout << "|                           Enter student id to delete                           |" << endl;
            cout << "|--------------------------------------------------------------------------------|" << endl;
            cin >> id;
            deleteStudent(avlTree, id);
        } else if(choice == "#"){
            break;
        } else {
            cout << "|--------------------------------------------------------------------------------|" << endl;
            cout << "|                           Invalid choice!                                      |" << endl;
            cout << "|--------------------------------------------------------------------------------|" << endl;
        }
    }

    return 0;
}
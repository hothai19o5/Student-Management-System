#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Cấu trúc lưu trữ thông tin sinh viên
struct SinhVien {
    string maSV;
    string hoTen;
    int namSinh;
    float diemTrungTuyen;
    float diemTB;
};

// Hàm băm tự tạo
int hashFunction(string maSV) {
    int sum = 0;
    for (char c : maSV) {
        sum += c;
    }
    return sum % 100000; // Lấy hàm băm có kích thước 100000 (Tránh đụng độ)
}

// Cấu trúc Node cho bảng băm
struct HashNode {
    string maSV;
    SinhVien* data; // Con trỏ đến dữ liệu sinh viên
    HashNode* next;

    HashNode(string maSV, SinhVien* sv) {
        this->maSV = maSV;
        this->data = sv;
        this->next = nullptr;
    }
};

// Cấu trúc Node cho cây cân bằng (ở đây dùng AVL Tree)
struct AVLNode {
    SinhVien data;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(SinhVien sv) {
        data = sv;
        left = right = nullptr;
        height = 1;
    }
};

// Hàm lấy chiều cao của node
int getHeight(AVLNode* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

// Hàm tính hệ số cân bằng
int getBalanceFactor(AVLNode* node) {
    if (node == nullptr)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// Hàm xoay phải
AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

// Hàm xoay trái
AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

// Hàm thêm node vào cây AVL
AVLNode* insertAVL(AVLNode* node, SinhVien sv) {
    if (node == nullptr)
        return new AVLNode(sv);

    if (sv.maSV < node->data.maSV)
        node->left = insertAVL(node->left, sv);
    else if (sv.maSV > node->data.maSV)
        node->right = insertAVL(node->right, sv);
    else // Không cho phép trùng mã sinh viên
        return node;

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    int balance = getBalanceFactor(node);

    // Left Left Case
    if (balance > 1 && sv.maSV < node->left->data.maSV)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && sv.maSV > node->right->data.maSV)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && sv.maSV > node->left->data.maSV) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && sv.maSV < node->right->data.maSV) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Hàm tìm kiếm sinh viên trong cây AVL
AVLNode* searchAVL(AVLNode* root, string maSV) {
    if (root == nullptr || root->data.maSV == maSV)
        return root;

    if (maSV < root->data.maSV)
        return searchAVL(root->left, maSV);

    return searchAVL(root->right, maSV);
}

// Hàm tìm node có giá trị nhỏ nhất (ngoài cùng bên trái)
AVLNode* minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

// Hàm xóa node khỏi cây AVL
AVLNode* deleteAVL(AVLNode* root, string maSV) {
    if (root == nullptr)
        return root;

    if (maSV < root->data.maSV)
        root->left = deleteAVL(root->left, maSV);
    else if (maSV > root->data.maSV)
        root->right = deleteAVL(root->right, maSV);
    else {
        // Node có 1 con hoặc không có con
        if ((root->left == nullptr) || (root->right == nullptr)) {
            AVLNode* temp = root->left ? root->left : root->right;
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else
                *root = *temp;
            delete temp;
        } else {
            // Node có 2 con
            AVLNode* temp = minValueNode(root->right);
            root->data = temp->data;
            root->right = deleteAVL(root->right, temp->data.maSV);
        }
    }

    if (root == nullptr)
        return root;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    int balance = getBalanceFactor(root);

    // Left Left Case
    if (balance > 1 && getBalanceFactor(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalanceFactor(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalanceFactor(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalanceFactor(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Hàm nhập thông tin sinh viên từ bàn phím
SinhVien nhapSinhVien() {
    SinhVien sv;
    cout << "Nhap ma sinh vien (8 ky tu): ";
    cin >> sv.maSV;
    while (sv.maSV.length() != 8) {
        cout << "Ma sinh vien phai co 8 ky tu. Nhap lai: ";
        cin >> sv.maSV;
    }
    cout << "Nhap ho ten: ";
    cin.ignore();
    getline(cin, sv.hoTen);
    cout << "Nhap nam sinh: ";
    cin >> sv.namSinh;
    cout << "Nhap diem trung tuyen: ";
    cin >> sv.diemTrungTuyen;
    cout << "Nhap diem trung binh: ";
    cin >> sv.diemTB;
    return sv;
}

// Hàm ghi thông tin sinh viên vào tệp
void ghiFile(string tenFile, vector<SinhVien>& danhSachSV) {
    ofstream outFile(tenFile);
    if (outFile.is_open()) {
        for (SinhVien sv : danhSachSV) {
            outFile << sv.maSV << "," << sv.hoTen << "," << sv.namSinh << ","
                << sv.diemTrungTuyen << "," << sv.diemTB << endl;
        }
        outFile.close();
        cout << "Ghi thong tin vao tep " << tenFile << " thanh cong." << endl;
    } else {
        cout << "Khong the mo tep de ghi." << endl;
    }
}

// Hàm đọc thông tin sinh viên từ tệp và lưu vào bảng băm
void docFile_BangBam(string tenFile, HashNode* hashTable[], int size) {
    ifstream inFile(tenFile);
    if (inFile.is_open()) {
        string line;
        while (getline(inFile, line)) {
            SinhVien sv;
            size_t pos = 0;
            string token;
            int i = 0;
            while ((pos = line.find(",")) != string::npos) {
                token = line.substr(0, pos);
                switch (i) {
                    case 0: sv.maSV = token; break;
                    case 1: sv.hoTen = token; break;
                    case 2: sv.namSinh = stoi(token); break;
                    case 3: sv.diemTrungTuyen = stof(token); break;
                }
                line.erase(0, pos + 1);
                i++;
            }
            sv.diemTB = stof(line);

            // Tạo bản sao của sinh viên
            SinhVien* newSV = new SinhVien(sv);

            // Thêm vào bảng băm
            int index = hashFunction(sv.maSV);
            HashNode* newNode = new HashNode(sv.maSV, newSV);
            if (hashTable[index] == nullptr) {
                // Nếu không bị đụng độ thì thêm vào đầu danh sách liên kết
                hashTable[index] = newNode;
            } else {
                // Nếu bị đụng độ thì thêm vào cuối danh sách liên kết
                HashNode* current = hashTable[index];
                while (current->next != nullptr) {
                    current = current->next;
                }
                current->next = newNode;
            }
        }
        inFile.close();
        cout << "Doc tep va luu vao bang bam thanh cong." << endl;
    } else {
        cout << "Khong the mo tep de doc." << endl;
    }
}

// Hàm đọc thông tin sinh viên từ tệp và lưu vào cây AVL
void docFile_CayAVL(string tenFile, AVLNode*& root) {
    ifstream inFile(tenFile);
    if (inFile.is_open()) {
        string line;
        while (getline(inFile, line)) {
            SinhVien sv;
            size_t pos = 0;
            string token;
            int i = 0;
            while ((pos = line.find(",")) != string::npos) {
                token = line.substr(0, pos);
                switch (i) {
                    case 0: sv.maSV = token; break;
                    case 1: sv.hoTen = token; break;
                    case 2: sv.namSinh = stoi(token); break;
                    case 3: sv.diemTrungTuyen = stof(token); break;
                }
                line.erase(0, pos + 1);
                i++;
            }
            sv.diemTB = stof(line);
            root = insertAVL(root, sv);
        }
        inFile.close();
        cout << "Doc tep va luu vao cay AVL thanh cong." << endl;
    } else {
        cout << "Khong the mo tep de doc." << endl;
    }
}

// Hàm tra cứu thông tin sinh viên từ bảng băm
void traCuu_BangBam(HashNode* hashTable[], int size, string maSV) {
    int index = hashFunction(maSV);
    HashNode* current = hashTable[index];
    while (current != nullptr) {
        if (current->maSV == maSV) {
            cout << "Thong tin sinh vien:" << endl;
            cout << "Ma SV: " << current->data->maSV << endl;
            cout << "Ho ten: " << current->data->hoTen << endl;
            cout << "Nam sinh: " << current->data->namSinh << endl;
            cout << "Diem trung tuyen: " << current->data->diemTrungTuyen << endl;
            cout << "Diem TB: " << current->data->diemTB << endl;
            return;
        }
        current = current->next;
    }
    cout << "Khong tim thay sinh vien co ma " << maSV << endl;
}

// Hàm tra cứu thông tin sinh viên từ cây AVL
void traCuu_CayAVL(AVLNode* root, string maSV) {
    AVLNode* result = searchAVL(root, maSV);
    if (result != nullptr) {
        cout << "Thong tin sinh vien:" << endl;
        cout << "Ma SV: " << result->data.maSV << endl;
        cout << "Ho ten: " << result->data.hoTen << endl;
        cout << "Nam sinh: " << result->data.namSinh << endl;
        cout << "Diem trung tuyen: " << result->data.diemTrungTuyen << endl;
        cout << "Diem TB: " << result->data.diemTB << endl;
    } else {
        cout << "Khong tim thay sinh vien co ma " << maSV << endl;
    }
}

// Hàm cập nhật điểm của sinh viên (cây AVL và tệp)
void capNhatDiem(AVLNode*& root, string tenFile, string maSV, float diemMoi) {
    AVLNode* svNode = searchAVL(root, maSV);
    if (svNode != nullptr) {
        svNode->data.diemTB = diemMoi;
        cout << "Cap nhat diem thanh cong." << endl;

        // Cập nhật tệp
        vector<SinhVien> danhSachSV;
        ifstream inFile(tenFile);
        if (inFile.is_open()) {
            string line;
            while (getline(inFile, line)) {
                SinhVien sv;
                size_t pos = 0;
                string token;
                int i = 0;
                while ((pos = line.find(",")) != string::npos) {
                    token = line.substr(0, pos);
                    switch (i) {
                        case 0: sv.maSV = token; break;
                        case 1: sv.hoTen = token; break;
                        case 2: sv.namSinh = stoi(token); break;
                        case 3: sv.diemTrungTuyen = stof(token); break;
                    }
                    line.erase(0, pos + 1);
                    i++;
                }
                sv.diemTB = stof(line);
                if (sv.maSV == maSV) {
                    sv.diemTB = diemMoi;
                }
                danhSachSV.push_back(sv);
            }
            inFile.close();

            // Ghi lại vào tệp
            ghiFile(tenFile, danhSachSV);
        } else {
            cout << "Khong the mo tep de cap nhat." << endl;
        }
    } else {
        cout << "Khong tim thay sinh vien co ma " << maSV << endl;
    }
}

// Hàm xóa sinh viên (cây AVL, bảng băm và tệp)
void xoaSinhVien(AVLNode*& root, HashNode* hashTable[], int hashSize, string tenFile, string maSV) {
    // Xóa khỏi cây AVL
    root = deleteAVL(root, maSV);

    // Xóa khỏi bảng băm
    int index = hashFunction(maSV);
    HashNode* current = hashTable[index];
    HashNode* prev = nullptr;
    while (current != nullptr) {
        if (current->maSV == maSV) {
            if (prev == nullptr) {
                hashTable[index] = current->next;
            } else {
                prev->next = current->next;
            }
            delete current->data; // Giải phóng bộ nhớ của sinh viên
            delete current;
            break;
        }
        prev = current;
        current = current->next;
    }

    // Xóa khỏi tệp
    vector<SinhVien> danhSachSV;
    ifstream inFile(tenFile);
    if (inFile.is_open()) {
        string line;
        while (getline(inFile, line)) {
            SinhVien sv;
            size_t pos = 0;
            string token;
            int i = 0;
            while ((pos = line.find(",")) != string::npos) {
                token = line.substr(0, pos);
                switch (i) {
                    case 0: sv.maSV = token; break;
                    case 1: sv.hoTen = token; break;
                    case 2: sv.namSinh = stoi(token); break;
                    case 3: sv.diemTrungTuyen = stof(token); break;
                }
                line.erase(0, pos + 1);
                i++;
            }
            sv.diemTB = stof(line);
            if (sv.maSV != maSV) {
                danhSachSV.push_back(sv);
            }
        }
        inFile.close();

        // Ghi lại vào tệp
        ghiFile(tenFile, danhSachSV);
        cout << "Da xoa sinh vien co ma " << maSV << endl;
    } else {
        cout << "Khong the mo tep de xoa." << endl;
    }
}

// Hàm giải phóng bộ nhớ của bảng băm
void clearHashTable(HashNode* hashTable[], int size) {
    for (int i = 0; i < size; i++) {
        HashNode* current = hashTable[i];
        while (current != nullptr) {
            HashNode* temp = current;
            current = current->next;
            delete temp->data; // Giải phóng bộ nhớ của sinh viên
            delete temp;
        }
        hashTable[i] = nullptr;
    }
}

// Hàm giải phóng bộ nhớ của cây AVL
void clearAVLTree(AVLNode* root) {
    if (root != nullptr) {
        clearAVLTree(root->left);
        clearAVLTree(root->right);
        delete root;
    }
}

// Hàm kiểm tra trùng sinh viên trong bảng băm
bool kiemTraTrungMaSV(HashNode* hashTable[], int hashSize, const string& maSV) {
    int index = hashFunction(maSV);
    HashNode* current = hashTable[index];
    while (current != nullptr) {
        if (current->maSV == maSV) {
            return true; // Trùng mã
        }
        current = current->next;
    }
    return false; // Không trùng mã
}

// Hàm main
int main() {
    string tenFile = "sinhvien.txt";
    vector<SinhVien> danhSachSV;
    const int HASH_TABLE_SIZE = 100000;
    HashNode* hashTable[HASH_TABLE_SIZE] = { nullptr };
    AVLNode* root = nullptr;

    int choice;
    do {
        cout << "\nMENU:" << endl;
        cout << "1. Nhap thong tin sinh vien va ghi vao tep" << endl;
        cout << "2. Doc tep va luu vao bang bam" << endl;
        cout << "3. Doc tep va luu vao cay AVL" << endl;
        cout << "4. Tra cuu thong tin sinh vien (bang bam)" << endl;
        cout << "5. Tra cuu thong tin sinh vien (cay AVL)" << endl;
        cout << "6. Cap nhat diem TB sinh vien" << endl;
        cout << "7. Xoa sinh vien" << endl;
        cout << "0. Thoat" << endl;
        cout << "Nhap lua chon cua ban: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int n;
                cout << "Ghi de vao file cu (1) Them vao sau file cu (2) Huy (3) ? ";
                int mode;
                cin >> mode;
                if (mode == 1) {     // Ghi đè vào file cũ
                    cout << "Nhap so luong sinh vien: ";
                    cin >> n;
                    danhSachSV.clear(); // Xóa nội dung cũ của danhSachSV
                    for (int i = 0; i < n; i++) {
                        danhSachSV.push_back(nhapSinhVien());
                    }
                    ghiFile(tenFile, danhSachSV);
                    break;
                } else if (mode == 2) {   // Thêm vào file cũ
                    ofstream outFile(tenFile, ios::app);
                    if (outFile.is_open()) {
                        cout << "Nhap so luong sinh vien: ";
                        cin >> n;
                        docFile_BangBam(tenFile, hashTable, HASH_TABLE_SIZE); // Đọc file và lưu vào bảng băm
                        docFile_CayAVL(tenFile, root);  //Đọc file và lưu vào cây AVL
                        for (int i = 0; i < n; i++) {
                            SinhVien sv;
                            bool trungMaSV;
                            do {
                                sv = nhapSinhVien();
                                trungMaSV = kiemTraTrungMaSV(hashTable, HASH_TABLE_SIZE, sv.maSV);
                                if (trungMaSV) {
                                    cout << "Ma sinh vien da ton tai. Vui long nhap lai." << endl;
                                }
                            } while (trungMaSV);

                            // Nếu không trùng mã sinh viên, thêm vào bảng băm, cây AVL và ghi vào file
                            int index = hashFunction(sv.maSV);
                            SinhVien* newSV = new SinhVien(sv);
                            HashNode* newNode = new HashNode(sv.maSV, newSV);
                            if (hashTable[index] == nullptr) {
                                hashTable[index] = newNode;
                            } else {
                                HashNode* current = hashTable[index];
                                while (current->next != nullptr) {
                                    current = current->next;
                                }
                                current->next = newNode;
                            }

                            root = insertAVL(root, sv);

                            outFile << sv.maSV << "," << sv.hoTen << "," << sv.namSinh << ","
                                    << sv.diemTrungTuyen << "," << sv.diemTB << endl;
                        }
                        outFile.close();
                        cout << "Ghi thong tin vao tep " << tenFile << " thanh cong." << endl;
                    } else {
                        cout << "Khong the mo tep de ghi." << endl;
                    }
                    break;
                } else {
                    break;
                }
            }
            case 2:
                docFile_BangBam(tenFile, hashTable, HASH_TABLE_SIZE);
                break;
            case 3:
                docFile_CayAVL(tenFile, root);
                break;
            case 4: {
                string maSV;
                cout << "Nhap ma sinh vien can tra cuu: ";
                cin >> maSV;
                traCuu_BangBam(hashTable, HASH_TABLE_SIZE, maSV);
                break;
            }
            case 5: {
                string maSV;
                cout << "Nhap ma sinh vien can tra cuu: ";
                cin >> maSV;
                traCuu_CayAVL(root, maSV);
                break;
            }
            case 6: {
                string maSV;
                float diemMoi;
                cout << "Nhap ma sinh vien can cap nhat diem: ";
                cin >> maSV;
                cout << "Nhap diem moi: ";
                cin >> diemMoi;
                capNhatDiem(root, tenFile, maSV, diemMoi);
                break;
            }
            case 7: {
                string maSV;
                cout << "Nhap ma sinh vien can xoa: ";
                cin >> maSV;
                xoaSinhVien(root, hashTable, HASH_TABLE_SIZE, tenFile, maSV);
                break;
            }
            case 0:
                cout << "Ket thuc chuong trinh." << endl;
                break;
            default:
                cout << "Lua chon khong hop le. Vui long nhap lai." << endl;
        }
    } while (choice != 0);

    // Giải phóng bộ nhớ
    clearHashTable(hashTable, HASH_TABLE_SIZE);
    clearAVLTree(root);

    return 0;
}
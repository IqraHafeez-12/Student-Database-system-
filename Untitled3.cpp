#include <iostream>
#include <string>
using namespace std;


struct Student {
    int rollNumber;
    string name;
    Student* left;
    Student* right;
    int height;

    Student(int roll, string name) : rollNumber(roll), name(name), left(nullptr), right(nullptr), height(1) {}
};


class StudentDatabase {
private:
    Student* root;

    
    int height(Student* node) {
        return node ? node->height : 0;
    }

    
    int getBalance(Student* node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    // Right rotate utility
    Student* rotateRight(Student* y) {
        Student* x = y->left;
        Student* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    // Left rotate utility
    Student* rotateLeft(Student* x) {
        Student* y = x->right;
        Student* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    
    Student* insert(Student* node, int roll, string name) {
        if (node == nullptr) {
            return new Student(roll, name);
        }
        if (roll < node->rollNumber) {
            node->left = insert(node->left, roll, name);
        } else if (roll > node->rollNumber) {
            node->right = insert(node->right, roll, name);
        } else {
            return node;
        }

        node->height = 1 + max(height(node->left), height(node->right));

        int balance = getBalance(node);

        // Left Left Case
        if (balance > 1 && roll < node->left->rollNumber) {
            return rotateRight(node);
        }

        // Right Right Case
        if (balance < -1 && roll > node->right->rollNumber) {
            return rotateLeft(node);
        }

        

       
        return node;
    }

    
    Student* minValueNode(Student* node) {
        Student* current = node;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    
    Student* deleteNode(Student* root, int roll) {
        if (root == nullptr)
            return root;

        
        if (roll < root->rollNumber)
            root->left = deleteNode(root->left, roll);
        else if (roll > root->rollNumber)
            root->right = deleteNode(root->right, roll);
        else {
            // Node with only one child or no child
            if (!root->left && !root->right) {
                delete root;
                return nullptr;
            } else if (!root->left) {
                Student* temp = root->right;
                delete root;
                return temp;
            } else if (!root->right) {
                Student* temp = root->left;
                delete root;
                return temp;
            }

            // Node with two children: 
            Student* temp = minValueNode(root->right);
            root->rollNumber = temp->rollNumber;
            root->name = temp->name;
            root->right = deleteNode(root->right, temp->rollNumber);
        }

        // Update height
        root->height = 1 + max(height(root->left), height(root->right));

        
        int balance = getBalance(root);

        // Left Left Case
        if (balance > 1 && getBalance(root->left) >= 0)
            return rotateRight(root);

        

        // Right Right Case
        if (balance < -1 && getBalance(root->right) <= 0)
            return rotateLeft(root);

        
        return root;
    }

    
    Student* search(Student* node, int roll) const {
        if (node == nullptr || node->rollNumber == roll) {
            return node;
        }
        if (roll < node->rollNumber) {
            return search(node->left, roll);
        } else {
            return search(node->right, roll);
        }
    }

    
    bool update(Student* node, int roll, string newName) {
        Student* target = search(node, roll);
        if (target) {
            target->name = newName;
            return true;
        }
        return false;
    }

    
    void inorder(Student* node) const {
        if (node != nullptr) {
            inorder(node->left);
            cout << "Roll Number: " << node->rollNumber << ", Name: " << node->name << endl;
            inorder(node->right);
        }
    }

public:
    StudentDatabase() : root(nullptr) {}

    
    void addStudent(int roll, string name) {
        root = insert(root, roll, name);
    }

    
    void deleteStudent(int roll) {
        root = deleteNode(root, roll);
    }

    
    void searchStudent(int roll) const {
        Student* result = search(root, roll);
        if (result != nullptr) {
            cout << "Student found: Roll Number: " << result->rollNumber << ", Name: " << result->name << endl;
        } else {
            cout << "Student with Roll Number " << roll << " not found." << endl;
        }
    }

    
    void updateStudent(int roll, string newName) {
        if (update(root, roll, newName)) {
            cout << "Student's name updated successfully." << endl;
        } else {
            cout << "Student with Roll Number " << roll << " not found." << endl;
        }
    }

    
    void displayStudents() const {
        inorder(root);
    }
};

// Main function 
int main() {
    StudentDatabase db;
    int choice, roll;
    string name;

    while (true) {
        cout << "\nStudent Database Menu:\n";
        cout << "1. Add Student\n";
        cout << "2. Delete Student\n";
        cout << "3. Search Student\n";
        cout << "4. Update Student Name\n";
        cout << "5. Display Students\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter Roll Number: ";
            cin >> roll;
            cout << "Enter Name: ";
            cin.ignore(); // to ignore the newline character left in the buffer
            getline(cin, name);
            db.addStudent(roll, name);
            break;
        case 2:
            cout << "Enter Roll Number to delete: ";
            cin >> roll;
            db.deleteStudent(roll);
            break;
        case 3:
            cout << "Enter Roll Number to search: ";
            cin >> roll;
            db.searchStudent(roll);
            break;
        case 4:
            cout << "Enter Roll Number to update: ";
            cin >> roll;
            cout << "Enter New Name: ";
            cin.ignore(); // to ignore the newline character left in the buffer
            getline(cin, name);
            db.updateStudent(roll, name);
            break;
        case 5:
            db.displayStudents();
            break;
        case 6:
            cout << "Exiting..." << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

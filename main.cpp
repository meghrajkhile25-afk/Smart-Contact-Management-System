#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <cctype>

using namespace std;

struct Contact {
    int id;
    string name, phone, email, city;
    Contact* next;
    Contact(int i, const string& n, const string& p, const string& e, const string& c)
        : id(i), name(n), phone(p), email(e), city(c), next(nullptr) {}
};

class ContactList {
private:
    Contact* head;

    Contact* findById(int id) const {
        for (Contact* p = head; p; p = p->next)
            if (p->id == id) return p;
        return nullptr;
    }

    static string lower(string s) {
        for (char& ch : s) ch = static_cast<char>(tolower(static_cast<unsigned char>(ch)));
        return s;
    }

    static bool validPhone(const string& phone) {
        int digits = 0;
        for (char ch : phone) {
            if (isdigit(static_cast<unsigned char>(ch))) ++digits;
            else if (ch != '+' && ch != '-' && ch != ' ' && ch != '(' && ch != ')')
                return false;
        }
        return digits >= 7 && digits <= 15;
    }

    static string readLine(const string& prompt) {
        string value;
        cout << prompt;
        getline(cin, value);
        return value;
    }

    static int readInt(const string& prompt) {
        int value;
        while (true) {
            cout << prompt;
            if (cin >> value) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
            cout << "Invalid number. Please try again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    void appendLoaded(Contact* c) {
        if (!head) { head = c; return; }
        Contact* p = head;
        while (p->next) p = p->next;
        p->next = c;
    }

public:
    ContactList() : head(nullptr) {}
    ~ContactList() {
        while (head) {
            Contact* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void addContact() {
        int id = readInt("Contact ID: ");
        if (id <= 0 || findById(id)) {
            cout << "ID must be positive and unique.\n";
            return;
        }
        string name = readLine("Name: ");
        string phone = readLine("Phone: ");
        string email = readLine("Email: ");
        string city = readLine("City: ");
        if (name.empty() || !validPhone(phone)) {
            cout << "Name cannot be empty; phone must contain 7-15 digits.\n";
            return;
        }
        Contact* c = new Contact(id, name, phone, email, city);
        c->next = head;
        head = c;
        cout << "Contact added successfully.\n";
    }

    void displayContacts() const {
        if (!head) { cout << "No contacts available.\n"; return; }
        cout << "\n--- Contact List ---\n";
        for (Contact* p = head; p; p = p->next) {
            cout << "ID: " << p->id << "\nName: " << p->name
                 << "\nPhone: " << p->phone << "\nEmail: " << p->email
                 << "\nCity: " << p->city << "\n--------------------\n";
        }
    }

    void searchContact() const {
        string key = readLine("Enter ID, name, or phone to search: ");
        bool found = false;
        string keyLower = lower(key);
        for (Contact* p = head; p; p = p->next) {
            if (to_string(p->id) == key || lower(p->name).find(keyLower) != string::npos
                || p->phone.find(key) != string::npos) {
                cout << "Found: ID " << p->id << " | " << p->name << " | "
                     << p->phone << " | " << p->email << " | " << p->city << "\n";
                found = true;
            }
        }
        if (!found) cout << "No matching contact found.\n";
    }

    void updateContact() {
        int id = readInt("Enter ID to update: ");
        Contact* p = findById(id);
        if (!p) { cout << "Contact not found.\n"; return; }
        string name = readLine("New name [" + p->name + "]: ");
        string phone = readLine("New phone [" + p->phone + "]: ");
        string email = readLine("New email [" + p->email + "]: ");
        string city = readLine("New city [" + p->city + "]: ");
        if (!name.empty()) p->name = name;
        if (!phone.empty()) {
            if (!validPhone(phone)) { cout << "Invalid phone; update cancelled for phone.\n"; }
            else p->phone = phone;
        }
        if (!email.empty()) p->email = email;
        if (!city.empty()) p->city = city;
        cout << "Contact updated.\n";
    }

    void deleteContact() {
        int id = readInt("Enter ID to delete: ");
        Contact* current = head;
        Contact* previous = nullptr;
        while (current && current->id != id) {
            previous = current;
            current = current->next;
        }
        if (!current) { cout << "Contact not found.\n"; return; }
        if (previous) previous->next = current->next;
        else head = current->next;
        delete current;
        cout << "Contact deleted.\n";
    }

    void sortByName() {
        if (!head || !head->next) { cout << "Not enough contacts to sort.\n"; return; }
        // Insertion sort by swapping data fields; linked-list nodes remain connected.
        for (Contact* i = head->next; i; i = i->next) {
            Contact* j = i;
            while (j != head) {
                Contact* prev = head;
                while (prev->next != j) prev = prev->next;
                if (lower(prev->name) <= lower(j->name)) break;
                swap(prev->id, j->id);
                swap(prev->name, j->name);
                swap(prev->phone, j->phone);
                swap(prev->email, j->email);
                swap(prev->city, j->city);
                j = prev;
            }
        }
        cout << "Contacts sorted alphabetically by name.\n";
    }

    void saveToFile(const string& filename = "data/contacts.txt") const {
        ofstream file(filename);
        if (!file) { cout << "Could not open file for saving. Run from project folder.\n"; return; }
        for (Contact* p = head; p; p = p->next) {
            file << p->id << '\n' << p->name << '\n' << p->phone << '\n'
                 << p->email << '\n' << p->city << '\n';
        }
        cout << "Contacts saved to " << filename << ".\n";
    }

    void loadFromFile(const string& filename = "data/contacts.txt") {
        ifstream file(filename);
        if (!file) return;
        while (true) {
            int id;
            string name, phone, email, city;
            if (!(file >> id)) break;
            file.ignore(numeric_limits<streamsize>::max(), '\n');
            if (!getline(file, name) || !getline(file, phone) ||
                !getline(file, email) || !getline(file, city)) break;
            if (id > 0 && !findById(id)) appendLoaded(new Contact(id, name, phone, email, city));
        }
    }
};

int main() {
    ContactList contacts;
    contacts.loadFromFile();
    int choice;
    do {
        cout << "\n===== SMART CONTACT MANAGEMENT SYSTEM =====\n"
             << "1. Add Contact\n2. Display Contacts\n3. Search Contact\n"
             << "4. Update Contact\n5. Delete Contact\n6. Sort by Name\n"
             << "7. Save Contacts\n8. Exit\n";
        cout << "Choose option: ";
        if (!(cin >> choice)) {
            cout << "Invalid input. Enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        switch (choice) {
            case 1: contacts.addContact(); break;
            case 2: contacts.displayContacts(); break;
            case 3: contacts.searchContact(); break;
            case 4: contacts.updateContact(); break;
            case 5: contacts.deleteContact(); break;
            case 6: contacts.sortByName(); break;
            case 7: contacts.saveToFile(); break;
            case 8: contacts.saveToFile(); cout << "Goodbye!\n"; break;
            default: cout << "Please choose from 1 to 8.\n";
        }
    } while (choice != 8);
    return 0;
}

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <regex>
using namespace std;

int MYID;

struct User {
    int id;
    string username;
    string password;
    string name;
    string email;
    bool allowAnonymous;
};

struct Question {
    int questionID;
    int parentID;
    int senderID;
    int receiverID;
    bool isAnonymous;
    string statement;
    string answer;
};

struct FileContent {
    vector<string> records;
};

set<int> usedUserIDs;
set<int> usedQuestionIDs;

// ============================================================
//                  عام: قراءة وكتابة أي ملف
// ============================================================

FileContent ReadFile(string filename) {
    FileContent fc;
    ifstream fin(filename);
    if (fin.fail()) {
        cout << "Error: cannot open " << filename << "\n";
        return fc;
    }

    string line;
    while (getline(fin, line)) {
        fc.records.push_back(line);
    }
    fin.close();
    return fc;
}

void WriteFile(string filename, FileContent fc) {
    ofstream fout(filename);
    for (int i = 0; i < fc.records.size(); i++) {
        fout << fc.records[i] << "\n";
    }
    fout.close();
}

// ============================================================
//                       USER: Parse / ToString
// ============================================================

string UserToString(User u) {
    return to_string(u.id) + "|" + u.username + "|" + u.password + "|"
         + u.name + "|" + u.email + "|" + to_string(u.allowAnonymous);
}

User ParseUser(string line) {
    istringstream iss(line);
    string token;
    User u;

    for (int i = 1; i <= 6; i++) {
        getline(iss, token, '|');
        switch (i) {
        case 1:
            try { u.id = stoi(token); }
            catch (invalid_argument&) { u.id = 0; }
            break;
        case 2: u.username = token; break;
        case 3: u.password = token; break;
        case 4: u.name = token; break;
        case 5: u.email = token; break;
        case 6:
            try { u.allowAnonymous = stoi(token); }
            catch (invalid_argument&) { u.allowAnonymous = false; }
            break;
        }
    }
    return u;
}

// ============================================================
//                     QUESTION: Parse / ToString
// ============================================================

string QuestionToString(Question q) {
    return to_string(q.questionID) + "|" + to_string(q.parentID) + "|"
         + to_string(q.senderID) + "|" + to_string(q.receiverID) + "|"
         + to_string(q.isAnonymous) + "|" + q.statement + "|" + q.answer;
}

Question ParseQuestion(string line) {
    istringstream iss(line);
    string token;
    Question q;

    for (int i = 1; i <= 7; i++) {
        getline(iss, token, '|');
        switch (i) {
        case 1:
            try { q.questionID = stoi(token); }
            catch (invalid_argument&) { q.questionID = -1; }
            break;
        case 2:
            try { q.parentID = stoi(token); }
            catch (invalid_argument&) { q.parentID = -1; }
            break;
        case 3:
            try { q.senderID = stoi(token); }
            catch (invalid_argument&) { q.senderID = -1; }
            break;
        case 4:
            try { q.receiverID = stoi(token); }
            catch (invalid_argument&) { q.receiverID = -1; }
            break;
        case 5:
            try { q.isAnonymous = stoi(token); }
            catch (invalid_argument&) { q.isAnonymous = false; }
            break;
        case 6: q.statement = token; break;
        case 7: q.answer = token; break;
        }
    }
    return q;
}

// ============================================================
//                    ID GENERATION SYSTEM
// ============================================================

int GenerateRandomID(set<int>& usedIDs) {
    int newID;
    do {
        newID = rand() % 900 + 100;
    } while (usedIDs.count(newID) > 0);

    usedIDs.insert(newID);
    return newID;
}

void HydrateUserIDs() {
    FileContent fc = ReadFile("users.txt");
    for (int i = 0; i < fc.records.size(); i++) {
        User u = ParseUser(fc.records[i]);
        usedUserIDs.insert(u.id);
    }
}

void HydrateQuestionIDs() {
    FileContent fc = ReadFile("questions.txt");
    for (int i = 0; i < fc.records.size(); i++) {
        Question q = ParseQuestion(fc.records[i]);
        usedQuestionIDs.insert(q.questionID);
    }
}

// ============================================================
//                      HELPER: FindUserByID
// ============================================================

User FindUserByID(int id) {
    FileContent fc = ReadFile("users.txt");

    for (int i = 0; i < fc.records.size(); i++) {
        User u = ParseUser(fc.records[i]);
        if (u.id == id) {
            return u;
        }
    }

    User notFound;
    notFound.id = -1;
    return notFound;
}

// ============================================================
//                          AskQ()
// ============================================================

void AskQ() {
    int id;
    User IamUser;

    while (true) {
        cout << "Enter User ID: ";

        if (!(cin >> id)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a valid ID.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        IamUser = FindUserByID(id);
        if (IamUser.id != -1) {
            break;
        }

        cout << "User doesn't exist, try another one\n";
    }

    Question NewQ;
    NewQ.receiverID = id;
    NewQ.senderID = MYID;

    cout << "Enter Question id to thread, or -1 for new question: ";
    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == -1) {
        NewQ.parentID = -1;
    }
    else {
        NewQ.parentID = choice;
    }

    if (IamUser.allowAnonymous == 0) {
        cout << "Note: Anonymous questions are not allowed for this user\n";
        NewQ.isAnonymous = 0;
    }
    else {
        cout << "This user allows anonymous questions, do you want to send as anonymous? (0 - 1): ";
        int anonChoice;
        cin >> anonChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        NewQ.isAnonymous = anonChoice;
    }

    cout << "Enter Question statement: ";
    string state;
    getline(cin, state);
    NewQ.statement = state;
    NewQ.answer = "";

    NewQ.questionID = GenerateRandomID(usedQuestionIDs);

    FileContent fc = ReadFile("questions.txt");
    fc.records.push_back(QuestionToString(NewQ));
    WriteFile("questions.txt", fc);

    cout << "Question sent successfully! (ID: " << NewQ.questionID << ")\n";
}
// ============================================================
//  Sign-up / Login Users
// ============================================================

bool ContainsDash(string s) {
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '-') return true;
    }
    return false;
}

void Signup() {
    string Username, Password;
    cout << "Enter username (no dashes): ";
    cin >> Username;
    while (ContainsDash(Username)) {
        cout << "Username can't contain dashes, try again: ";
        cin >> Username;
    }

    cout << "Enter password: ";
    cin >> Password;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter your name: ";
    string Name;
    getline(cin, Name);

    cout << "Enter your email: ";
    string Email;
    cin >> Email;
    regex emailPattern("[^@\\s]+@[^@\\s]+\\.[^@\\s]+");
    while (!regex_match(Email, emailPattern)) {
        cout << "Invalid email format, try again: ";
        cin >> Email;
    }

    cout << "Allow anonymous questions? (0 - 1): ";
    int Anon;
    cin >> Anon;
    while (Anon != 0 && Anon != 1) {
        cout << "Invalid input, please enter 0 or 1: ";
        cin >> Anon;
    }

    int newID = GenerateRandomID(usedUserIDs);
    User newUser = {newID, Username, Password, Name, Email, (bool)Anon};

    FileContent fc = ReadFile("users.txt");
    fc.records.push_back(UserToString(newUser));
    WriteFile("users.txt", fc);

    cout << "Signup successful! Your user ID is: " << newID << "\n";
}

bool Login(){
    bool found = false;

    while(!found){
        cout << "Enter username and password (or -1 to cancel): ";
        string usern; cin >> usern;
        if (usern == "-1") {
            return false;   // المستخدم لغى، ارجع false فورًا
        }
        string passw; cin >> passw;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        FileContent fc = ReadFile("users.txt");
        for (int i = 0; i < fc.records.size(); i++) {
            User u = ParseUser(fc.records[i]);
            if (u.username == usern && u.password == passw) {
                MYID = u.id;
                found = true;
                break;
            }
        }
        if (!found) cout << "Your username/Password is wrong, try again..\n";
    }
    return true;
}

// ============================================================
//  َQuestions printing
// ============================================================

void PrintQtoMe() {
    FileContent fc = ReadFile("questions.txt");
    bool found = false;

    cout << "\n---------------- Questions To Me ----------------\n";

    for (int i = 0; i < fc.records.size(); i++) {
        Question q = ParseQuestion(fc.records[i]);

        if (q.receiverID == MYID && q.parentID == -1) {
            found = true;
            cout << "Question ID (" << q.questionID << ")";

            if (q.isAnonymous) {
                cout << " from Anonymous";
            } else {
                cout << " from user id(" << q.senderID << ")";
            }

            cout << "\n  Question: " << q.statement << "\n";

            if (q.answer == "") {
                cout << "  Answer: NOT Answered YET\n";
            } else {
                cout << "  Answer: " << q.answer << "\n";
            }

            for (int j = 0; j < fc.records.size(); j++) {
                Question subQ = ParseQuestion(fc.records[j]);
                if (subQ.parentID == q.questionID) {
                    cout << "    Thread: Question ID (" << subQ.questionID << ")";
                    if (subQ.isAnonymous) {
                        cout << " from Anonymous";
                    } else {
                        cout << " from user id(" << subQ.senderID << ")";
                    }
                    cout << "\n      Question: " << subQ.statement << "\n";

                    if (subQ.answer == "") {
                        cout << "      Answer: NOT Answered YET\n";
                    } else {
                        cout << "      Answer: " << subQ.answer << "\n";
                    }
                }
            }
            cout << "\n";
        }
    }

    if (!found) {
        cout << "No questions found for you.\n";
    }
    cout << "-------------------------------------------------\n";
}

void PrintQfromMe() {
    FileContent fc = ReadFile("questions.txt");
    bool found = false;

    cout << "\n---------------- Questions From Me ----------------\n";

    for (int i = 0; i < fc.records.size(); i++) {
        Question q = ParseQuestion(fc.records[i]);

        if (q.senderID == MYID) {
            found = true;
            cout << "Question ID (" << q.questionID << ")";

            if (q.isAnonymous) {
                cout << " !AQ";
            }

            cout << " to user id(" << q.receiverID << ")";
            cout << "\n  Question: " << q.statement << "\n";

            if (q.answer == "") {
                cout << "  Answer: NOT Answered YET\n";
            } else {
                cout << "  Answer: " << q.answer << "\n";
            }
            cout << "\n";
        }
    }

    if (!found) {
        cout << "You haven't asked any questions yet.\n";
    }
    cout << "---------------------------------------------------\n";
}

// ============================================================
//  Answers / Delete Questions
// ============================================================

void AnswerQ() {
    cout << "Enter Question id or -1 to cancel: ";
    int qID;
    cin >> qID;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (qID == -1) return;

    FileContent fc = ReadFile("questions.txt");
    bool found = false;

    for (int i = 0; i < fc.records.size(); i++) {
        Question q = ParseQuestion(fc.records[i]);

        if (q.questionID == qID) {
            found = true;

            if (q.receiverID != MYID) {
                cout << "Error: You can only answer questions sent to you!\n";
                return;
            }

            if (q.answer != "") {
                cout << "Warning: Question already answered. Answer will be updated.\n";
            }

            cout << "Enter answer: ";
            string newAns;
            getline(cin, newAns);

            q.answer = newAns;
            fc.records[i] = QuestionToString(q);

            WriteFile("questions.txt", fc);
            cout << "Answer saved successfully!\n";
            break;
        }
    }

    if (!found) {
        cout << "Question ID not found, try again.\n";
    }
}

void DeleteQ() {
    cout << "Enter Question id or -1 to cancel: ";
    int qID;
    cin >> qID;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (qID == -1) return;

    FileContent fc = ReadFile("questions.txt");
    bool found = false;
    bool canDelete = false;

    for (int i = 0; i < fc.records.size(); i++) {
        Question q = ParseQuestion(fc.records[i]);
        if (q.questionID == qID) {
            found = true;
            if (q.senderID == MYID || q.receiverID == MYID) {
                canDelete = true;
            }
            break;
        }
    }

    if (!found) {
        cout << "Question ID not found.\n";
        return;
    }
    if (!canDelete) {
        cout << "Error: You can only delete questions you sent or received!\n";
        return;
    }

    set<int> toDelete;
    toDelete.insert(qID);

    bool changed;
    do {
        changed = false;
        for (int i = 0; i < fc.records.size(); i++) {
            Question q = ParseQuestion(fc.records[i]);
            if (toDelete.count(q.parentID) > 0 && toDelete.count(q.questionID) == 0) {
                toDelete.insert(q.questionID);
                changed = true;
            }
        }
    } while (changed);

    FileContent updatedFC;
    for (int i = 0; i < fc.records.size(); i++) {
        Question q = ParseQuestion(fc.records[i]);
        if (toDelete.count(q.questionID) == 0) {
            updatedFC.records.push_back(fc.records[i]);
        }
    }

    WriteFile("questions.txt", updatedFC);
    cout << "Question and its full thread deleted successfully! (" << toDelete.size() << " question(s) removed)\n";
}

// ============================================================
//  List-user / Feedback
// ============================================================

void ListUser() {
    FileContent fc = ReadFile("users.txt");
    if (fc.records.empty()) {
        cout << "No users found.\n";
        return;
    }

    cout << "\n----------------------------------------\n";
    for (int i = 0; i < (int)fc.records.size(); i++) {
        User u = ParseUser(fc.records[i]);
        cout << "ID: " << u.id << "\t Name: " << u.name << "\n";
    }
    cout << "----------------------------------------\n";
}

void Feed() {
    FileContent fc = ReadFile("questions.txt");

    cout << "\n---------------- Feed ----------------\n";

    for (int i = 0; i < fc.records.size(); i++) {
        Question q = ParseQuestion(fc.records[i]);

        if (q.parentID == -1) {
            cout << "Question Id (" << q.questionID << ")";
        }
        else {
            cout << "Thread Parent Question ID (" << q.parentID << ") Question Id (" << q.questionID << ")";
        }

        if (q.isAnonymous) {
            cout << " from Anonymous";
        }
        else {
            cout << " from user id(" << q.senderID << ")";
        }

        cout << " To user id(" << q.receiverID << ")\t\tQuestion: " << q.statement << "\n";

        if (q.answer == "") {
            cout << "\tAnswer: NOT Answered YET\n";
        }
        else {
            cout << "\tAnswer: " << q.answer << "\n";
        }
    }

    cout << "----------------------------------------\n";
}


int main() {
    srand(time(0));
    HydrateUserIDs();
    HydrateQuestionIDs();


    bool loggedIn = false;
    while (!loggedIn) {
        cout << "Menu:\n  1: Login\n  2: Sign Up\nEnter number in range 1 - 2: ";
        int authChoice;
        cin >> authChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (authChoice == 1) {
            loggedIn = Login();
        }
        else if (authChoice == 2) {
            Signup();
        }
        else {
            cout << "Invalid choice, try again..\n";
        }
    }

    bool running = true;
    while (running) {
        cout << "\nMenu:\n"
             << "  1: Print Questions To Me\n"
             << "  2: Print Questions From Me\n"
             << "  3: Answer Question\n"
             << "  4: Delete Question\n"
             << "  5: Ask Question\n"
             << "  6: List System Users\n"
             << "  7: Feed\n"
             << "  8: Logout\n"
             << "Enter number in range 1 - 8: ";
        int op;
        cin >> op;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (op) {
        case 1: PrintQtoMe(); break;
        case 2: PrintQfromMe(); break;
        case 3: AnswerQ(); break;
        case 4: DeleteQ(); break;
        case 5: AskQ(); break;
        case 6: ListUser(); break;
        case 7: Feed(); break;
        case 8: running = false; break;
        default: cout << "Invalid choice, try again..\n"; break;
        }


    }

    return 0;
}

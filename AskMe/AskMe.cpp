#include "AskMe.h"

// Users File will be   : "users.txt"
// Questions File will be: "questions.txt"

using namespace std;

// =================================================
//                  HelpingTools
// =================================================

vector<string> HelpingTools::ReadFileLines(const string &path){
	vector<string> Content;
	ifstream fRead(path);

	if(fRead.fail()){
		cout << "The File can't be opened, Try again.\n";
		return Content;
	}
	string line;
	while (getline(fRead, line)) {
		if (!line.empty()) Content.push_back(line);
	}
	return Content;
}

void HelpingTools::WriteFileLines(const string &path, const vector<string> &lines, bool append){
	ofstream fWrite(path, append ? ios::app : ios::out);
	if (fWrite.fail()) {
		cout << "The File can't be opened, Try again.\n";
		return;
	}
	for (size_t i = 0; i < lines.size(); i++) {
		fWrite << lines[i] << "\n";
	}
}

vector<string> HelpingTools::SplitString(const string &line, const string &sperator){
	vector<string> Content;
	stringstream ss(line); string val;

	while(getline(ss, val, sperator[0])){
		Content.push_back(val);
	}

	return Content;
}

int HelpingTools::ToInt(const string &str){
	int num = 0;
	bool negative = false;
	size_t i = 0;

	if (!str.empty() && str[0] == '-') { negative = true; i = 1; }

	for (; i < str.size(); i++){
		if (str[i] < '0' || str[i] > '9') break;
		num = num * 10 + (str[i] - '0');
	}

	return negative ? -num : num;
}

int HelpingTools::StoI(const string &str){
	return ToInt(str);
}

string HelpingTools::ItoS(int val){
	return to_string(val);
}

int HelpingTools::ReadInt(int low, int high) {
	int val;
	cout << "Enter number in range between (" << low << ") and (" << high << "): ";
	cin >> val;

	while (cin.fail() || val < low || val > high) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid input! Please enter a number between (" << low << ") and (" << high << "): ";
		cin >> val;
	}
	return val;
}

int HelpingTools::Menu(const vector<string> &choice){
	cout << "\nMenu:\n";
	for (int ch = 0; ch < (int) choice.size(); ++ch) {
		cout << "\t" << ch + 1 << ": " << choice[ch] << "\n";
	}
	return ReadInt(1, choice.size());
}

// =================================================
//                  Question
// =================================================

Question::Question() {
	question_id = -1;
	parent_question_id = -1;
	from_user_id = -1;
	to_user_id = -1;
	is_anonymous_questions = 0;
	question_text = "";
	answer_text = "";
}

Question::Question(const string &line) {
	vector<string> fields = HelpingTools::SplitString(line);
	if (fields.size() >= 7) {
		question_id = HelpingTools::ToInt(fields[0]);
		parent_question_id = HelpingTools::ToInt(fields[1]);
		from_user_id = HelpingTools::ToInt(fields[2]);
		to_user_id = HelpingTools::ToInt(fields[3]);
		is_anonymous_questions = HelpingTools::ToInt(fields[4]);
		question_text = fields[5];
		answer_text = fields[6];
	} else {
		question_id = -1;
		parent_question_id = -1;
		from_user_id = -1;
		to_user_id = -1;
		is_anonymous_questions = 0;
	}
}

int Question::GetQuestionId() const { return question_id; }
void Question::SetQuestionId(int id) { question_id = id; }

int Question::GetParentQuestionId() const { return parent_question_id; }
void Question::SetParentQuestionId(int id) { parent_question_id = id; }

int Question::GetFromUserId() const { return from_user_id; }
void Question::SetFromUserId(int id) { from_user_id = id; }

int Question::GetToUserId() const { return to_user_id; }
void Question::SetToUserId(int id) { to_user_id = id; }

int Question::GetIsAnonymousQ() const { return is_anonymous_questions; }
void Question::SetIsAnonymousQ(int is_anon) { is_anonymous_questions = is_anon; }

string Question::GetQuestionText() const { return question_text; }
void Question::SetQuestionText(const string &text) { question_text = text; }

string Question::GetAnswerText() const { return answer_text; }
void Question::SetAnswerText(const string &text) { answer_text = text; }

string Question::ToString() const {
	return to_string(question_id) + "|"
		 + to_string(parent_question_id) + "|"
		 + to_string(from_user_id) + "|"
		 + to_string(to_user_id) + "|"
		 + to_string(is_anonymous_questions) + "|"
		 + question_text + "|"
		 + answer_text;
}

void Question::PrintToQuestion() const {
	if (parent_question_id != -1) {
		cout << "\tThread Parent Question ID (" << parent_question_id << ") ";
	}

	cout << "Question ID (" << question_id << ")";

	if (!is_anonymous_questions) {
		cout << " From User ID (" << from_user_id << ")";
	}

	cout << "\n\tQuestion: " << question_text << "\n";

	if (answer_text != "") {
		cout << "\tAnswer: " << answer_text << "\n";
	}
}

void Question::PrintFromQuestion() const {
	cout << "Question ID (" << question_id << ")";

	if (is_anonymous_questions) {
		cout << " [AQ]";
	}

	cout << " To User ID (" << to_user_id << ")";
	cout << "\n\tQuestion: " << question_text << "\n";

	if (answer_text != "") {
		cout << "\tAnswer: " << answer_text << "\n";
	}
}

void Question::PrintFeedQuestion() const {
	if (parent_question_id != -1) {
		cout << "\tThread Parent Question ID (" << parent_question_id << ") ";
	}

	cout << "Question ID (" << question_id << ")";

	if (!is_anonymous_questions) {
		cout << " From User ID (" << from_user_id << ")";
	}

	cout << " To User ID (" << to_user_id << ")";
	cout << "\n\tQuestion: " << question_text << "\n";

	if (answer_text != "") {
		cout << "\tAnswer: " << answer_text << "\n";
	}
}

// =================================================
//                  User
// =================================================

User::User() {
	user_id = -1;
	allow_anonymous_questions = 0;
}

User::User(const string &line) {
	vector<string> substrs = HelpingTools::SplitString(line, "|");
	if (substrs.size() < 6) return;

	user_id = HelpingTools::StoI(substrs[0]);
	username = substrs[1];
	password = substrs[2];
	name = substrs[3];
	email = substrs[4];
	allow_anonymous_questions = HelpingTools::StoI(substrs[5]);
}

int User::GetUserId() const { return user_id; }
void User::SetUserId(int id) { user_id = id; }

string User::GetUsername() const { return username; }
void User::SetUsername(const string &uname) { username = uname; }

string User::GetPassword() const { return password; }
bool User::IsMatchingPassword(const string &pass) const { return password == pass; }
void User::SetPassword(const string &pass) { password = pass; }

string User::GetName() const { return name; }
void User::SetName(const string &n) { name = n; }

string User::GetEmail() const { return email; }
void User::SetEmail(const string &m) { email = m; }

int User::GetAllowAnonymousQuestions() const { return allow_anonymous_questions; }
void User::SetAllowAnonymousQuestions(int allow) { allow_anonymous_questions = allow; }

string User::ToString() const {
	string line;

	line = HelpingTools::ItoS(user_id) + "|"
		 + username + "|"
		 + password + "|"
		 + name + "|"
		 + email + "|"
		 + HelpingTools::ItoS(allow_anonymous_questions);

	return line;
}

void User::Print() const {
	cout << "User ID: " << user_id
		 << " | Name: " << name
		 << " | Email: " << email
		 << " | Allow Anonymous: " << (allow_anonymous_questions ? "Yes" : "No")
		 << "\n";
}

void User::AddQuestionTo(const Question &q) {
	questions_to_me.push_back(q);
}

void User::AddQuestionFrom(const Question &q) {
	questions_from_me.push_back(q);
}

void User::ClearQuestions() {
	questions_to_me.clear();
	questions_from_me.clear();
}

// =================================================
//                  QuestionsManager
// =================================================

QuestionsManager::QuestionsManager() {
	last_id = 0;
}

void QuestionsManager::LoadDatabase() {
	questionid_questionObject_map.clear();
	questionid_questionThreadsid_map.clear();
	last_id = 0;

	vector<string> lines = HelpingTools::ReadFileLines("questions.txt");

	for (const string &line : lines) {
		if (line.empty()) continue;

		Question Q(line);
		if (Q.GetQuestionId() == -1) continue;

		last_id = max(last_id, Q.GetQuestionId());

		questionid_questionObject_map[Q.GetQuestionId()] = Q;

		if (Q.GetParentQuestionId() != -1)
			questionid_questionThreadsid_map[Q.GetParentQuestionId()].push_back(Q.GetQuestionId());
	}
}

void QuestionsManager::FillUserQuestions(User &user){
	user.ClearQuestions();

	for (const auto &pair : questionid_questionObject_map){
		const Question &Q = pair.second;

		if (Q.GetToUserId() == user.GetUserId()) {
			if (Q.GetParentQuestionId() == -1) {
				user.AddQuestionTo(Q);
			}
		}

		if (Q.GetFromUserId() == user.GetUserId())
			user.AddQuestionFrom(Q);
	}
}

void QuestionsManager::PrintUserToQuestions(const User &user) const {
	cout << "\n";
	bool found = false;

	for (const auto &pair : questionid_questionObject_map) {
		const Question &q = pair.second;

		if (q.GetToUserId() == user.GetUserId() && q.GetParentQuestionId() == -1) {
			found = true;
			q.PrintToQuestion();

			auto thread_it = questionid_questionThreadsid_map.find(q.GetQuestionId());
			if (thread_it != questionid_questionThreadsid_map.end()) {
				for (int thread_id : thread_it->second) {
					auto q_it = questionid_questionObject_map.find(thread_id);
					if (q_it != questionid_questionObject_map.end()) {
						cout << "\tThread: ";
						q_it->second.PrintToQuestion();
					}
				}
			}
			cout << "\n";
		}
	}

	if (!found) {
		cout << "No questions for this user.\n";
	}
}

void QuestionsManager::PrintUserFromQuestions(const User &user) const {
	cout << "\n";
	bool found = false;

	for (const auto &pair : questionid_questionObject_map) {
		const Question &q = pair.second;

		if (q.GetFromUserId() == user.GetUserId()) {
			found = true;
			q.PrintFromQuestion();
		}
	}

	if (!found) {
		cout << "No questions sent by this user.\n";
	}
}

int QuestionsManager::ReadQuestionIdAny(const User &user) const {
	int id;

	while (true) {
		cout << "Enter Question id or -1 to cancel: ";
		cin >> id;

		if (id == -1)
			return -1;

		auto it = questionid_questionObject_map.find(id);

		if (it != questionid_questionObject_map.end()) {
			const Question &q = it->second;

			if (q.GetToUserId() == user.GetUserId()) {
				return id;
			} else {
				cout << "ERROR: Question is not directed to you. Try again.\n";
			}
		} else {
			cout << "ERROR: No question with such ID. Try again.\n";
		}
	}
}

int QuestionsManager::ReadQuestionIdThread(const User &user) const {
	int question_id;

	while (true) {
		cout << "Enter Question id or -1 to cancel: ";
		cin >> question_id;

		if (question_id == -1)
			return -1;

		auto q_it = questionid_questionObject_map.find(question_id);

		if (q_it == questionid_questionObject_map.end()) {
			cout << "ERROR: No question with such ID. Try again.\n";
			continue;
		}

		const Question &q = q_it->second;

		if (q.GetParentQuestionId() != -1) {
			cout << "ERROR: This question is already a thread. You can only reply to main questions. Try again.\n";
			continue;
		}

		return question_id;
	}
}

void QuestionsManager::AnswerQuestion(const User &user) {
	int question_id = ReadQuestionIdAny(user);

	if (question_id == -1)
		return;

	Question &q = questionid_questionObject_map[question_id];

	if (!q.GetAnswerText().empty()) {
		cout << "Warning: Question is already answered. It will be updated.\n";
	}

	cout << "Enter answer: ";
	string answer;
	cin.ignore();
	getline(cin, answer);

	q.SetAnswerText(answer);

	UpdateDatabase();
}

void QuestionsManager::DeleteQuestion(const User &user) {
	int question_id = ReadQuestionIdAny(user);

	if (question_id == -1)
		return;

	vector<int> ids_to_remove;

	auto thread_it = questionid_questionThreadsid_map.find(question_id);
	if (thread_it != questionid_questionThreadsid_map.end()) {
		ids_to_remove = thread_it->second;
		questionid_questionThreadsid_map.erase(thread_it);
	} else {
		int parent_id = questionid_questionObject_map[question_id].GetParentQuestionId();
		if (parent_id != -1) {
			auto &threads = questionid_questionThreadsid_map[parent_id];
			threads.erase(remove(threads.begin(), threads.end(), question_id), threads.end());
		}
	}

	ids_to_remove.push_back(question_id);

	for (int id : ids_to_remove) {
		questionid_questionObject_map.erase(id);
	}

	UpdateDatabase();
}

void QuestionsManager::AskQuestion(const User &user, pair<int, int> to_user_pair) {
	int to_user_id = to_user_pair.first;
	int allow_anonymous = to_user_pair.second;

	int parent_id = -1;

	cout << "For thread question enter 1, for new question enter 2: ";
	int choice = HelpingTools::ReadInt(1, 2);

	if (choice == 1) {
		parent_id = ReadQuestionIdThread(user);
		if (parent_id == -1)
			return;
	}

	int is_anonymous = 0;
	if (allow_anonymous) {
		cout << "Send anonymous question? (1 or 0): ";
		is_anonymous = HelpingTools::ReadInt(0, 1);
	}

	cout << "Enter question text: ";
	string text;
	cin.ignore();
	getline(cin, text);

	Question q;
	q.SetQuestionId(++last_id);
	q.SetParentQuestionId(parent_id);
	q.SetFromUserId(user.GetUserId());
	q.SetToUserId(to_user_id);
	q.SetIsAnonymousQ(is_anonymous);
	q.SetQuestionText(text);

	questionid_questionObject_map[q.GetQuestionId()] = q;

	if (parent_id != -1) {
		questionid_questionThreadsid_map[parent_id].push_back(q.GetQuestionId());
	}

	UpdateDatabase();
}

void QuestionsManager::ListFeed() const {
	cout << "\n";
	bool empty = true;

	for (const auto &pair : questionid_questionObject_map) {
		const Question &q = pair.second;

		if (!q.GetAnswerText().empty()) {
			empty = false;
			q.PrintFeedQuestion();
		}
	}

	if (empty) {
		cout << "No answered questions in the feed yet.\n";
	}
}

void QuestionsManager::UpdateDatabase() const {
	vector<string> lines;

	for (const auto &pair : questionid_questionObject_map) {
		const Question &q = pair.second;
		lines.push_back(q.ToString());
	}

	HelpingTools::WriteFileLines("questions.txt", lines, false);
}

// =================================================
//                  UsersManager
// =================================================

UsersManager::UsersManager() {
	last_id = 0;
}

void UsersManager::LoadDatabase() {
	user_map.clear();
	username_userObject_map.clear();
	id_userobject_map.clear();
	last_id = 0;

	vector<string> lines = HelpingTools::ReadFileLines("users.txt");

	for (const string &line : lines) {
		if (line.empty())
			continue;

		User user(line);
		user_map[user.GetUserId()] = user;
		username_userObject_map[user.GetUsername()] = user;
		id_userobject_map[user.GetUserId()] = user;

		last_id = max(last_id, user.GetUserId());
	}
}

void UsersManager::UpdateDatabase() const {
	vector<string> lines;

	for (const auto &pair : user_map) {
		lines.push_back(pair.second.ToString());
	}

	HelpingTools::WriteFileLines("users.txt", lines, false);
}

bool UsersManager::Access() {
	while (true) {
		cout << "\n===== Welcome =====\n";
		cout << "\t1: Login\n";
		cout << "\t2: Sign Up\n";
		cout << "\t3: Exit Program\n";

		int choice = HelpingTools::ReadInt(1, 3);

		if (choice == 1) {
			if (Login()) {
				cout << "\nLogin successful. Welcome, " << current_user.GetName() << "!\n";
				return true;
			}
			// Wrong credentials: message already printed inside Login(). Loop back to menu.
		} else if (choice == 2) {
			SignUp();
			cout << "\nAccount created successfully. You are now logged in.\n";
			return true;
		} else if (choice == 3) {
			cout << "\nGoodbye!\n";
			return false;
		}
	}
}

bool UsersManager::Login() {
	string user_name, password;

	cout << "Enter user name & password: ";
	cin >> user_name >> password;

	auto it = username_userObject_map.find(user_name);

	if (it == username_userObject_map.end() || it->second.GetPassword() != password) {
		cout << "\nInvalid user name or password. Try again\n\n";
		return false;
	}

	current_user = it->second;
	return true;
}

void UsersManager::SignUp() {
	string user_name, password, name, email;
	int allow_anonymous_questions;

	while (true) {
		cout << "Enter user name. (No spaces): ";
		cin >> user_name;

		if (username_userObject_map.find(user_name) != username_userObject_map.end()) {
			cout << "Already used. Try again\n";
		} else {
			break;
		}
	}

	cout << "Enter password: ";
	cin >> password;

	cout << "Enter name: ";
	cin >> name;

	cout << "Enter email: ";
	cin >> email;

	cout << "Allow anonymous questions? (1 or 0): ";
	allow_anonymous_questions = HelpingTools::ReadInt(0, 1);

	User user;
	user.SetUserId(++last_id);
	user.SetUsername(user_name);
	user.SetPassword(password);
	user.SetName(name);
	user.SetEmail(email);
	user.SetAllowAnonymousQuestions(allow_anonymous_questions);

	current_user = user;
	user_map[user.GetUserId()] = user;
	username_userObject_map[user.GetUsername()] = user;
	id_userobject_map[user.GetUserId()] = user;

	UpdateDatabase();
}

void UsersManager::ListUsers() const {
	cout << "\n";
	for (const auto &pair : user_map) {
		cout << "ID: " << pair.second.GetUserId() << "\tName: " << pair.second.GetName() << "\n";
	}
}

pair<int, int> UsersManager::GetToUserPair() const {
	int to_user_id = -1;
	while (true) {
		cout << "Enter User id or -1 to cancel: ";
		cin >> to_user_id;
		if (to_user_id == -1) return make_pair(-1, -1);

		auto it = id_userobject_map.find(to_user_id);
		if (it == id_userobject_map.end()) {
			cout << "ERROR: Invalid user ID. Try again\n";
		} else {
			return make_pair(to_user_id, it->second.GetAllowAnonymousQuestions());
		}
	}
}

User& UsersManager::GetCurrentUser() {
	return current_user;
}

const User& UsersManager::GetCurrentUser() const {
	return current_user;
}

// =================================================
//                  UISystem
// =================================================

void UISystem::LoadDatabase(bool fill_user_questions) {
	UserMng.LoadDatabase();
	QMng.LoadDatabase();
}

void UISystem::Run() {
	LoadDatabase();

	while (true) {
		bool logged_in = UserMng.Access();

		if (!logged_in) {
			// User chose "Exit Program" from the login/sign-up menu.
			return;
		}

		bool logged_out = false;
		while (!logged_out) {
			cout << "\n===== Main Menu =====\n";
			cout << "\t1: View Questions Sent To Me\n";
			cout << "\t2: View Questions I Sent\n";
			cout << "\t3: Answer a Question\n";
			cout << "\t4: Delete a Question\n";
			cout << "\t5: Ask a Question\n";
			cout << "\t6: List System Users\n";
			cout << "\t7: View Answered Questions Feed\n";
			cout << "\t8: Logout\n";

			int choice = HelpingTools::ReadInt(1, 8);

			const User &current_user = UserMng.GetCurrentUser();

			if (choice == 1) {
				QMng.PrintUserToQuestions(current_user);
			} else if (choice == 2) {
				QMng.PrintUserFromQuestions(current_user);
			} else if (choice == 3) {
				QMng.AnswerQuestion(current_user);
			} else if (choice == 4) {
				QMng.DeleteQuestion(current_user);
			} else if (choice == 5) {
				pair<int, int> to_user_pair = UserMng.GetToUserPair();
				if (to_user_pair.first != -1) {
					QMng.AskQuestion(current_user, to_user_pair);
				}
			} else if (choice == 6) {
				UserMng.ListUsers();
			} else if (choice == 7) {
				QMng.ListFeed();
			} else if (choice == 8) {
				cout << "\nLogging out...\n";
				logged_out = true;
			}
		}
	}
}

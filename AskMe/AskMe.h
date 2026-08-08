#ifndef USERS_MANAGER_H_
#define USERS_MANAGER_H_

#include<cmath>
#include<string>
#include<iostream>
#include<algorithm>
#include<fstream>
#include<sstream>
#include<map>
#include<vector>
#include<utility>
#include<set>

using namespace std;

// =================================================
//                  Helping Functions
// =================================================

class HelpingTools{
public:
	static vector<string> ReadFileLines(const string &path);
	static void WriteFileLines(const string &path, const vector<string> &line, bool append = true);
	static vector<string> SplitString(const string &line, const string &sperator = "|");
	static int ToInt(const string &str);      // parses a string to int
	static int StoI(const string &str);       // alias used by User parsing
	static string ItoS(int val);              // int -> string
	static int ReadInt(int low, int high);
	static int Menu(const vector<string> &choice); // Login, Sign-up, List-users, Exit.
};

// =================================================
//                  Main Classes
// =================================================

class Question{
private:
	int question_id;
	int parent_question_id;
	int from_user_id;
	int to_user_id;
	int is_anonymous_questions;
	string question_text;
	string answer_text;

public:
	Question();
	Question(const string &line);

	int GetQuestionId() const;
	void SetQuestionId(int id);

	int GetParentQuestionId() const;
	void SetParentQuestionId(int id);

	int GetFromUserId() const;
	void SetFromUserId(int id);

	int GetToUserId() const;
	void SetToUserId(int id);

	int GetIsAnonymousQ() const;
	void SetIsAnonymousQ(int is_anon);

	string GetQuestionText() const;
	void SetQuestionText(const string &text);

	string GetAnswerText() const;
	void SetAnswerText(const string &text);

	string ToString() const;
	void PrintToQuestion() const;
	void PrintFromQuestion() const;
	void PrintFeedQuestion() const;
};

class User {
private:
	int user_id;
	string username;
	string password;
	string name;
	string email;
	int allow_anonymous_questions;

	vector<Question> questions_to_me;
	vector<Question> questions_from_me;

public:
	User();
	User(const string &line);

	int GetUserId() const;
	void SetUserId(int id);

	string GetUsername() const;
	void SetUsername(const string &uname);

	string GetPassword() const;
	bool IsMatchingPassword(const string &pass) const;
	void SetPassword(const string &pass);

	string GetName() const;
	void SetName(const string &n);

	string GetEmail() const;
	void SetEmail(const string &m);

	int GetAllowAnonymousQuestions() const;
	void SetAllowAnonymousQuestions(int allow);

	string ToString() const;
	void Print() const;

	void AddQuestionTo(const Question &q);
	void AddQuestionFrom(const Question &q);
	void ClearQuestions();
};

class QuestionsManager{
private:
	map<int, vector<int>> questionid_questionThreadsid_map;
	map<int, Question> questionid_questionObject_map;
	int last_id;

public:
	QuestionsManager();

	void LoadDatabase();
	void FillUserQuestions(User &user);
	void PrintUserToQuestions(const User &user) const;
	void PrintUserFromQuestions(const User &user) const;
	int ReadQuestionIdAny(const User &user) const;
	int ReadQuestionIdThread(const User &user) const;
	void AnswerQuestion(const User &user);
	void DeleteQuestion(const User &user);
	void AskQuestion(const User &user, pair<int, int> to_user_pair);
	void ListFeed() const;
	void UpdateDatabase() const;
};

class UsersManager {
private:
	User current_user;
	map<int, User> user_map;
	map<string, User> username_userObject_map;
	map<int, User> id_userobject_map;
	int last_id;

public:
	UsersManager();

	void LoadDatabase();
	bool Access();                 // Menu ---> Login / Sign-up / Exit. Returns false if user chose Exit.
	bool Login();
	void SignUp();
	void ListUsers() const;
	pair<int, int> GetToUserPair() const; // 1- user id, 2- allow_anonymous
	void UpdateDatabase() const;

	User& GetCurrentUser();             // Reading & Editing
	const User& GetCurrentUser() const; // Reading Only
};

class UISystem {
private:
	UsersManager UserMng;
	QuestionsManager QMng;

public:
	void LoadDatabase(bool fill_user_questions = false);
	void Run();
};

#endif

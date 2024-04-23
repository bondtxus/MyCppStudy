#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

using namespace std;

class User {
public:
  int id;
  string name;
  string email;
};

void addUser(vector<shared_ptr<User>>& userlist, const string& name, const string& email) {
    auto user = make_shared<User>();
    int id = userlist.empty() ? 1 : userlist.back()->id + 1;
    user->id = id;
    user->name = name;
    user->email = email;

    userlist.push_back(user);
}

void deleteUser(vector<shared_ptr<User>>& userlist, int id) {
     auto it = find_if(userlist.begin(), userlist.end(), [id](const shared_ptr<User>& user) {
        return user->id == id;
    });

    if (it != userlist.end()) {
        userlist.erase(it);
    } else {
        cerr << "Error: User with ID " << id << " not found." << endl;
    }
}

void updateUser(vector<shared_ptr<User>>& userlist, int id, string newName, string newEmail) {
  auto it = find_if(userlist.begin(), userlist.end(), [id](const shared_ptr<User>& user) {
    return user->id == id;
  });

  if (it != userlist.end()) {
    (*it)->name = newName;
    (*it)->email = newEmail;
  } else {
    cerr << "Error: User with ID " << id << " not found." << endl;
  }
}

// Comparator for sorting users by name
bool compareByName(const shared_ptr<User>& user1, const shared_ptr<User>& user2) {
  return user1->name < user2->name;
}

void listUsers(vector<shared_ptr<User>>& userlist) {
  vector<shared_ptr<User>> sortedUsers = userlist;
  sort(sortedUsers.begin(), sortedUsers.end(), compareByName);

  for (const auto& user : sortedUsers) {
    cout << "ID: " << user->id << ", Name: " << user->name << ", Email: " << user->email << endl;
  }
}

// Function to read user list from file
vector<shared_ptr<User>> readUserList(const string& filename) {
    ifstream file(filename);
    vector<shared_ptr<User>> users;

    if (!file) {
        cerr << "Error: Unable to open file " << filename << endl;
        return users;
    }

    int id;
    string name, email;
    while (file >> id >> name >> email) {
        auto user = make_shared<User>();
        user->id = id;
        user->name = name;
        user->email = email;
        users.push_back(user);
    }

    return users;
}

// Function to write user list to file
void writeUserList(const string& filename, const vector<shared_ptr<User>>& users) {
    ofstream file(filename);

    for (const auto& user : users) {
        file << user->id << " " << user->name << " " << user->email << endl;
    }
}

int main(int argc, char* argv[]) {

  // for debug
  cout << "<<<<<<<<<< For Debug <<<<<<<<<<" << endl;
  cout << "argc " << argc << endl;
  for (int i = 0; i < argc; i++){
    cout << "argv[" << i << "] = " << argv[i] << endl;
  }

  if (argc < 2) {
    cerr << "Usage: userlist -a/-d/-u/-l" << endl;
    return 1;
  }
  cout << ">>>>>>>>>> For Debug >>>>>>>>>>" << endl;

  string filename = "userlist.txt";
  vector<shared_ptr<User>> userlist = readUserList(filename);

  string option = argv[1];
  if (option == "-a" && argc == 4) {
      addUser(userlist, argv[2], argv[3]);
  } else if (option == "-d" && argc == 3) {
      int id = stoi(argv[2]);
      deleteUser(userlist, id);
  } else if (option == "-u" && argc == 5) {
      int id = stoi(argv[2]);
      updateUser(userlist, id, argv[3], argv[4]);
  } else if (option == "-l" && argc == 2) {
      listUsers(userlist);
  } else {
      cerr << "Invalid option or arguments." << endl;
    return 1;
  }
  
  writeUserList(filename, userlist);
  return 0;
}

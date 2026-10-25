// CS300 Jin Han Jun Project Two Module 7

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <cctype>

using namespace std;

struct Course {
	string courseNumber;
	string courseTitle;
	vector<string> prerequisites;
};

struct Node {
	Course course;
	Node* left;
	Node* right;

	Node(Course nCourse) {
		course = nCourse;
		left = nullptr;
		right = nullptr;
	}
};

class BinarySearchTree {
private:
	Node* root;
	
	Node* insertNode(Node* node, Course course) {
		if (node == nullptr) {
			return new Node(course);
		}
		
		if (course.courseNumber < node->course.courseNumber) {
			node->left = insertNode(node->left, course);
		}
		else if (course.courseNumber > node->course.courseNumber) {
			node->right = insertNode(node->right, course);
		}
		
		return node;
	}

	Node* searchNode(Node* node, string courseNumber) {
		if (node == nullptr) {
			return nullptr;
		}

		if (courseNumber == node->course.courseNumber) {
			return node;
		}

		if (courseNumber < node->course.courseNumber) {
			return searchNode(node->left, courseNumber);
		}

		else {
			return searchNode(node->right, courseNumber);
		}
	}
	
	void inOrder(Node* node) {
		if (node == nullptr) {
			return;
		}

		inOrder(node->left);

		cout << node->course.courseNumber << ", "
			<< node->course.courseTitle << endl;

		inOrder(node->right);
	}

	void deleteNodes(Node* node) {
		if (node == nullptr) {
			return;
		}

		deleteNodes(node->left);
		deleteNodes(node->right);
		delete node;
	}

public:
	BinarySearchTree() {
		root = nullptr;
	}

	~BinarySearchTree() {
		deleteNodes(root);
	}

	void Insert(Course course) {
		root = insertNode(root, course);
	}

	Course* Search(string courseNumber) {
		Node* node = searchNode(root, courseNumber);

		if (node == nullptr) {
			return nullptr;
		}

		return &(node->course);
	}

	void PrintAll() {
		inOrder(root);
	}
};

void loadCourses(string filename, BinarySearchTree& bst) {
	ifstream file(filename);

	if (!file.is_open()) {
		cout << "Error occurred while opening the file." << endl;
		return;
	}

	string line;

	while (getline(file, line)) {
		stringstream ss(line);
		Course course;
		string prerequisite;

		getline(ss, course.courseNumber, ',');
		getline(ss, course.courseTitle, ',');

		while (getline(ss, prerequisite, ',')) {
			if (!prerequisite.empty()) {
				course.prerequisites.push_back(prerequisite);
			}
		}

		bst.Insert(course);
	}

	file.close();
	cout << "Data loaded!" << endl;
}

void printCourse(BinarySearchTree& bst, string courseNumber) {
	Course* course = bst.Search(courseNumber);

	if (course == nullptr) {
		cout << "Course not found." << endl;
		return;
	}

	cout << course->courseNumber << "," << course->courseTitle << endl;

	cout << "Prerequisites: ";

	if (course->prerequisites.empty()) {
		cout << "None";
	}

	else {
		for (int i = 0; i < course->prerequisites.size(); i++) {
			Course* prerequisite = bst.Search(course->prerequisites[i]);

			if (prerequisite != nullptr) {
				cout << prerequisite->courseNumber
					<< "," << prerequisite->courseTitle;
			}

			else {
				cout << course->prerequisites[i];
			}

			if (i < course->prerequisites.size() - 1) {
				cout << "; ";
			}
		}
	}

	cout << endl;
}

int main() {
	BinarySearchTree bst;
	int choice = 0;
	string filename;
	string courseNumber;

	cout << "Welcome to the course planner!" << endl;

	while (choice != 9) {
		cout << endl;
		cout << "1. Load Data Structure" << endl;
		cout << "2. Print Course List" << endl;
		cout << "3. Print Course" << endl;
		cout << "9. Exit" << endl;
		
		cout << endl << "What would you like to do? ";
		cin >> choice;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Choose a valid option." << endl;
			continue;
		}

		switch (choice) {
		
		case 1: 
			cout << "Enter file name: ";
			getline(cin >> ws, filename);
			loadCourses(filename, bst);
			break;

		case 2:
			cout << endl << "Here is the course list." << endl;
			bst.PrintAll();
			break;

		case 3:
			cout << "What course would you like to view." << endl;
			cin >> courseNumber;

			for (char& c : courseNumber) {
				c = toupper(c);
			}

			printCourse(bst, courseNumber);
			break;

		case 9:
			break;

		default:
			cout << "Choose a valid option." << endl;
			break;
		}
	}

	cout << "Thanks for using the course planner." << endl;

	return 0;
}
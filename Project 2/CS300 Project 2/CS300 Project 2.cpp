
#include <iostream>
#include <fstream>
#include "CSVparser.hpp"
#include <vector>

using namespace std;

// Global definitions

/*
 Each course has an Id, Name, and a Vector 
 */
struct Course {
	string courseNum;
	string courseName;
	std::vector<string> preReqs;
	Course() {}
};
/*
 Each node in Binary Search Tree contains the course info and left and right
children
 */
struct Node {
	Course course;
	Node* left;
	Node* right;
	Node() {
		left = nullptr;
		right = nullptr;
	}
	Node(Course aCourse) : Node() {
		this->course = aCourse;
	}
};

// CourseBST Class Definition


	
	class CourseBST {
	private:
		Node* root;
		void addNode(Node* node, Course course);
		void printSampleSchedule(Node* node);
		void printCourseInformation(Node* node, string courseNum);
	public:
		CourseBST();
		virtual ~CourseBST();
		void DeleteRecursive(Node* node);
		void Insert(Course course);
		int NumPrerequisiteCourses(Course course);
		void PrintSampleSchedule();
		void PrintCourseInformation(string courseNum);
};
/*
Construct the Binary Search Tree 
initialize the root
*/
CourseBST::CourseBST() {
	root = nullptr;
}
/*
 Deconstruct the Binary Search Tree by passing the root
 to the function that recursively deletes each node
 */
CourseBST::~CourseBST() {
	DeleteRecursive(root);
}
/*
Delete the Binary Search Tree nodes
this is used by the deconstructor
 */
void CourseBST::DeleteRecursive(Node* node) {
	if (node) {
		DeleteRecursive(node->left);
		DeleteRecursive(node->right);
		delete node;
	}
}
/*
 Insert a node into Binary Search Tree
 */
void CourseBST::Insert(Course course) {
	//If the root is not initialized
	if (root == nullptr)
		//initialize it with the current course info
		root = new Node(course);
	//Else, send the node to the addNode method
	
		
		//to be added to Binary Search Tree
	else

		this->addNode(root, course);
}
/*
 Returns the number of prereqs the passed course has
 */
int CourseBST::NumPrerequisiteCourses(Course course) {
	int count = 0;
	for (unsigned int i = 0; i < course.preReqs.size(); i++) {
		if (course.preReqs.at(i).length() > 0)
			count++;
	}
	return count;
}
/*
 Passes Binary Search Tree root to the private method
 */
void CourseBST::PrintSampleSchedule() {
	this->printSampleSchedule(root);
}
/*
 Passes the Binary Search Tree root and the course Id
 to be searched for and displayed if found
 */
void CourseBST::PrintCourseInformation(string courseNum) {
	this->printCourseInformation(root, courseNum);
}
/*
Utilizes the public Insert method
 Determines and sets correct alphanumeric location
 */
void CourseBST::addNode(Node* node, Course course) {
	
	if (node->course.courseNum.compare(course.courseNum) > 0) {
		if (node->left == nullptr)
			node->left = new Node(course);
		else
			this->addNode(node->left, course);
	}
	
	else {
		if (node->right == nullptr)
			node->right = new Node(course);
		else
			this->addNode(node->right, course);
	}
}
/*
Recursively prints the loaded courses
 */
void CourseBST::printSampleSchedule(Node* node) {
	if (node != nullptr) {
		printSampleSchedule(node->left);
		cout << node->course.courseNum << ", " << node->course.courseName << endl;
		printSampleSchedule(node->right);
	}
	return;
}
/*
 Displays a courses information with prereqs
 */
void CourseBST::printCourseInformation(Node* curr, string courseNum) {
	// Traverse Binary Search Tree until bottom or matching courseNum 
	while (curr != nullptr) {
		if (curr->course.courseNum.compare(courseNum) == 0) {
			// Display course and get num prereqs
			cout << endl << curr->course.courseNum << ", " << curr -> course.courseName << endl;
			unsigned int size = NumPrerequisiteCourses(curr->course);
			cout << "Prerequisite(s): ";
			// If prereqs exist, display prereqs
			unsigned int i = 0;
			for (i = 0; i < size; i++) {
				cout << curr->course.preReqs.at(i);
				if (i != size - 1)
					cout << ", ";
			}
			// If no prereqs, Prompt user
			if (i == 0)
				cout << "No prerequisites required.";
			cout << endl;
			return;
		}
		// if courseNum is smaller than the current courseNum traverse left
		else if (courseNum.compare(curr->course.courseNum) < 0)
			curr = curr->left;
		// if courseNum is larger than the current courseNum traverse right
		else
			curr = curr->right;
	}
	// nullptr has been reached, therfore no course found
		cout << "Error: Course " << courseNum << " not found." << endl;
}

// Static methods


	
	//Load Courses from cin choice for csv file
	bool loadCourses(string csvPath, CourseBST * coursesBST) {
	// Open course file
	try {
		ifstream courseFile(csvPath);
		if (courseFile.is_open()) {
			while (!courseFile.eof()) {
				// A vector will hold each csv that a course contains
				vector<string> courseInfo;
				string courseData;
				getline(courseFile, courseData);
				while (courseData.length() > 0) {
					// Get substring of each course data, add to vector & delete from str
						unsigned int comma = courseData.find(',');
					if (comma < 100) { 
							courseInfo.push_back(courseData.substr(0, comma));
						courseData.erase(0, comma + 1);
					}
					// Add the last course
					else {
						courseInfo.push_back(courseData.substr(0,
							courseData.length()));
						courseData = "";
					}
				}
				// Load the separated values into a course,
				//insert into Binary Search Tree and close file
				Course course;
				course.courseNum = courseInfo[0];
				course.courseName = courseInfo[1];
				for (unsigned int i = 2; i < courseInfo.size(); i++) {
					course.preReqs.push_back(courseInfo[i]);
				}
				coursesBST->Insert(course);
			}
			courseFile.close();
			return true;
		}
	}
	catch (csv::Error& e) {
		cerr << e.what() << endl;
	}
	return false;
}



	// Main method
	
	int main(int argc, char* argv[]) {
	// Process command line argument
	string csvPath, courseId;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		break;
	case 3:
		csvPath = argv[1];
		courseId = argv[2];
		break;
	default:
		csvPath = "";
		break;
	}
	// Define Binary Search Tree and Welcome User
	CourseBST* coursesBST = nullptr;
	cout << "\nWelcome to the course planner!\n" << endl;
	// Making user choice a string and convert first char to int
		string choice = "0";
	int userChoice = choice[0] - '0';
	//if not 9 / exit
	while (userChoice != 9) {
		cout << " 1. Load Data Structure" << endl;
		cout << " 2. Print Course List" << endl;
		cout << " 3. Print Course" << endl;
		cout << " 9. Exit" << endl;
		cout << "\nPlease Choose a Valid option. ";
		cin >> choice;
		// Check if user choice is a double digit
		if (choice.length() == 1)
			userChoice = choice[0] - '0';
		else
			userChoice = 0;
		bool success = 1;
		// Handle users choice
		switch (userChoice) {
			// Instantiate Binary Search Tree
			// get file path name
			//load courses into Binary Search Tree
		case 1:
			if (coursesBST == nullptr)
				coursesBST = new CourseBST();
			if (csvPath.length() == 0) {
				
					
					cout << "Enter the file name which contains the course data: ";
				cin >> csvPath;
			}
			// Determine if the file was opened successfuly
			success = loadCourses(csvPath, coursesBST);
			if (success)
				cout << "Courses loaded.\n" << endl;
			else
				cout << "Error: File not found.\n" << endl;
			csvPath = "";
			break;
			// Validate that a Binary Search Tree exists
			//and prints Courses
		case 2:
			if (coursesBST != nullptr && success) {
				cout << "All Courses:\n" << endl;
				coursesBST->PrintSampleSchedule();
				cout << endl;
			}
			else
				cout << "Load courses first - option 1\n" << endl;
			break;
			// Validate that a Binary Search Tree exists
			//and search display course info
		case 3:
			if (coursesBST != nullptr && success) {
				if (courseId.length() == 0) {
					cout << "Enter Course id: ";
					cin >> courseId;
					for (auto& userChoice : courseId) userChoice =
						toupper(userChoice);
				}
				coursesBST->PrintCourseInformation(courseId);
				cout << endl;
			}
			else
				cout << "Load courses first - option 1\n" << endl;
			courseId = "";
			break;
			// User entered invalid data or exited the program
		default:
			if (userChoice != 9)
				cout <<"Error: " << choice << " is not a valid option\n" << endl;
			break;
		}
	}
	cout << "\nThank you for using Course Planner." << endl;
	return 0;
}




	
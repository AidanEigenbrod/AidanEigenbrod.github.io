#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
using namespace std;

// Enhanced Course Planner
// Category Two: Algorithms & Data Structures
// Enhanced by Aidan Eigenbrod – CS 300 (Updated)

struct Course {
    string courseN;                  // Course number (e.g., CS300)
    string courseT;                  // Course title
    vector<string> preRequisites;    // List of prerequisite course numbers
};

// Splits a CSV line into fields
vector<string> splitLine(const string& line) {
    vector<string> courseData;
    string temp;

    for (char c : line) {
        if (c == ',') {
            if (!temp.empty()) courseData.push_back(temp);
            temp.clear();
        }
        else {
            temp += c;
        }
    }

    if (!temp.empty()) courseData.push_back(temp);
    return courseData;
}

// Loads all courses from the CSV file into the hash map
void loadCourses(unordered_map<string, Course>& courseMap, const string& fileName) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error: Cannot open file " << fileName << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        vector<string> courseData = splitLine(line);
        if (courseData.size() < 2) continue;

        Course course;
        course.courseN = courseData[0];
        course.courseT = courseData[1];

        // Store all prerequisites after the first two fields
        for (size_t i = 2; i < courseData.size(); ++i) {
            course.preRequisites.push_back(courseData[i]);
        }

        courseMap[course.courseN] = course;
    }

    cout << "Courses loaded successfully!" << endl;
}

// Optimized prerequisite check using a set to avoid repeated reporting
void optimizedValidatePrerequisites(const unordered_map<string, Course>& courseMap) {
    unordered_set<string> missingCourses;

    // Collect all missing prerequisite IDs
    for (const auto& pair : courseMap) {
        const Course& course = pair.second;

        for (const string& preReq : course.preRequisites) {
            if (courseMap.find(preReq) == courseMap.end()) {
                missingCourses.insert(preReq);   // Avoid duplicates automatically
            }
        }
    }

    // Only display results if something is missing
    if (!missingCourses.empty()) {
        cout << "\nMissing Prerequisite Courses:\n";
        for (const string& missing : missingCourses) {
            cout << " - " << missing << endl;
        }
        cout << endl;
    }
}

// Fast lookup for a single course and its prerequisites
void optimizedSearchCourse(const unordered_map<string, Course>& courseMap,
    const string& courseNumber)
{
    auto it = courseMap.find(courseNumber);

    if (it == courseMap.end()) {
        cout << "Error: Course " << courseNumber << " not found.\n";
        return;
    }

    const Course& course = it->second;

    cout << "Course Number: " << course.courseN << endl;
    cout << "Title: " << course.courseT << endl;

    if (course.preRequisites.empty()) {
        cout << "Prerequisites: None\n";
        return;
    }

    cout << "Prerequisites:\n";

    // Print full prerequisite info when available
    for (const string& req : course.preRequisites) {
        auto pre = courseMap.find(req);
        if (pre != courseMap.end()) {
            cout << " - " << pre->second.courseN
                << " (" << pre->second.courseT << ")\n";
        }
        else {
            cout << " - " << req << " (Not found)\n";
        }
    }
}

// Prints courses alphabetically by course number
void sortCourseList(const unordered_map<string, Course>& courseMap) {
    vector<string> sortedKeys;

    // Extract keys for sorting
    for (const auto& pair : courseMap) {
        sortedKeys.push_back(pair.first);
    }

    sort(sortedKeys.begin(), sortedKeys.end());

    cout << "\nSorted Course List:\n";
    for (const string& key : sortedKeys) {
        cout << key << ": " << courseMap.at(key).courseT << endl;
    }
}

int main() {
    unordered_map<string, Course> courseMap;   // Main data structure (hash map)
    int choice;
    string fileName;
    string courseNumber;

    cout << "Enter CSV file path: ";
    getline(cin, fileName);

    do {
        cout << "\n------ Course Planner Menu ------\n";
        cout << "1. Load course data\n";
        cout << "2. Print all courses\n";
        cout << "3. Print course information\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        // Handles invalid input (letters, symbols, etc.)
        if (cin.fail()) {
            cin.clear();
            cin.ignore(20, '\n');
            choice = 0;
        }

        switch (choice) {
        case 1:
            loadCourses(courseMap, fileName);
            optimizedValidatePrerequisites(courseMap);  // Improved check
            break;

        case 2:
            if (courseMap.empty()) {
                cout << "Error: No course data loaded.\n";
                break;
            }
            sortCourseList(courseMap);
            break;

        case 3:
            if (courseMap.empty()) {
                cout << "Error: No course data loaded.\n";
                break;
            }
            cout << "Enter course number: ";
            cin >> courseNumber;
            optimizedSearchCourse(courseMap, courseNumber);
            break;

        case 9:
            cout << "Goodbye!" << endl;
            break;

        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 9);

    return 0;
}
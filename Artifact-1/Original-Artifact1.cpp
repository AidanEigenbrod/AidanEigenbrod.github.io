#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

//Created By Aidan Eigenbrod
//CS 300 2025

struct Course {
    string courseN;
    string courseT;
    vector<string> preRequisites;
};

//Splits a line by commas
vector<string> splitLine(const string& line) {
    vector<string> courseData;
    string temp = "";
    for (char c : line) {
        if (c == ',') {
            if (!temp.empty()) courseData.push_back(temp);
            temp = "";
        }
        else {
            temp += c;
        }
    }
    if (!temp.empty()) courseData.push_back(temp);
    return courseData;
}

//Loads courses
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
        for (size_t i = 2; i < courseData.size(); ++i)
            course.preRequisites.push_back(courseData[i]);

        courseMap[course.courseN] = course;
    }

    file.close();
    cout << "Courses loaded successfully!" << endl;
}

//Validates prerequisites
void validatePrerequisites(const unordered_map<string, Course>& courseMap) {
    for (auto validate = courseMap.begin(); validate != courseMap.end(); ++validate) {
        const Course& course = validate->second;
        for (size_t i = 0; i < course.preRequisites.size(); ++i) {
            const string& preReq = course.preRequisites[i];
            if (courseMap.find(preReq) == courseMap.end()) {
                cout << "Prerequisite " << preReq
                    << " for course " << course.courseN << " not found." << endl;
            }
        }
    }
}

//Searches for a specific course
void searchCourse(const unordered_map<string, Course>& courseMap, const string& courseNumber) {
    auto it = courseMap.find(courseNumber);
    if (it != courseMap.end()) {
        const Course& course = it->second;
        cout << "Course Number: " << course.courseN << endl;
        cout << "Title: " << course.courseT << endl;

        if (course.preRequisites.empty()) {
            cout << "Prerequisites: None" << endl;
        }
        else {
            cout << "Prerequisites:\n";
            for (size_t i = 0; i < course.preRequisites.size(); ++i) {
                const string& req = course.preRequisites[i];
                auto pre = courseMap.find(req);
                if (pre != courseMap.end())
                    cout << pre->second.courseN << " (" << pre->second.courseT << ") \n";
                else
                    cout << req << " ";
            }
            cout << endl;
        }
    }
    else {
        cout << "Error: Course " << courseNumber << " not found." << endl;
    }
}

//Prints all courses sorted alphanumerically
void printCourseList(const unordered_map<string, Course>& courseMap) {
    vector<string> keys;
    for (auto printCourse = courseMap.begin(); printCourse != courseMap.end(); ++printCourse) {
        keys.push_back(printCourse->first);
    }

    sort(keys.begin(), keys.end());

    for (size_t i = 0; i < keys.size(); ++i) {
        const string& key = keys[i];
        cout << key << ": " << courseMap.at(key).courseT << endl;
    }
}

int main() {
    unordered_map<string, Course> courseMap;
    int choice;
    string fileName;
    string courseNumber;

    cout << "Enter the CSV file name. Needs to be full path to work properly: ";
    getline(cin, fileName);

    do {
        cout << "\n1. Load course data from file\n";
        cout << "2. Print all courses\n";
        cout << "3. Print course information\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {               
            cin.clear();                
            cin.ignore(20, '\n');     
            choice = 0;                 
        }

        switch (choice) {
        case 1:
            loadCourses(courseMap, fileName);
            validatePrerequisites(courseMap);
            break;
        case 2:
            printCourseList(courseMap);
            break;
        case 3:
            if (courseMap.empty()) {
                cout << "Error: No course data loaded. Please choose option 1 first." << endl;
                break;
            }

            cout << "Enter course number: ";
            cin >> courseNumber;
            searchCourse(courseMap, courseNumber);
            break;
        case 9:
            cout << "Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice." << endl;
        }
    } while (choice != 9);

    return 0;
}
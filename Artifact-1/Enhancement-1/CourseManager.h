#ifndef COURSE_MANAGER_H
#define COURSE_MANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

// Stores information for each course
struct Course {
    string courseN;
    string courseT;
    vector<string> preRequisites;
};

// Class that manages all course operations and replaces the original procedural design
class CourseManager {
private:
    unordered_map<string, Course> courseMap;

    // Splits a CSV line into parts
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

public:
    CourseManager() {}

    // Loads course data from CSV
    void loadCourses(const string& fileName) {
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

            for (size_t i = 2; i < courseData.size(); ++i) {
                course.preRequisites.push_back(courseData[i]);
            }

            courseMap[course.courseN] = course;
        }

        cout << "Courses loaded successfully." << endl;
        file.close();
    }

    // Checks for missing prerequisite courses
    void validatePrerequisites() const {
        for (const auto& entry : courseMap) {
            const Course& course = entry.second;
            for (const string& preReq : course.preRequisites) {
                if (courseMap.find(preReq) == courseMap.end()) {
                    cout << "Prerequisite " << preReq
                        << " for " << course.courseN << " not found." << endl;
                }
            }
        }
    }

    // Prints all courses alphabetically
    void printCourseList() const {
        vector<string> keys;
        for (const auto& entry : courseMap) {
            keys.push_back(entry.first);
        }

        sort(keys.begin(), keys.end());

        for (const string& key : keys) {
            cout << key << ": " << courseMap.at(key).courseT << endl;
        }
    }

    // Prints full details for a single course
    void searchCourse(const string& courseNumber) const {
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
                for (const string& req : course.preRequisites) {
                    auto pre = courseMap.find(req);
                    if (pre != courseMap.end()) {
                        cout << pre->second.courseN << " (" << pre->second.courseT << ")\n";
                    }
                    else {
                        cout << req << endl;
                    }
                }
            }
        }
        else {
            cout << "Error: Course not found." << endl;
        }
    }

    bool isEmpty() const {
        return courseMap.empty();
    }
};

#endif
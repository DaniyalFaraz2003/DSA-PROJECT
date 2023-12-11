#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <windows.h> 
#include <direct.h>
using namespace std;



class FileHandling {

public:


    bool copyFile(const string& from, const string& destination) {

        ifstream source_File(from, ios::binary);
        ofstream dest_File(destination, ios::binary);

        if (!source_File) {

            cout << "Error opening source file " << endl;
            return false;

        }

        if (!dest_File) {

            cout << "Error opening destination file " << endl;
            return false;

        }

        dest_File << source_File.rdbuf();

        if (!dest_File) {

            cout << "Error copying file from " << from << " to " << destination << endl;
            return false;

        }

        return true;
    }

    // Function to create a folder
    void createFolder(const std::string& folderPath) {
        createFolderIfNotExists(folderPath);
    }

    // Function to remove a folder
    void removeFolder(const std::string& folderPath) {
        std::string command = "rmdir /s /q " + folderPath;
        if (system(command.c_str()) != 0) {
            std::cerr << "Error removing directory: " << folderPath << std::endl;
        }
        else {
            std::cout << "Directory removed: " << folderPath << std::endl;
        }
    }

    void createFolderIfNotExists(const std::string& folderPath) {
        if (_mkdir(folderPath.c_str()) != 0) {
            std::cerr << "Error creating directory: " << folderPath << std::endl;
        }
        else {
            std::cout << "Directory created: " << folderPath << std::endl;
        }
    }


    bool removeFile(const string& path) {

        if (remove(path.c_str()) == 0) {

            return true;

        }

        else {

            cout << "Error removing file " << endl;
            return false;

        }
    }

    bool moveFile(const string& from, const string& destination) {

        if (rename(from.c_str(), destination.c_str()) == 0) {

            return true;

        }

        else {

            cout << "Error moving file: " << from << " to " << destination << endl;
            return false;

        }
    }
};

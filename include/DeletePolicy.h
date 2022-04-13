#pragma once
#include <vector>
#include <string>
#include <mutex>

using std::vector;
using std::string;

enum deletePolicyType { ALL, exclude }; // manage deleting files in directory

// Policy to manage cleaning up a directory
class DeletePolicy
{
private:
    deletePolicyType _policy;
    vector<string> _exclude;
    std::mutex *_mutex;

public:
    DeletePolicy(deletePolicyType policy, vector<string> exclude); // if some files to exclude
    DeletePolicy(deletePolicyType policy); // if delete policy is ALL
    void CleanUp(string &dir); // delete files in directory

    ~DeletePolicy();
    DeletePolicy(const DeletePolicy &); // copy constructor
    DeletePolicy &operator=(const DeletePolicy &); // copy assignment operator
    DeletePolicy(DeletePolicy &&); // move constructor
    DeletePolicy &operator=(DeletePolicy &&); // assignment move constructor
};

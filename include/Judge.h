#include <string>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <atomic>
#include "model/Problem.h"
#include "muduo/base/Timestamp.h"

using namespace std;

static const string samplePath = "../sample/";
static const string tempPath = "../temp/";

namespace Judge
{

string cppSrcPath(const string& name)
{
    return tempPath + name + ".cc";
}

string compileErrorPath(const string& name)
{
    return tempPath + name + ".compile_error";
}

string exePath(const string& name)
{
    return tempPath + name + ".out";
}

string stdinPath(const string& name)
{
    return tempPath + name + ".stdin";
}

string stdoutPath(const string& name)
{
    return tempPath + name + ".stdout";
}

string stderrorPath(const string& name)
{
    return tempPath + name + ".stderror";
}

string inputPath(int pid)
{
    return samplePath + to_string(pid) + "_input.txt";
}

string outputPath(int pid)
{
    return samplePath + to_string(pid) + "_output.txt";
}

bool read(const string& filePath, string& content)
{
    content.clear();
    ifstream file(filePath.c_str());
    if (!file.is_open()) {
        return false;
    }

    //一个进程可以打开多少个文件
    //ulimint -a  可以修改
    string line;
    while(getline(file, line)) {
        content += line + "\n"; 
    }
    file.close();
    return true;
}

bool write(const string& filePath, const string& content)
{
    ofstream file(filePath.c_str());
    if(!file.is_open())
    {
        return false;
    }
    file.write(content.c_str(), content.size());
    file.close();
    return true;
}

string writeCppTmpFile(const string& code, const string& input)
{
    static atomic_int id(0);
    ++id;
    string filename = "tmp_"  + Timestamp::now().toString() + "_"+ to_string(id);
    write(cppSrcPath(filename),code);
    write(stdinPath(filename),input);
    return filename;
}

bool compileCpp(const string& filename)
{
    char * command[20] = {0};
    char buf[20][50] = {{0}};
    for(int i = 0;i < 20;i++)
    {
        command[i] = buf[i];
    }
    sprintf(command[0], "%s", "g++");
    sprintf(command[1], "%s", cppSrcPath(filename).c_str());
    sprintf(command[2], "%s", "-o");
    sprintf(command[3], "%s", exePath(filename).c_str());
    sprintf(command[4], "%s", "-std=c++11");
    command[5] = nullptr;
    int ret = fork();
    if(ret > 0)
    {
        waitpid(ret, nullptr, 0);
    }
    else
    {
        int fd = open(stderrorPath(filename).c_str(),O_WRONLY | O_CREAT, 0666);
        if(fd < 0)
        {
            exit(1);
        }
        dup2(fd, 2);
        execvp(command[0],command);
        exit(0);
    }

    struct stat st;
    ret = stat(exePath(filename).c_str(), &st);
    if(ret < 0)
    {
        return false;
    }
    return true;
}

int runCpp(const string& filename)
{
    int ret = fork();
    if(ret > 0)
    {
        int status = 0;
        waitpid(ret, &status, 0);
        return status & 0x7f;
    }
    else
    {
        int fd_stdin = open(stdinPath(filename).c_str(), O_RDONLY);
        dup2(fd_stdin, 0);
        int fd_stdout = open(stdoutPath(filename).c_str(), O_WRONLY | O_CREAT, 0666);
        dup2(fd_stdout, 1);
        int fd_stderror = open(stderrorPath(filename).c_str(), O_WRONLY | O_CREAT, 0666);
        dup2(fd_stderror, 2);
 
        execl(exePath(filename).c_str(), exePath(filename).c_str(), nullptr);
        exit(0);
    }
 
}
 
bool compileAndRunCpp(const string& code, const string& input)
{
    string filename = writeCppTmpFile(code, input);
    if(compileCpp(filename)== false)
    {
        return false;
    }
    runCpp(filename);
    return true;
}
 
 
bool judge(const string& code, const Problem& problem)
{
    int pid = problem.getPid();
    string input;
    read(inputPath(pid),input);
    
    string filename = writeCppTmpFile(code, input);
    if(compileCpp(filename) == false)
    {
        return false;
    }

    runCpp(filename);

    string output;
    read(stdoutPath(filename), output);
    string sample;
    read(outputPath(pid), sample);
    if(output == sample)
    {
        return true;
    }

    return false;
}
 
};
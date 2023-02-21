#pragma once
#include "Judge.h"
#include <string>
#include <atomic>
#include <fstream>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../model/Problem.h"
#include "../tinymuduo/base/Timestamp.h"

class PyJudge : public Judge
{
public:
    bool judge(const string &code, const Problem &problem)
    {
        int pid = problem.getPid();
        string input;
        read(inputPath(pid),input);

        string filename = WritePythonTmpFile(code, input);

        runPython(filename);

        string output;
        read(stdoutPath(filename), output);
        string sample;
        read(outputPath(pid), sample);

        if(output==sample)
        {
            return true;
        }
        return false;
    }

private:
    string pySrcPath(const string &name)
    {
        return tempPath + name + ".py";
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

    string WritePythonTmpFile(const std::string& code, const std::string& stdininfo) 
    {
        //定义一个原子的id变量  使用方法与普通的相同  但是使用算数操作时是原子的
        //原子操作依赖cpu支持
        static std::atomic_int id(0);
        ++id;
        //为了保证文件名的唯一性
        std::string file_name = "tmp_" + Timestamp::now().toString()
            + "." +  std::to_string(id);
        write(pySrcPath(file_name), code);  
        write(stdinPath(file_name), stdininfo);
        return file_name;
    }

    bool runPython(const string& filename)
    {
        //1. 创建子进程
        int ret = fork();
        if (ret > 0) {
            //2. 父进程进行等待
            int status = 0;
            waitpid(ret, &status, 0);
            return status & 0x7f;
        }else {
            //3. 进行重定向   (标准输入 标准输出  标准错误)
            int fd_stdin = open(stdinPath(filename).c_str(), O_RDONLY);
            dup2(fd_stdin, 0);
            int fd_stdout = open(stdoutPath(filename).c_str(), O_WRONLY | O_CREAT, 0666);
            dup2(fd_stdout, 1);
            int fd_stderror = open(stderrorPath(filename).c_str(), O_WRONLY | O_CREAT, 0666);
            dup2(fd_stderror, 2);
            //4. 子进程进行程序替换
            execlp("python", "python", pySrcPath(filename).c_str(), NULL);
            exit(0);
        }
    }
private:
    static const string samplePath;
    static const string tempPath;
};

const string PyJudge::samplePath = "../sample/";
const string PyJudge::tempPath = "../temp/";
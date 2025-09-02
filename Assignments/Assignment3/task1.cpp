#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <sstream>
#include <regex>
#include <memory>
#include <algorithm>
using namespace std;

class FileSystemNode
{
public:
    string name;
    bool isFile;
    string content;
    map<string, shared_ptr<FileSystemNode> > children;
    // 构建的时候得有.. 表示父节点
    FileSystemNode(const string &name, bool isFile) : name(name), isFile(isFile), content("") {}
};
class FileSystem
{
private:
    shared_ptr<FileSystemNode> root;
    shared_ptr<FileSystemNode> currentDir;

public:
    FileSystem()
    {
        root = make_shared<FileSystemNode>("/", false);
        root->children[".."] = root;
        currentDir = root;
    }

    // 辅助函数：拆分路径
    vector<string> splitPath(const string &path)
    {
        vector<string> parts;
        string current;
        int n = path.size();

        for (int i = 0; i < n; i++)
        {
            if (path[i] == '/')
            {
                if (!current.empty())
                {
                    parts.push_back(current);
                    current.clear();
                }
                if (i + 1 < n && path[i + 1] == '/')
                {
                    current += '/';
                }
            }
            else
            {
                current += path[i];
            }
        }
        if (!current.empty())
        {
            parts.push_back(current);
        }
        return parts;
    }

    // 辅助函数：解析路径
    shared_ptr<FileSystemNode> resolvePath(const string &path)
    {
        shared_ptr<FileSystemNode> current = (path[0] == '/') ? root : currentDir;
        vector<string> parts = splitPath(path);

        for (const auto &part : parts)
        {
            if (part == ".")
                continue;
            if (part == "..")
            {
                // 回到上一级目录
                if (current != root)
                    current = current->children[".."];
            }
            else
            {
                // 它只能解析路径
                if (current->children.count(part) == 0 || current->children[part]->isFile)
                {
                    return nullptr; // 路径不存在或指向文件
                }
                current = current->children[part];
            }
        }
        return current;
    }
    // 这个可以返回文件见
    shared_ptr<FileSystemNode> resolveFilePath(const string &path)
    {
        shared_ptr<FileSystemNode> current = (path[0] == '/') ? root : currentDir;
        vector<string> parts = splitPath(path);

        for (size_t i = 0; i < parts.size(); ++i)
        {
            const auto &part = parts[i];
            if (part == ".")
                continue;
            if (part == "..")
            {
                if (current != root)
                    current = current->children[".."];
            }
            else
            {
                if (current->children.count(part) == 0)
                {
                    return nullptr;
                }
                current = current->children[part];
            }
        }
        // 最终返回当前解析的节点（可以是文件）
        return current;
    }

    // 辅助函数：返回路径的父目录
    shared_ptr<FileSystemNode> getParent(const string &path)
    {
        if (path.empty())
        {
            return nullptr;
        }

        bool isAbsolutePath = (path[0] == '/');
        vector<string> parts = splitPath(path);

        // 如果路径为空或只有一个部分，父目录是根目录（绝对路径）或当前目录（相对路径）
        if (parts.empty())
        {
            // cout<<"empty parts"<<endl;
            return isAbsolutePath ? root : nullptr; // 区分绝对路径和相对路径
        }

        if (parts.size() == 1)
        {
            // cout<<"only 1 part"<<endl;
            return isAbsolutePath ? root : currentDir; // 父目录是根目录或当前目录
        }
        // 去掉最后一个部分（目标文件或目录）
        parts.pop_back();

        // 构造父目录路径
        string parentPath = ""; // 如果是绝对路径，以 '/' 开头
        for (size_t i = 0; i < parts.size(); ++i)
        {
            if (i > 0 || isAbsolutePath)
            { // 添加 '/' 分隔符
                parentPath += "/";
            }
            parentPath += parts[i];
        }

        // 使用 resolvePath 解析父目录路径
        shared_ptr<FileSystemNode> parentDir = resolvePath(parentPath);

        if (!parentDir)
        {
            //cout << "error: parent directory does not exist for path: " << parentPath << endl;
        }
        return parentDir;
    }

    // 辅助函数：返回路径的最后部分
    string getLastPathComponent(const string &path)
    {
        vector<string> parts = splitPath(path);

        // 如果路径最后有斜杠并且为空，去掉空部分
        while (!parts.empty() && parts.back().empty())
        {
            parts.pop_back();
        }

        // 如果处理后仍然有内容，返回最后一个部分
        return parts.empty() ? "" : parts.back();
    }

    // 命令：pwd
    void pwd()
    {
        // 从当前目录开始
        auto current = currentDir;
        vector<string> pathParts;

        // 一直追溯到根目录
        while (current != root)
        {
            pathParts.push_back(current->name);
            current = current->children[".."]; // 追溯到父目录
        }

        // 根目录
        pathParts.push_back("/");

        // 逆序拼接路径
        reverse(pathParts.begin(), pathParts.end());
        for (size_t i = 0; i < pathParts.size(); ++i)
        {
            if (i > 1)
                cout << "/";
            cout << pathParts[i];
        }
        cout << endl;
    }

    // 命令：cd
    void cd(const string &path)
    {
        shared_ptr<FileSystemNode> target = resolvePath(path);
        if (!target || target->isFile)
        {
            cout << "error" << endl;
        }
        else
        {
            currentDir = target;
        }
    }

    // 命令：mkdir
    void mkdir(const string &path)
    {   
        if (resolvePath(path) != nullptr)
        {   
            cout<<"error"<<endl;
            //cout << "error: the path already exists" << endl;
            return;
        }

        // 确定起始节点：绝对路径从 root 开始，相对路径从 currentDir 开始
        auto current = (path[0] == '/') ? root : currentDir;

        // 将路径分割为各部分
        vector<string> parts = splitPath(path);

        // 遍历路径部分，逐级检查或创建目录
        for (size_t i = 0; i < parts.size(); ++i)
        {
            const string &part = parts[i];
            if(part==".")continue;
            if(part==".."){
                current=current->children[".."];
                continue;
            }
            // 如果部分不存在，则创建目录
            if (!current->children.count(part))
            {
                auto newDir = make_shared<FileSystemNode>(part, false);
                current->children[part] = newDir;
                newDir->children[".."] = current;
                newDir->children["."] = newDir;
            }

            // 移动到下一层目录
            current = current->children[part];
        }
    }

    // 命令：echo
    void echo(const string &content)
    {
        cout << content << endl;
    }
    void echo(const string &content, const string &path, bool append)
    {
        // cout<<"path="<<path<<endl;
        shared_ptr<FileSystemNode> parentDir = getParent(path);
        string fileName = getLastPathComponent(path);

        if (!parentDir)
        {
            string parentPath = path.substr(0, path.size() - fileName.size());
            if(!parentPath.empty())mkdir(parentPath);
            parentDir=getParent(path);
            if(!parentDir){
                cout<<"error"<<endl;
                //cout << "error: no parent dir " << endl;
                return;
            }
        }
        if (parentDir->isFile)
        {   
            cout<<"error"<<endl;
            //cout << "error: parent is not a dir" << endl;
            return;
        }

        if (!parentDir->children.count(fileName))
        {
            parentDir->children[fileName] = make_shared<FileSystemNode>(fileName, true);
        }

        shared_ptr<FileSystemNode> fileNode = parentDir->children[fileName];
        fileNode->children[".."]=parentDir;
        fileNode->children["."]=fileNode;

        if (!fileNode->isFile)
        {   
            cout<<"error"<<endl;
            //cout << "error: not a file" << endl;
        }
        else
        {
            if (append)
            {
                fileNode->content += content + "\n";
            }
            else
            {
                fileNode->content = content + "\n";
            }
        }
    }

    // 命令：cat
    void cat(const string &path)
    {
        shared_ptr<FileSystemNode> fileNode = resolveFilePath(path);
        if (!fileNode)
        {
            cout<<"error"<<endl;
            //cout << "error: not exsit" << endl;
        }
        else if (!fileNode->isFile)
        {
            cout<<"error"<<endl;
            //cout << "error: not a file" << endl;
        }
        else
        {
            cout << fileNode->content;
        }
    }
    void ls(const string &path)
    {
        shared_ptr<FileSystemNode> dir = resolveFilePath(path);
        if (!dir)
        {
            cout<<"error"<<endl;
            //cout << "error: not exsit" << endl;
            return;
        }
        if (dir->isFile)
        {   
            cout<<"error"<<endl;
            //cout << "error: not a dir" << endl;
            return;
        }
        
        // 收集所有子节点的名称
        vector<string> entries;
        entries.push_back(".");
        entries.push_back("..");
        for (const auto &[name, child] : dir->children)
        {   
            if(name=="."||name=="..")continue;
            entries.push_back(name);
        }

        // 按字典序排序
        sort(entries.begin(), entries.end());

        // 输出子节点名称，用制表符分隔
        for (size_t i = 0; i < entries.size()-1; ++i)
        {
            cout << entries[i] << "\t";
        }
        cout<<entries[entries.size()-1];
        cout << endl;
    }

    // 命令：find
    void find(const string &path, const string &pattern)
    {
       auto startDir = resolvePath(path);
        if (!startDir || startDir->isFile)
        {   
            cout<<"error"<<endl;
            //cout << "error: path does not exist or is not a directory" << endl;
            return;
        }

        
        string pattern_sub = pattern.substr(1, pattern.size() - 2);
        string regexPattern = "^" + regex_replace(pattern_sub, regex(R"(\*)"), R"(.*)") + "$";
        regex re(regexPattern);

        // 递归查找
        vector<string> results;

        if(regex_match(startDir->name,re))results.push_back("");

        findRecursive(startDir, re, "", results);

        sort(results.begin(),results.end());

        // 输出结果
        for (const auto &result : results)
        {
            cout << path<<result << endl;
        }
    }

    void findRecursive(shared_ptr<FileSystemNode> node, const regex &re, const string &currentPath, vector<string> &results)
    {
        for (const auto &[name, child] : node->children)
        {
            // 忽略 "." 
            if (name == "."||name=="..")
                continue;

            // 构建完整路径
            string fullPath = currentPath.empty() ? name : currentPath + "/" + name;

            // 检查是否匹配
            if (regex_match(name, re))
            {
                results.push_back(fullPath);
            }

            // 如果是目录，递归调用
            if (!child->isFile)
            {
                findRecursive(child, re, fullPath, results);
            }
        }
    }
    // 命令: mv
    void mv(const string &sourcePath, const string &destPath)
    {
        // 解析源路径
        shared_ptr<FileSystemNode> sourceParent = getParent(sourcePath);
        string sourceName = getLastPathComponent(sourcePath);

        if (!sourceParent || sourceParent->children.count(sourceName) == 0)
        {
            cout<<"error"<<endl;
            //cout << "error: source path does not exist" << endl;
            return;
        }
        shared_ptr<FileSystemNode> sourceNode = sourceParent->children[sourceName];

        // 检查目标路径是否以 '/' 结尾
        bool isDestDir = destPath.back() == '/';
        shared_ptr<FileSystemNode> destParent;
        string destName;

        if (isDestDir)
        {
            // 目标是目录路径
            destParent = resolvePath(destPath);
            if (!destParent || destParent->isFile)
            {
                cout<<"error"<<endl;
                //cout << "error: destination directory does not exist" << endl;
                return;
            }
            // 将源文件或目录移动到目标目录下，保留原名称
            destName = sourceName;
        }
        else
        {
            // 目标是文件路径
            destParent = getParent(destPath);
            if (!destParent || destParent->isFile)
            {
                cout<<"error"<<endl;
                //cout << "error: destination parent directory does not exist" << endl;
                return;
            }
            destName = getLastPathComponent(destPath);

            // 如果目标已经存在，则报错
            if (destParent->children.count(destName))
            {
                destParent->children.erase(destName);
            }
        }

        // 如果移动回原位就不动
        if (sourceParent == destParent && sourceName == destName)
        {
            return;
        }
        // 执行移动操作
        destParent->children[destName] = sourceNode; // 将源节点移动到目标父目录
        sourceNode->name = destName;                 // 更新节点名称
        sourceParent->children.erase(sourceName);    // 从源父目录中删除源节点

        sourceNode->children[".."] = destParent;
        sourceNode->children["."]=sourceNode;
    }
    // 命令：rm
    void rm(const string &path, bool isFile)
    {
        shared_ptr<FileSystemNode> parentDir = getParent(path);
        string targetName = getLastPathComponent(path);

        // 检查父目录是否存在以及目标是否存在
        if (!parentDir)
        {
            cout<<"error"<<endl;
            //cout << "error: parentdir path does not exist" << endl;
            return;
        }
        if (parentDir->children.count(targetName) == 0)
        {   
            cout<<"error"<<endl;
            //cout << "error: target does not exist,parentdir= " << parentDir->name << endl;
            //cout << targetName << endl;
            return;
        }

        shared_ptr<FileSystemNode> targetNode = parentDir->children[targetName];

        // 根据类型检查
        if (isFile && !targetNode->isFile)
        {
            cout<<"error"<<endl;
            //cout << "error: target is not a file" << endl;
            return;
        }
        if (!isFile && targetNode->isFile)
        {
            cout<<"error"<<endl;
            //cout << "error: target is not a directory" << endl;
            return;
        }

        // 执行删除
        if (targetNode->isFile)
        {
            parentDir->children.erase(targetName);
            // cout << "success: " << targetName << " removed" << endl;
            return;
        }
        if (!targetNode->isFile)
        {
            rmRecursive(targetNode);
            parentDir->children.erase(targetName);
            // cout << "success: " << targetName << " removed" << endl;
            return;
        }
    }
    // 递归删除目录及其所有子节点
    void rmRecursive(shared_ptr<FileSystemNode> targetNode)
    {
        string dirname = targetNode->name;
        // 遍历 children，删除所有子节点
        for (auto it = targetNode->children.begin(); it != targetNode->children.end();)
        {
            const string &childName = it->first;               // 子节点的名称
            shared_ptr<FileSystemNode> childNode = it->second; // 子节点的指针

            if (childName != ".."&&childName!=".")
            { // 跳过父目录引用 ("..")
                if (childNode->isFile)
                {
                    // 删除文件节点
                    // cout<<"delete file "<<childNode->name<<"\t";
                    it = targetNode->children.erase(it);
                }
                else
                {
                    // 递归删除子目录
                    rmRecursive(childNode);
                    // cout<<"delete dir "<<childNode->name<<endl;
                    it = targetNode->children.erase(it);
                }
            }
            else
            {
                ++it; // 跳过 ".."
            }
        }
        // cout<<"delete target dir "<<dirname<<endl;
    }
};

pair<int, int> countSpacesAndQuotes(const string& str) {
    int spaceCount = 0;  // 空格计数
    int quoteCount = 0;  // 单引号计数

    for (char c : str) {
        if (c == ' ') {
            spaceCount++;  // 如果是空格，计数加1
        } else if (c == '\'') {
            quoteCount++;  // 如果是单引号，计数加1
        }
    }

    return {spaceCount, quoteCount};
}
void copeStr(const string& input, string& parameter1, string& parameter2, string& parameter3, string& parameter4) {
    istringstream stream(input);
    vector<string> words;
    string word;
    bool insideQuotes = false;
    string currentArg;

    while (stream >> word) {
        if (word[0] == '\'' && word[word.length() - 1] == '\'') {
            words.push_back(word.substr(1, word.length() - 2));
        } else if (word[0] == '\'' && !insideQuotes) {
            insideQuotes = true;
            currentArg = word.substr(1);
        } else if (word[word.length() - 1] == '\'' && insideQuotes) {
            currentArg += " " + word.substr(0, word.length() - 1);
            words.push_back(currentArg);
            insideQuotes = false;
        } else if (insideQuotes) {
            currentArg += " " + word;
        } else {
            words.push_back(word);
        }
    }

    if (words.size() >= 1) parameter1 = words[0];
    if (words.size() >= 2) parameter2 = words[1];
    if (words.size() >= 3) parameter3 = words[2];
    if (words.size() >= 4) parameter4 = words[3];
}
void howEcho(FileSystem &fs, const string input)
{
    string arg1, arg2, arg3, arg4 = "";
    copeStr(input, arg1, arg2, arg3, arg4);
    // 找到重定向符号的位置
     if (arg4!=""||arg1=="")
            {
                cout<<"error"<<endl;             
            }
            if (arg2!=">>"&&arg2!=">"&&arg2!="")
            {
                cout<<"error"<<endl;
            }
            if (arg2=="")
            {
                fs.echo(arg1);
            }
            else if(arg2==">"){
                fs.echo(arg1,arg3,false);
            }
            else if(arg2==">>"){
                fs.echo(arg1,arg3,true);
            }
            


    
    // Step 1: 确定是否有重定向符号
    /*if (redirectPos != string::npos)
    {
        // 检查是否为 `>>`
        if (redirectPos + 1 < input.size() && input[redirectPos + 1] == '>')
        {
            appendMode = true;
            redirectPos += 1; // 将位置移动到 `>>` 的最后一个字符
        }

        // Step 2: 提取三个部分
        string content = input.substr(0, redirectPos);
        string path = input.substr(redirectPos + 1); // 获取重定向符号后的部分

        // 针对 `>>` 情况，确保 `>` 不会出现在内容中
        if (appendMode && content.back() == '>')
        {
            content = content.substr(0, content.size() - 1);
        }

        // 去掉路径的多余空格
        path = path.substr(path.find_first_not_of(" "));

        // 去掉内容部分的多余空格
        content = content.substr(content.find_first_not_of(" "));
        content = content.substr(0, content.find_last_not_of(" ") + 1);

        if (content.front() == '\'' && content.back() == '\'')
        {
            content = content.substr(1, content.length() - 2); // 去掉首尾的单引号
        }else{
            int count=countSpacesAndQuotes(content).first;
            if(count!=0){
                cout<<"error"<<endl;
                return;
            }
        }
        // 如果路径为空，报错
        if (path.empty())
        {
            cout<<"error"<<endl;
            return;
        }

        // Step 3: 调用对应的文件系统方法
        if (appendMode)
        {
            fs.echo(content, path, true); // 追加模式
        }
        else
        {
            fs.echo(content, path, false); // 覆盖模式
        }
    }
    else
    {
        // 没有重定向符号，直接输出内容
        string content = input;

        // 去掉多余空格
        content = content.substr(content.find_first_not_of(" "));
        content = content.substr(0, content.find_last_not_of(" ") + 1);

        if (content.front() == '\'' && content.back() == '\'')
        {
            content = content.substr(1, content.length() - 2); // 去掉首尾的单引号
        }
        fs.echo(content);
    }*/
}

void executeCommand(FileSystem &fs, const string &command)
{
    stringstream ss(command);
    string cmd;
    ss >> cmd;

    if (cmd == "pwd")
    {
        string extra;
        if (ss >> extra)
        {
            cout << "error" << endl;
        }
        else
        {
            fs.pwd();
        }
    }
    else if (cmd == "cd")
    {
        string path, extra;
        ss >> path;
        if (path.empty() || (ss >> extra))
        {
            cout << "error" << endl;
        }
        else
        {
            fs.cd(path);
        }
    }
    else if (cmd == "mkdir")
    {
        string arg, path, extra;
        ss >> arg >> path;
        if (arg != "-p" || path.empty() || (ss >> extra))
        {
            cout << "error" << endl;
        }
        else
        {
            fs.mkdir(path);
        }
    }
    else if (cmd == "echo")
    {
        string str;
        getline(ss, str);
        if (str.empty())
        {
            cout << "error" << endl;
        }
        else
        {
            howEcho(fs, str);
        }
    }
    else if (cmd == "cat")
    {
        string path, extra;
        ss >> path;
        if (path.empty() || (ss >> extra))
        {
            cout << "error" << endl;
        }
        else
        {
            fs.cat(path);
        }
    }
    else if (cmd == "ls")
    {
        string arg, path, extra;
        ss >> arg >> path;
        if (arg != "-a" || path.empty() || (ss >> extra))
        {
            cout << "error" << endl;
        }
        else
        {
            fs.ls(path);
        }
    }
    else if (cmd == "mv")
    {
        string arg1, arg2, extra;
        ss >> arg1 >> arg2;
        if (arg2.empty())
        {
            cout << "error" << endl;
            return;
        }
        if (ss >> extra)
        {
            cout << "error" << endl;
            return;
        }
        fs.mv(arg1, arg2);
    }
    else if (cmd == "rm")
    {
        string arg1, arg2, extra;
        ss >> arg1;
        if (arg1 != "-r")
        {
            if (ss >> extra)
            {
                cout << "error" << endl;
                return;
            }
            fs.rm(arg1, true);
        }
        else
        {
            ss >> arg2;
            if (ss >> extra)
            {
                cout << "error" << endl;
                return;
            }
            fs.rm(arg2, false);
        }
        return;
    }
    else if(cmd=="find"){
        string path,arg,reg,extra;
        ss>>path>>arg>>reg;
        if(reg.empty()||ss>>extra||arg!="-name"){
            cout<<"error"<<endl;
            return;
        }
        if(reg.front()!='\''||reg.back()!='\''){
            cout << "error" << endl;
            return;
        }
        fs.find(path,reg);
    }else
    {
        cout << "error"<< endl;
    }
}


int main()
{
    FileSystem fs;
    int n;
    cin >> n;
    cin.ignore(); // 忽略换行符

    vector<string> commands; // 存储所有输入命令

    for (int i = 0; i < n; ++i)
    {
        string command;
        getline(cin, command);
        executeCommand(fs, command);
        //commands.push_back(command); // 将命令存入容器
    }

    return 0;
}
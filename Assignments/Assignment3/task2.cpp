#include <iostream>
#include <cstring>
using namespace std;

const int MAX_FILES = 100;
const int MAX_BLOCKS = 300;
const int MAX_FILE_NAME = 11;

///////////////// COMMANDS //////////////////
/**
 * Initializes the file system with a specified number of blocks.
 * This function sets up the initial state of the file system, including
 * the bitmap and block linked list.
 * @param n - The total number of blocks available in the file system.
 */
void initFileSys(int n);

/**
 * Creates a file with a specified name and required block count.
 * This function allocates the required blocks and links them for the file.
 * If there are insufficient blocks or if the file already exists, it should
 * handle these cases.
 * @param filename - The name of the file to create.
 * @param blockCount - The number of blocks required for the file.
 */
void createFile(const char *filename, int blockCount);

/**
 * Deletes a file by name, freeing its allocated blocks.
 * This function releases the blocks used by the specified file,
 * updates the bitmap, and removes the file from the file list.
 * If the file is not found, it should handle this case.
 * @param filename - The name of the file to delete.
 */
void deleteFile(const char *filename);

/**
 * Appends a specified number of blocks to an existing file.
 * This function allocates new blocks and adds them to the end of the file鈥檚 block chain.
 * If the file does not exist or there are insufficient blocks, it should handle these cases.
 * @param filename - The name of the file to which blocks will be appended.
 * @param blockCount - The number of additional blocks required.
 */
void appendBlocks(const char *filename, int blockCount);

/**
 * Defragments the file system by reorganizing blocks to be contiguous.
 * This function attempts to remove fragmentation by moving all used blocks to the
 * beginning of the block space, thereby consolidating free space at the end.
 */
void defragment();

/**
 * Displays the current status of the file system.
 * This function outputs the block chains of all files in the system in creation order.
 * If there are no files, it outputs "EMPTY". Each file鈥檚 block chain is shown in
 * the format <filename> [block1 block2 ... -1].
 */
void displayStatus();

///////////////// HELPER FUNCTIONS //////////////////
/**
 * Finds the index of a file in the file list by name.
 * This function helps check if a file exists and locate its position
 * within the file list.
 * @param filename - The name of the file to locate.
 * @return - The index of the file in the file list, or -1 if not found.
 */
int findFile(const char *filename);

/**
 * Allocates a specified number of free blocks.
 * This function searches for available blocks in the bitmap, allocates them,
 * and updates the bitmap to reflect the allocation.
 * @param allocatedBlocks - An array to store the indices of the allocated blocks.
 * @param blockCount - The number of blocks to allocate.
 * @return - True if the required blocks are successfully allocated, otherwise false.
 */
bool allocateFreeBlocks(int *allocatedBlocks, int blockCount);

///////////////// FILE STRUCTURE //////////////////
/**
 * File structure representing a single file within the file system.
 * - name: The name of the file (up to 10 characters).
 * - headBlock: The first block in the file鈥檚 block chain.
 * - tailBlock: The last block in the file鈥檚 block chain.
 * - blockNum: The total number of blocks currently allocated to the file.
 */
struct File {
    explicit File() : name(), headBlock(-1), tailBlock(-1), blockNum(0) {
        name[0] = '\0';
    }

    char name[MAX_FILE_NAME];
    int headBlock;
    int tailBlock;
    int blockNum;

    // TODO: If you want to use `cout<<file` to output a File object, you should be able to do so with the following code:
    friend ostream &operator<<(ostream &os, const File &file) {
        //......
        return os;
    }
};

int totalBlocks;             // Total blocks in the file system
int freeBlockCnt;            // Count of free blocks currently available
unsigned char bitmap[MAX_BLOCKS]; // Bitmap to manage block allocation
int blockLinkedList[MAX_BLOCKS];  // Array to represent the block links (linked list)
File fileList[MAX_FILES];    // Array to store all file structures
int fileCount = 0;           // Current number of files in the file system

void initFileSys(int n){
    totalBlocks=n;
    if(totalBlocks>MAX_BLOCKS)totalBlocks=MAX_BLOCKS;
    freeBlockCnt=totalBlocks;
    memset(bitmap,0,sizeof(bitmap));
    memset(blockLinkedList, -1, sizeof(blockLinkedList));
    fileCount=0;
}

void displayStatus() {
    if (fileCount == 0) { // 如果没有文件
        cout << "EMPTY" << endl;
        return;
    }

    // 按照文件创建顺序遍历文件列表
    for (int i = 0; i < fileCount; ++i) {
        cout << fileList[i].name << " [";

        int currentBlock = fileList[i].headBlock; // 从头块开始遍历块链
        if (currentBlock == -1) {
            cout << "-1]"<<endl;; // 如果文件没有分配块
            continue;
        } else {
            while (currentBlock != -1) {
                cout << currentBlock; // 输出块索引
                currentBlock = blockLinkedList[currentBlock]; // 移动到下一个块
                if (currentBlock != -1) {
                    cout << " "; // 块索引之间加空格
                }
            }
        }

        cout << " -1]" << endl; // 每个文件的块链以 -1 结束
    }
}
// 分配空闲块
bool allocateFreeBlocks(int *allocatedBlocks, int blockCount) {
    if(blockCount==0)return true;
    int count = 0;
    for (int i = 0; i < totalBlocks && count < blockCount; ++i) {
        if (bitmap[i] == 0) { // 如果块空闲
            allocatedBlocks[count++] = i; // 分配块
            bitmap[i] = 1;                // 更新为已占用
            --freeBlockCnt;               // 减少空闲块数
        }
    }
    return count == blockCount; // 是否分配完成
}

int findFile(const char *filename) {
    for (int i = 0; i < fileCount; ++i) {
        if (strcmp(fileList[i].name, filename) == 0) {
            return i;
        }
    }
    return -1; // 未找到文件
}

void deleteFile(const char *filename) {
    int index = findFile(filename);
    if (index == -1) {
        cout << filename << " NOT FOUND" << endl;
        return;
    }

    int currentBlock = fileList[index].headBlock;
    while (currentBlock != -1) {
        int nextBlock = blockLinkedList[currentBlock];
        bitmap[currentBlock] = 0; // 更新为未占用
        blockLinkedList[currentBlock] = -1;
        ++freeBlockCnt;
        currentBlock = nextBlock;
    }

    // 删除文件列表中的文件
    for (int i = index; i < fileCount - 1; ++i) {
        fileList[i] = fileList[i + 1];
    }
    --fileCount;
}

void appendBlocks(const char *filename, int blockCount) {
    int index = findFile(filename);
    if (index == -1) {
        cout << filename << " NOT FOUND" << endl;
        return;
    }

    if (blockCount > freeBlockCnt) {
        cout << filename << " INSUFFICIENT BLOCKS" << endl;
        return;
    }

    int allocatedBlocks[blockCount];
    if (!allocateFreeBlocks(allocatedBlocks, blockCount)) {
        cout << filename << " INSUFFICIENT BLOCKS" << endl;
        return;
    }
    if(blockCount<=0)return;

    File &currentFile=fileList[index];
    if(currentFile.blockNum==0){
        //这里实现操作
        currentFile.headBlock = allocatedBlocks[0]; // 设置第一个块为头块
        currentFile.tailBlock = allocatedBlocks[0]; // 同时设置为尾块
        //这里只要把第一个变成头和尾就好了，请完成这个逻辑
        for (int i = 1; i < blockCount; ++i) {
            blockLinkedList[currentFile.tailBlock] = allocatedBlocks[i];// 下一个的存储位置
            currentFile.tailBlock = allocatedBlocks[i];
        }
        blockLinkedList[currentFile.tailBlock] = -1;
        currentFile.blockNum += blockCount;

        return;
    }
    for (int i = 0; i < blockCount; ++i) {
        blockLinkedList[currentFile.tailBlock] = allocatedBlocks[i];// 下一个的存储位置
        currentFile.tailBlock = allocatedBlocks[i];
    }
    blockLinkedList[currentFile.tailBlock] = -1;
    currentFile.blockNum += blockCount;
}
void defragment() {
    int compactIndex = 0; // 紧凑块的目标位置

    // 创建新的位图
    unsigned char newBitmap[MAX_BLOCKS] = {0};

    // 遍历文件列表，按照文件创建顺序整理
    for (int i = 0; i < fileCount; ++i) {
        int currentBlockCount = fileList[i].blockNum; // 当前文件的块数

        // 如果文件有块，更新头块和尾块
        if (currentBlockCount > 0) {
            fileList[i].headBlock = compactIndex; // 头块从 compactIndex 开始
            fileList[i].tailBlock = compactIndex + currentBlockCount - 1; // 尾块为头块加块数减1
        }

        // 为文件分配连续块
        for (int j = 0; j < currentBlockCount; ++j) {
            newBitmap[compactIndex] = 1; // 标记为已用
            blockLinkedList[compactIndex] = (j == currentBlockCount - 1) ? -1 : compactIndex + 1; // 最后一块指向 -1
            ++compactIndex; // 移动到下一个紧凑块
        }
    }

    // 清空未使用的区域（剩余部分标记为空闲）
    for (int i = compactIndex; i < totalBlocks; ++i) {
        newBitmap[i] = 0;            // 标记为空闲
        blockLinkedList[i] = -1; // 清空链表
    }

    // 替换位图
    memcpy(bitmap, newBitmap, sizeof(newBitmap));

    // 更新空闲块数量
    freeBlockCnt = totalBlocks - compactIndex;
}

void createFile(const char *filename, int blockCount) {
    if (findFile(filename) != -1) {
        cout << filename << " EXISTED" << endl;
        return;
    }

    if (blockCount > freeBlockCnt) {
        cout << filename << " INSUFFICIENT BLOCKS" << endl;
        return;
    }

    int allocatedBlocks[blockCount];
    if (!allocateFreeBlocks(allocatedBlocks, blockCount)) {
        cout << filename << " INSUFFICIENT BLOCKS" << endl;
        return;
    }
    if(blockCount==0){
        File newFile;
        strcpy(newFile.name, filename);
        newFile.headBlock = -1;
        newFile.tailBlock = -1;
        newFile.blockNum = 0;

        fileList[fileCount++] = newFile;
        return;
    }
    File newFile;
    strcpy(newFile.name, filename);
    newFile.headBlock = allocatedBlocks[0];
    newFile.tailBlock = allocatedBlocks[blockCount - 1];
    newFile.blockNum = blockCount;

    // 连接块链
    for (int i = 0; i < blockCount - 1; ++i) {
        blockLinkedList[allocatedBlocks[i]] = allocatedBlocks[i + 1];
    }
    blockLinkedList[allocatedBlocks[blockCount - 1]] = -1;

    fileList[fileCount++] = newFile; // 加入文件列表
}
int main() {
    // TODO: Implement the main logic of the file system
    string command; // 用于存储输入命令

    while (cin >> command) { // 持续读取输入直到 QUIT
        if (command == "INIT") {
            int n;
            cin >> n;
            initFileSys(n); // 初始化文件系统
        } else if (command == "CREATE") {
            string filename;
            int blockCount;
            cin >> filename >> blockCount;
            createFile(filename.c_str(), blockCount); // 创建文件
        } else if (command == "DELETE") {
            string filename;
            cin >> filename;
            deleteFile(filename.c_str()); // 删除文件
        } else if (command == "APPEND") {
            string filename;
            int blockCount;
            cin >> filename >> blockCount;
            appendBlocks(filename.c_str(), blockCount); // 追加块
        } else if (command == "STATUS") {
            displayStatus(); // 显示文件系统状态
        }else if(command=="DEFRAG"){
            defragment();
        } else if (command == "QUIT") {
            break; // 退出程序
        }
    }
    return 0;
}

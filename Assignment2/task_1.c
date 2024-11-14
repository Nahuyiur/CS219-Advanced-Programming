#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Node Node;
/*
The parameters in the following two structures are enough to finish the assignment. However, If adding some parameters to these two structures can be helpful, you can also do that.
*/
typedef struct Node {
    void* data;           // pointer to node data
    size_t byte_num;     // The size of the data in bytes    
    Node* next;    // pointer to next node
} Node;

// Definition of linked list node structure
typedef struct MagicLinkedList {
    Node* head;           // head node
    Node* tail;           // tail node
} MagicLinkedList;

/* Your code starts here */
// Save every three octal string into bytes
void octal2byte(void *dst, char *octal) {
    // 计算八进制字符串的长度，包括字符串末尾的空字符
    size_t len = strlen(octal);

    // 计算需要多少字节来存储这些八进制数字
    size_t dst_len = (len - 2) / 3; // 减去前缀"0o"

    // 如果八进制字符串长度不是3的倍数，需要补零
    if ((len - 2) % 3 != 0) {
        dst_len += 1;
    }

    // 分配内存来存储转换后的字节序列
    char* _dst = (char*)malloc(dst_len);
    if (_dst == NULL) {
        // 如果内存分配失败，立即返回
        return;
    }

    // 将八进制数字转换为字节
    for (size_t i = 2, j = 0; i < len; i += 3, j++) {
        // 每个字节由3个八进制数字组成
        int byte = 0;
        int count = 0;

        // 处理每个八进制数字
        for (int k = 0; k < 3 && i + k < len; k++, count++) {
            int digit = octal[i + k] - '0';
            if (digit < 0 || digit > 7) {
                // 如果数字不是有效的八进制数字，释放内存并返回
                free(_dst);
                return;
            }

            // 将八进制数字转换为二进制并存储到字节中
            if (count == 0) {
                byte |= digit << 5; // 第一个数字左移5位
            } else if (count == 1) {
                byte |= digit << 2; // 第二个数字左移2位
            } else {
                byte |= digit >> 1; // 第三个数字右移1位
            }
        }

        // 如果八进制字符串长度不是3的倍数，补零
        if (count < 3) {
            byte <<= (3 - count) * 2; // 根据缺失的数字数量左移
        }

        _dst[j] = (char)byte;
    }

    // 将转换后的字节序列复制到提供的目的地
    memcpy(dst, _dst, dst_len);

    // 释放动态分配的内存
    free(_dst);
}
void releaseNode(Node* node){
    free(node->data);
    node->data=NULL;
    free(node);
    return;
}
// Create a new linked list
MagicLinkedList* create() {
    // Allocate memory for the linked list
    MagicLinkedList* list=(MagicLinkedList*)malloc(sizeof(MagicLinkedList));

    list->head=NULL;
    list->tail=NULL;
    return list; 
}

// Append a new node to the end of the linked list
void append_node(MagicLinkedList* list, void* data) {
    if(list==NULL)return;

    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data=NULL;

    int len=strlen((char*)data)+1;
    int byte_count=(len-1)/3;
    new_node->data=malloc(new_node->byte_num);
    octal2byte(new_node->data,(char*)data);

    new_node->byte_num=byte_count;
    new_node->next=NULL;

    if(list->head==NULL){
        list->head=new_node;
        list->tail=new_node;
    }else{
        list->tail->next=new_node;
        list->tail=new_node;
    }


    // Allocate memory for a new node and set its data and size
    // Link the new node to the end of the linked list
    // TODO: Implement this function
}

// Insert a new node at a specific position in the linked list
void insert_node(MagicLinkedList* list, int position, void* data) {
    if(list==NULL)return;
    if(position<0)return;

    Node* new_node=(Node*)malloc(sizeof(Node));
    new_node->data=NULL;

    int len=strlen((char*)data)+1;
    int byte_count=(len-1)/3;
    new_node->data=malloc(new_node->byte_num);
    octal2byte(new_node->data,(char*)data);

    new_node->byte_num=byte_count;
    new_node->next=NULL;

    
    Node* current=list->head; // 找到前一个node
    for (int i = 1; i < position; i++)
    {
        if(current==NULL){
            releaseNode(current);
            return;
        }
        current=current->next;
    }
    
    if(position==0){
        list->head=new_node;
        if(current==NULL){
            list->tail=new_node;
        }else{
            new_node->next=current;
        }
    }else{
        if(current==NULL){
            releaseNode(current);
        }else{
            if(current->next!=NULL){
                new_node->next=current->next;
            }else{
                list->tail=new_node;
            }
        }
    }
    return;
}

// Delete a node from the linked list at a specific position
void delete_node(MagicLinkedList* list, int position) {
    // 检查链表是否为空或位置无效
    if (list == NULL || position < 0) return;

    Node* current = list->head;
    Node* previous = NULL;

    // 遍历找到要删除的节点
    for (int i = 0; i < position; i++) {
        if (current == NULL) return; // 如果位置超出范围则退出
        previous = current;
        current = current->next;
    }

    // 如果节点为空，则直接返回
    if (current == NULL) return;

    // 如果要删除的是头节点
    if (position == 0) {
        list->head = current->next; // 更新头节点
        if (list->head == NULL) {    // 如果链表变为空
            list->tail = NULL;       // 更新尾节点为NULL
        }
    } else {
        // 如果要删除的是非头节点
        previous->next = current->next; // 将前驱节点指向下一个节点
        if (current->next == NULL) {     // 如果要删除的是尾节点
            list->tail = previous;       // 更新尾节点
        }
    }

    // 释放节点内存
    releaseNode(current);
}


// Destroy the linked list and free all allocated memory
void destroy(MagicLinkedList* list) {
    // Free all nodes in the list and the list itself
    // TODO: Implement this function
    if(list==NULL)return;

    Node* current=list->head;
    while(current!=NULL){
        Node* next=current->next;
        releaseNode(current);
    }

    free(list);
    return;
}

void printx(MagicLinkedList *LinkedList){
    // TODO: print the node's data with its corresponding ASCII character from head node to tail node. Print each node in one line.
    /*
    Note:
    For each node, print every 1 byte (char) in [33, 127) as a character, if data is not within the domain, use '#' instead. You should print a node in one line.
    */
    if(LinkedList==NULL)return;

    Node* current=LinkedList->head;
    while(current!=NULL){
        for (size_t i = 0; i < current->byte_num; i++)
        {
            unsigned char byte_value=((unsigned char*)current->data)[i];

            if(byte_value>=33&&byte_value<127){
                printf("%c",byte_value);
            }else{
                if(byte_value!=0)printf("#");
            }
        }
        printf("\n");
        current=current->next;
    }
    return;
}

void printx_reverse_helper(Node* node) {
    if (node == NULL) return;
    printx_reverse_helper(node->next); // 先递归到最后一个节点
    for (size_t i = 0; i < node->byte_num; i++) {
        unsigned char byte_value = ((unsigned char*)node->data)[i];
        if (byte_value >= 33 && byte_value < 127) {
            printf("%c", byte_value);
        } else {
            if(byte_value!=0)printf("#");
        }
    }
    printf("\n");
}
void printx_reverse(MagicLinkedList *LinkedList){
    // TODO: Similar with printx, only reverse the order of nodes, the values inside the nodes are not reversed.
    if (LinkedList == NULL) return;
    printx_reverse_helper(LinkedList->head);
}

// Do not make any modifications to this main function
int main() {
    int T;
    scanf("%d", &T);
    MagicLinkedList *m_list = NULL;

    while (T--) {
        char op;
        scanf(" %c", &op);
        switch (op) {
        case 'C': {
            if (m_list != NULL) {
                destroy(m_list);
            }
            m_list = create();
            break;
        }
        case 'A': {
            int len;
            scanf("%d", &len);
            char *octal_str = (char *)malloc(len + 1);
            scanf("%s", octal_str);
            append_node(m_list, octal_str);
            free(octal_str);
            break;
        }
        case 'I': {
            int index;
            scanf("%d", &index);
            int len;
            scanf("%d", &len);
            char *octal_str = (char *)malloc(len + 1);
            scanf("%s", octal_str);
            insert_node(m_list, index, octal_str);
            free(octal_str);
            break;
        }
        case 'D': {
            int item_id;
            scanf("%d", &item_id);
            delete_node(m_list, item_id);
            break;
        }
        case 'X': {
            destroy(m_list);
            m_list = NULL;
            break;
        }
        case 'Q': {
            printx(m_list);
            break;
        }
        case 'R': {
            printx_reverse(m_list);
            break;
        }
        default: {
            break;
        }
        }
    }

    if (m_list != NULL) {
        destroy(m_list);
    }
    return 0;
}
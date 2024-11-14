#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Node Node;
/*
The parameters in the following two structures are enough to finish the assignment. 
However, If adding some parameters to these two structures can be helpful, you can also do that.
*/
typedef struct Node {
    void* data;           // pointer to node data
    size_t byte_num;     // The size of the data in bytes    
    Node* next;    // pointer to next node
    Node* p;
} Node;

// Definition of linked list node structure
typedef struct MagicLinkedList {
    Node* head;           // head node
    Node* tail;           // tail node
} MagicLinkedList;

/* Your code starts here */

// Save every three octal string into bytes
void octal2byte(void *dst, char *octal) {
    /*
    Function: For the given octal string "0o123321", write integer equaling binary representation '00101001 01101000' into dst. Save each byte into dst.

    Notes:
    - The length of octal might be ODD, make sure the each octal number can be grouped in a byte. 
    For example, "0o66777" should be stored as 0o066777 by simply padding 0 here).
    - You should return immediately if any failure happens.
    */
    size_t len = strlen(octal) + 1;
    
    size_t dst_len = len / 3 - (len % 3 == 0);
    int h = 3 - (len % 3), dh = 0;
    char* _dst = (char*)malloc(dst_len);
    char a;
    if (h == 1){
        a  = '\0';
        a |= (octal[2] - '0') << 2;
        a |= (octal[3] - '0') >> 1;
        _dst[0] = a;
        dh = 2;
    } else if (h == 2){
        a  = '\0';
        a |= (octal[2] - '0') >> 1;
        _dst[0] = a;
        dh = 1;
    }
    for (size_t i=(h!=3), j=2+dh; i<dst_len; i++, j+=3){
        a  = '\0';
        a |= (octal[j]     - '0') << 5;
        a |= (octal[j + 1] - '0') << 2;
        a |= (octal[j + 2] - '0') >> 1;
        _dst[i] = a;
    }
    memcpy(dst, _dst, dst_len);
    free(_dst);
    return;
}

// Create a new_ linked list
MagicLinkedList* create() {
    // Allocate memory for the linked list
    MagicLinkedList* ml = (MagicLinkedList*)malloc(sizeof(MagicLinkedList));
    ml->head = NULL;
    ml->tail = NULL;
    return ml;
}

// Append a new_ node to the end of the linked list
void append_node(MagicLinkedList* list, void* data) {
    // Allocate memory for a new_ node and set its data and size
    // Link the new_ node to the end of the linked list
    if (list == NULL) {
        return;
    }

    Node* new_ = (Node*)malloc(sizeof(Node));
    new_->data = NULL;

    size_t len = strlen((char*)data) + 1;
    new_->byte_num = len / 3 - (len % 3 == 0);
    new_->data = malloc(new_->byte_num);
    octal2byte(new_->data, (char*)data);
    new_->next = NULL;
    new_->p = NULL;

    if (list->tail == NULL){
        list->head = new_;
        list->tail = new_;
    } else {
        list->tail->next = new_;
        new_->p = list->tail;
        list->tail = new_;
    }
    return;
}

// Insert a new_ node at a specific position in the linked list
void insert_node(MagicLinkedList* list, int position, void* data) {
    // Allocate memory for a new_ node and set its data and size
    // Insert the node at the specified position in the linked list 
    // position is non-negative and start from 0. If position is invalid or out of bound, don't operate on list.
    if (list == NULL || position < 0) {
        return;
    }

    Node* new_ = (Node*)malloc(sizeof(Node));
    new_->data = NULL;
    
    size_t len = strlen((char*)data) + 1;
    new_->byte_num = len / 3 - (len % 3 == 0);
    new_->data = malloc(new_->byte_num);
    octal2byte(new_->data, (char*)data);
    new_->next = NULL;
    new_->p = NULL;

    Node* h = list->head;
    for(int i=1; i<position; i++){
        if (h==NULL){
            free(new_->data);
            new_->data = NULL;
            free(new_);
            return;
        }
        h = h->next;
    }
    if (position == 0) {
        list->head = new_;
        if (h==NULL) {
            list->tail = new_;
        } else {
            h->p = new_;
            new_->next = h;
        }
    } else {
        if (h==NULL) {
            free(new_->data);
            new_->data = NULL;
            free(new_);
        } else {
            new_->p = h;
            if (h->next){
                new_->next = h->next;
                h->next->p = new_;
            } else {
                list->tail = new_;
            }
            h->next = new_;
        }
    }
    return;
}

void releaseN(Node* node){
    free(node->data);
    free(node);
    return;
}

// Delete a node from the linked list at a specific position
void delete_node(MagicLinkedList* list, int position) {
    // Remove the node at the specified position and free its memory. 
    // position is non-negative and start from 0. If position is invalid or out of bound, don't operate on list.
    if (list == NULL || position < 0) {
        return;
    }
    Node* ptr = list->head;
    for(int i=0; i<position; i++){
        if (ptr==NULL){
            return;
        }
        ptr = ptr->next;
    }
    if (ptr==NULL){
        return;
    }
    if (position == 0) {
        Node* N = ptr->next;
        if (N==NULL){
            list->head=NULL;
            list->tail=NULL;
        } else {
            N->p = NULL;
            list->head=N;
        }
    } else {
        Node* h = ptr->p;
        Node* n = ptr->next;
        if (n==NULL){
            h->next = NULL;
            list->tail = h;
        } else {
            h->next = n;
            n->p = h;
        }
    }
    releaseN(ptr);
    ptr = NULL;
    return;
}


// Destroy the linked list and free all allocated memory
void destroy(MagicLinkedList* list) {
    // Free all nodes in the list and the list itself
    if (list == NULL) {
        return;
    }
    Node* h = list->head;
    if (h) {
        while(h->next) {
            h = h->next;
            releaseN(h->p);
            h->p = NULL;
        }
        releaseN(h);
        h=NULL;
    }
    free(list);
}

void printN(Node* node){
    if (node->data == NULL){
        return;
    }
    char* _data = (char*)node->data;
    for (size_t i=0; i<node->byte_num; i++){
        if (_data[i] < 33 || _data[i] >= 127){
            printf("%c", '#');
        } else {
            printf("%c", _data[i]);
        }
    }
    printf("\n");
    return;
}

void printx(MagicLinkedList *LinkedList){
    /*
    Note:
    For each node, print every 1 byte (char) in [33, 127) as a character, if data is not within the domain, use '#' instead. You should print a node in one line.
    */
    if (LinkedList == NULL) {
        return;
    }
    Node* h = LinkedList->head;
    while(h) {
        printN(h);
        h = h->next;
    }
    return;
}

void printx_reverse(MagicLinkedList *LinkedList){
    if (LinkedList == NULL) {
        return;
    }
    Node* h = LinkedList->tail;
    while(h) {
        printN(h);
        h = h->p;
    }
    return;
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
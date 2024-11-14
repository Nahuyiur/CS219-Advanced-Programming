#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node Node;

typedef struct Node {
    void* data;        // pointer to node data
    size_t byte_num;   // size of the data in bytes    
    Node* next;        // pointer to the next node
    Node* prev;        // pointer to the previous node
} Node;

// Definition of linked list structure
typedef struct MagicLinkedList {
    Node* head;        // head node
    Node* tail;        // tail node
} MagicLinkedList;

/* Convert octal string to bytes */
void octal2byte(void *dst, char *octal) {
    size_t len = strlen(octal) + 1;
    size_t dst_len = len / 3 - (len % 3 == 0);
    int padding = 3 - (len % 3);
    char* byte_array = (char*)malloc(dst_len);
    char byte;

    if (padding == 1) {
        byte = '\0';
        byte |= (octal[2] - '0') << 2;
        byte |= (octal[3] - '0') >> 1;
        byte_array[0] = byte;
        octal += 2;
    } else if (padding == 2) {
        byte = '\0';
        byte |= (octal[2] - '0') >> 1;
        byte_array[0] = byte;
        octal += 1;
    }

    for (size_t i = (padding != 3), j = 2; i < dst_len; i++, j += 3) {
        byte = '\0';
        byte |= (octal[j] - '0') << 5;
        byte |= (octal[j + 1] - '0') << 2;
        byte |= (octal[j + 2] - '0') >> 1;
        byte_array[i] = byte;
    }

    memcpy(dst, byte_array, dst_len);
    free(byte_array);
    return;
}

/* Create a new linked list */
MagicLinkedList* create() {
    MagicLinkedList* list = (MagicLinkedList*)malloc(sizeof(MagicLinkedList));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

/* Append a node to the end of the linked list */
void append_node(MagicLinkedList* list, void* data) {
    if (list == NULL) return;

    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data=NULL;

    size_t len = strlen((char*)data) + 1;
    new_node->byte_num = len / 3 - (len % 3 == 0);
    new_node->data = malloc(new_node->byte_num);
    octal2byte(new_node->data, (char*)data);

    new_node->next = NULL;
    new_node->prev = NULL;

    if (list->tail == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        new_node->prev=list->tail;
        list->tail = new_node;
    }
    return;
}

/* Insert a node at a specified position in the linked list */
void insert_node(MagicLinkedList* list, int position, void* data) {
    if (list == NULL || position < 0) return;

    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data=NULL;

    size_t len = strlen((char*)data) + 1;
    new_node->byte_num = len / 3 - (len % 3 == 0);
    new_node->data = malloc(new_node->byte_num);
    octal2byte(new_node->data, (char*)data);
    new_node->next=NULL;
    new_node->prev=NULL;

    if (position == 0) {  
        new_node->next = list->head;
        new_node->prev = NULL;
        if (list->head) {
            list->head->prev = new_node;
        }
        list->head = new_node;
        if (list->tail == NULL) {
            list->tail = new_node;
        }
    } else {  
        Node* current = list->head;
        for (int i = 1; i < position && current != NULL; i++) {
            current = current->next;
        }

        if (current == NULL) {  
            free(new_node->data);
            free(new_node);
            return;
        }

        new_node->next = current->next;
        new_node->prev = current;

        if (current->next) {
            current->next->prev = new_node;
        } else {
            list->tail = new_node;
        }

        current->next = new_node;
    }
    return;
}

/* Release a single node */
void release_node(Node* node) {
    free(node->data);
    free(node);
    return;
}

/* Delete a node at a specific position */
void delete_node(MagicLinkedList* list, int position) {
    if (list == NULL || position < 0) return;

    Node* current = list->head;
    for (int i = 0; i < position && current != NULL; i++) {
        current = current->next;
    }

    if (current == NULL) return;

    if (current->prev) {
        current->prev->next = current->next;
    } else {
        list->head = current->next;
    }

    if (current->next) {
        current->next->prev = current->prev;
    } else {
        list->tail = current->prev;
    }

    release_node(current);
    current=NULL;
    return;
}

/* Destroy the entire linked list */
void destroy(MagicLinkedList* list) {
    if (list == NULL) return;

    Node* current = list->head;
    while (current != NULL) {
        Node* next_node = current->next;
        release_node(current);
        current = next_node;
    }
    free(list);
}

/* Print each node's data, replacing non-printable characters with '#' */
void print_node_data(Node* node) {
    if (node->data == NULL) return;

    char* data = (char*)node->data;
    for (size_t i = 0; i < node->byte_num; i++) {
        if (data[i] >= 33 && data[i] < 127) {
            printf("%c", data[i]);
        } else {
            printf("#");
        }
    }
    printf("\n");
    return;
}

/* Print the linked list from head to tail */
void printx(MagicLinkedList *list) {
    if (list == NULL) return;

    Node* current = list->head;
    while (current != NULL) {
        print_node_data(current);
        current = current->next;
    }
    return;
}

/* Print the linked list from tail to head */
void printx_reverse(MagicLinkedList *list) {
    if (list == NULL) return;

    Node* current = list->tail;
    while (current != NULL) {
        print_node_data(current);
        current = current->prev;
    }
    return;
}

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

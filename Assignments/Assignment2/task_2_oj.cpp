#include <iostream>
#include <stdexcept>

using namespace std;
struct ChildNode;
struct OperationNode;

struct ChildNode {
    float value;                
    OperationNode* parent;     

    ChildNode(float val) : value(val), parent(nullptr) {}
};

struct OperationNode {
    char op;                   
    ChildNode* left;          
    ChildNode* right;         

    OperationNode(char operation) 
        : op(operation), left(nullptr), right(nullptr) {}

    ~OperationNode();
    
    void setLeft(ChildNode* leftChild);

    void setRight(ChildNode* rightChild);

    float compute() const;

    void print() const;

    OperationNode* deepCopy() const;

    OperationNode* shallowCopy() const;
};
//析构函数,结束时释放内存
OperationNode::~OperationNode() {
    if (left && left->parent == this) delete left;  
    if (right && right->parent == this) delete right; 
}
//PREPRND END

// TEMPLATE BEGIN

// Deletes an existing left node and updates the value and pointer
void OperationNode::setLeft(ChildNode* leftChild) {
    if(leftChild==nullptr)return;

    if(this->left!=nullptr){
        delete this->left;
    }

    this->left=leftChild;
    leftChild->parent=this;
}

// Deletes an existing right node and updates the value and pointer
void OperationNode::setRight(ChildNode* rightChild) {
    if(rightChild==nullptr)return;

    if(this->right!=nullptr){
        delete this->right;
    }
    
    this->right=rightChild;
    rightChild->parent=this;
}

// Calculate the result of this computational tree
float OperationNode::compute() const {
    float ans=NULL;
    if(this->op=='\0'||this->left==nullptr||this->right==nullptr)return ans;
    
    char op=this->op;
    float left_value=this->left->value;
    float right_value=this->right->value;
    switch (op)
    {
    case '+':
        ans=left_value+right_value;
        break;
    case '-':
        ans=left_value-right_value;
        break;
    case '*':
        ans=left_value*right_value;
        break;
    case '/':
        if(right_value==0){
            ans=NULL;
        }else{
            ans=left_value/right_value;
        }
        break;
    default:
        break;
    }
    return ans;
}

// Print the formula according to the fixed format: Parent('*'): 10.5 * 2 = 21

void OperationNode::print() const {
    if(this->op=='\0'||this->left==nullptr||this->right==nullptr)return;

    char op=this->op;
    float left_value=this->left->value;
    float right_value=this->right->value;

    float ans=this->compute();
    if(ans==NULL)return;
    
    cout<< "Parent(" << op << "): " <<left_value<<" "<<op<<" "<<right_value<<" "<<"="<<" ";
    cout<<ans<<endl;
}

//Deep copy of both left and right child nodes
OperationNode* OperationNode::deepCopy() const {
    OperationNode* copy = new OperationNode(op);
    ChildNode* leftCopy = new ChildNode(left->value);
    ChildNode* rightCopy = new ChildNode(right->value);
    copy->left=leftCopy;
    copy->right=rightCopy;

    leftCopy->parent=copy;
    rightCopy->parent=copy;
    
    return copy;
}
//Share the original left and right child nodes
OperationNode* OperationNode::shallowCopy() const {
    OperationNode* copy = new OperationNode(op);

    copy->left=this->left;
    
    copy->right=this->right;

    return copy;
}
// TEMPLATE END
//APPEND BEGIN
void testCalculationTree() {
    OperationNode root('*');
 
    ChildNode* leftChild = new ChildNode(5.5f);   
    ChildNode* rightChild = new ChildNode(3.2f); 
    root.setLeft(leftChild);
    root.setRight(rightChild);

    root.print();  // Parent(*): 5.5 * 3.2 = 17.76

    OperationNode* deepCopy = root.deepCopy();
    root.setLeft(new ChildNode(10.5f));
    root.setRight(new ChildNode(2.0f));

    cout << "Deep Copy Result: ";
    deepCopy->print();  // Parent(*): 5.5 * 3.2 = 17.76
    cout << "Modified Original Result: ";
    root.print();       // Parent(*): 10.5 * 2 = 21

    OperationNode* shallowCopy = root.shallowCopy();
    root.left->value = 1.0f;
	root.right->value = 2.0f;
    cout << "Shallow Copy Result: ";

    shallowCopy->print(); // // Parent(*): 1 * 2 = 2

    delete deepCopy;     
    delete shallowCopy;  
}
int main()
{
    int times;
    cin >> times;

    char root_op;
    cin >> root_op;

    OperationNode root(root_op);

    ChildNode *leftChild = nullptr;
    ChildNode *rightChild = nullptr;
    OperationNode *shallowCopy = nullptr;
    OperationNode *deepCopy = nullptr;
    while (times--)
    {
        int operation;
        cin >> operation;
        switch (operation)
        {
        case 1:
            float left;
            cin >> left;
            leftChild = new ChildNode(left);
            root.setLeft(leftChild);
            break;
        case 2:
            float right;
            cin >> right;
            rightChild = new ChildNode(right);
            root.setRight(rightChild);
            break;
        case 3:
            shallowCopy = root.shallowCopy();
            float shallow_left, shallow_right;
            cin >> shallow_left;
            cin >> shallow_right;
            leftChild->value = shallow_left;
            rightChild->value = shallow_right;

            break;
        case 4:
            deepCopy = root.deepCopy();
            float deep_left, deep_right;
            cin >> deep_left;
            cin >> deep_right;
            root.setLeft(new ChildNode(deep_left));
            root.setRight(new ChildNode(deep_right));
            break;
        case 5:
            root.print();
            break;
        case 6:
            cout << root.compute();
            break;
        case 7:
            cout << leftChild->value << leftChild->parent->op;
            break;
        case 8:
            cout << leftChild->parent->left->value;
            break;
        case 9:
            cout << rightChild->value << rightChild->parent->op;
            break;
        case 10:
            cout << rightChild->parent->right->value;
            break;
        case 11:
            shallowCopy->print();
            break;
        case 12:
            deepCopy->print();
            break;
        default:
            break;
        }
    }

    return 0;
}
//APPEND END
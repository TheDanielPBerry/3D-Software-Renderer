#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

const int N = 400000;
const int D = 2;
double P[N][D];	//P[i][j] is jth coordinate of the ith point

struct Node {
	int index;
	int val = 0;
	int size = 1;
	Node* left = nullptr;
	Node* right = nullptr;
};


void update_size(Node* root)
{
	root->size = 1;
	if(root->left != nullptr) {
		root->size += root->left->size;
	}
	if(root->right != nullptr) {
		root->size += root->right->size;
	}
}


Node* insert(Node* root, int index, int dim)
{
	if(root == nullptr) {
		Node* n = new Node;
		n->index = index;
		return n;
	}
	if(index == root->index) {
		return root;
	}

	if(P[index][dim] < P[root->index][dim]) {
		root->left = insert(root->left, index, (dim+1)%D);
	} else {
		root->right = insert(root->right, index, (dim+1)%D);
	}
	update_size(root);
	return root;
}


pair<Node*, Node*> split(Node* root, int index)
{
	if(root == nullptr) {
		return make_pair(nullptr, nullptr);
	}

	if(root->index > index) {
		pair<Node *, Node *> result = split(root->left, index);
		root->left = result.second;
		update_size(root);
		return make_pair(result.first, root);
	}

	pair<Node *, Node *> result = split(root->right, index);
	root->right = result.first;
	update_size(root);
	return make_pair(root, result.second);
}


Node* join(Node* left, Node* right)
{
	if(left == nullptr) return right;
	if(right == nullptr) return left;

	uint total = left->size + right->size;
	if((rand()%total) < left->size) {
		left->right = join(left->right, right);
		update_size(left);
		return left;
	}
	right->left = join(left, right->left);
	update_size(right);
	return right;
}


Node* insert_balanced(Node* root, int index)
{
	if(root == nullptr) {
		Node* n = new Node;
		n->index = index;
		return n;
	}
	if(index == root->index) {
		return root;
	}

	//Probability 1/n
	if((rand() % (root->size+1)) == 0) {
		Node *L, *R;
		tie(L, R) = split(root, index);
		Node* n = new Node;
		n->right = R;
		update_size(n);
		return n;
	}

	if(index < root->index) {
		root->left = insert_balanced(root->left, index);
	} else {
		root->right = insert_balanced(root->right, index);
	}
	update_size(root);
	return root;
}


Node* remove(Node* root, int index)
{
	if(root == nullptr) return nullptr;
	if(root->index == index) {
		//Remove
		Node* n = join(root->left, root->right);
		delete root;
		return n;
	}

	if(index < root->index) {
		root->left = remove(root->left, index);
	} else {
		root->right = remove(root->right, index);
	}
	update_size(root);

	return root;
}

Node* find(Node* root, int index)
{
	if(root == nullptr) {
		return nullptr;
	}
	if(index == root->index) return root;
	if(index < root->index) {
		return find(root->left, index);
	}
	return find(root->right, index);
}

void print_inorder(Node* root)
{
	if(root != nullptr) {
		print_inorder(root->left);
		cout << P[root->index] << endl;
		print_inorder(root->right);
	}
}

class VirtualArray {
private:
	Node* root = nullptr;
public:
	int &operator[] (int key) {
		Node* n = find(root, key);
		if(n == nullptr) {
			root = insert(root, key, 0);
		}
		return find(root, key)->val;
	}
};

struct Box {
	vector<double> low, high;
};

bool point_in_box(int index, Box &b)
{
	for(int j=0; j<D; j++) {
		if(P[index][j] < b.low[j] || P[index][j] > b.high[j]) {
			return false;
		}
	}
	return true;
}

bool boxes_overlap(Box &b1, Box &b2)
{
	for(int j=0; j<D; j++) {
		if(b1.high[j] < b2.low[j] || b1.low[j] > b2.high[j]) {
			return false;
		}
	}
	return true;
}


bool box_contains(Box &big, Box &small)
{
	for(int j=0; j<D; j++) {
		if(small.low[j] < big.low[j] || small.high[j] > big.high[j]) {
			return false;
		}
	}
	return true;
}

int rq_count(Node *root, Box &qbox, Box &bbox, int dim)
{
	if(root == nullptr) {
		return 0;
	}

	int n = 0;
	if(point_in_box(root->index, qbox)) {
		n = 1;
	}
	if(!boxes_overlap(bbox, qbox)) {
		return 0;
	}
	if(box_contains(qbox, bbox)) {
		return root->size;
	}

	double high = bbox.high[dim];
	bbox.high[dim] = P[root->index][dim];
	n += rq_count(root->left, qbox, bbox, (dim+1)%D);
	bbox.high[dim] = high;

	double low = bbox.low[dim];
	bbox.low[dim] = P[root->index][dim];
	n += rq_count(root->right, qbox, bbox, (dim+1)%D);
	bbox.low[dim] = low;
	return n;
}

int main(void)
{
	Node *root = nullptr;
	for(int i=0; i<N; i++) {
		for(int j=0; j<D; j++) {
			P[i][j] = drand48();
		}
		root = insert(root, i, 0);
	}
	//print_inorder(root);

	vector<double> low(D, 0.25), high(D, 0.75);
	Box qbox = {low, high};

	vector<double> zeroes(D, 0), ones(D, 1.0);
	Box bbox = {zeroes, ones};

	for(int k=0; k<200; k++) {
		rq_count(root, qbox, bbox, 0);
	}
	cout << "Fast is done" << endl;
}


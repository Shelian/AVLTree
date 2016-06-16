#ifndef __BALANCETREE_H__
#define __BALANCETREE_H__

template<class K,class V>
struct BLNode
{
	K _key;
	V _value;
	BLNode<K, V>* _left;
	BLNode<K, V>* _right;
	BLNode<K, V>* _parent;
	int _bf;

	BLNode(const K& key, const V& value)
		:_key(key)
		, _value(value)
		, _parent(NULL)
		, _left(NULL)
		, _right(NULL)
		, _bf(0)
	{}

};

template<class K,class V>
class BalanceTree
{
	typedef BLNode<K, V> Node;

public:
	BalanceTree()
		:_root(NULL)
	{}

	bool Insert(const K& key, const V& value)
	{
		//先查找需要插入的位置
		if (_root == NULL)
		{
			_root = new Node(key, value);
			return true;
		}

		Node* parent = NULL;
		Node* cur = _root;
		while (cur)
		{
			if (cur->_key > key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (cur->_key < key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				return false;
			}
		}

		//找到插入位置以后,确定平衡因子
		Node* ppNode = parent->_parent;
		

		if (parent->_key > key)
		{
			cur = new Node(key, value);
			parent->_left = cur;
			cur->_parent = parent;
			parent->_bf--;
			while (parent)
			{
				if ((parent->_bf == -1 ||parent->_bf ==1)&& parent->_parent != NULL)
				{
					if (parent->_parent->_key >parent->_key)
						parent->_parent->_bf--;
					else if (parent->_parent->_key < parent->_key)
						parent->_parent->_bf++;
					if (parent->_parent->_bf == 2 || parent->_parent->_bf == -2)
						break;
					cur = parent;
					parent = parent->_parent;
				}
				else
					break;
			}
			
		}

		else if (parent->_key < key)
		{
			cur = new Node(key, value);
			parent->_right = cur;
			cur->_parent = parent;
			parent->_bf++;
			while (parent)
			{
				if ((parent->_bf == 1 ||parent->_bf == -1)&& parent->_parent != NULL)
				{
					if (parent->_parent->_key >parent->_key)
						parent->_parent->_bf--;
					else if (parent->_parent->_key < parent->_key)
						parent->_parent->_bf++;
					if (parent->_parent->_bf == 2 || parent->_parent->_bf == -2)
						break;
					cur = parent;
					parent = parent->_parent;
				}
				else
					break;
			}
		}

		//根据平衡因子的变动，开始调整当前的树
		ppNode = parent->_parent;
		Node* pppNode = NULL;
		if (ppNode!=NULL)
			pppNode = ppNode->_parent;
		if (ppNode && (ppNode->_bf == 2 || ppNode->_bf == -2))
		{
			//左旋
			if (parent == ppNode->_right && parent->_right == cur)
			{
				RotateL(ppNode);
			}

			//右旋
			else if (ppNode->_left == parent && parent->_left == cur)
			{
				RotateR(ppNode);
			}

			//左右双旋
			else if (ppNode->_left == parent &&parent->_right == cur)
			{
				int bf = cur->_bf;
				Node* pNode = ppNode;
				Node* pLeftNode = parent;
				RotateL(parent);
				ppNode->_left = parent;
				RotateR(ppNode);

				if (bf == 0)
				{
					pNode->_bf = 0;
					pLeftNode->_bf = 0;
				}
				else if (bf == -1)
				{
					pNode->_bf = 1;
					pLeftNode->_bf = 0;
				}
				else if (bf == 1)
				{
					pNode->_bf = 0;
					pLeftNode->_bf = -1;
				}
			}

			//右左双旋
			else if (ppNode->_right == parent &&parent->_left == cur)
			{
				int bf = cur->_bf;
				Node* pNode = ppNode;
				Node* pRightNode = parent;
				RotateR(parent);
				ppNode->_right = parent;
				RotateL(ppNode);

				if (bf == 0)
				{
					pNode->_bf = 0;
					pRightNode->_bf = 0;
				}
				else if (bf == -1)
				{
					pNode->_bf = 0;
					pRightNode->_bf = 1;
				}
				else if (bf == 1)
				{
					pNode->_bf = -1;
					pRightNode->_bf = 0;
				}
			}

			if (pppNode == NULL)
			{
				_root = ppNode;
			}
			else
			{
				if (pppNode->_key > ppNode->_key)
					pppNode->_left = ppNode;
				else if (pppNode->_key < ppNode->_key)
					pppNode->_right = ppNode;
			}

		}
	}


	size_t Height()
	{
		return _Height(_root);
	}
	
	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

protected:
	size_t _Height(Node* root)
	{
		if (root == NULL)
			return 0;

		return _Height(root->_left) > _Height(root->_right) ? _Height(root->_left) + 1 : _Height(root->_right) + 1;
	}
	void _InOrder(Node* root)
	{
		if (root == NULL)
			return;

		_InOrder(root->_left);
		cout << root->_key << " ";
		_InOrder(root->_right);
	}

	void RotateL(Node*& parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		parent->_right = subRL;
		if (subRL != NULL)
			subRL->_parent = parent;

		subR->_parent = parent->_parent;
		parent->_parent = subR;
		subR->_left = parent;

		parent->_bf = 0;
		subR->_bf = 0;

		parent = subR;
	}

	void RotateR(Node*& parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		parent->_left = subLR;
		if (subLR != NULL)
			subLR->_parent = parent;

		subL->_parent = parent->_parent;
		subL->_right = parent;
		parent->_parent = subL;
		parent->_bf = 0;
		subL->_bf = 0;

		parent = subL;
	}



protected:
	Node* _root;
};




void test()
{
	BalanceTree<int, int> bt;
	int arr[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		bt.Insert(arr[i], i);
	}

	bt.InOrder();
	cout << bt.Height() << endl;

	BalanceTree<int, int> bt1;
	int arr1[] = { 4,2,6,1,3,5,15,7,16,14 };
	for (int i = 0; i < sizeof(arr1) / sizeof(arr1[0]); i++)
	{
		bt1.Insert(arr1[i], i);
	}

	bt1.InOrder();
	cout << bt1.Height() << endl;
}


#endif //__BALANCETREE_H__
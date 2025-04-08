#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>

using namespace std;

struct Lnode
{
	int x;
	Lnode* next;
	Lnode(std::initializer_list<int> list = {})
		:next(nullptr), x(0)
	{
		Lnode* tail = this;
		for (auto x : list)
		{
			Lnode* newnode = new Lnode;
			newnode->x = x;
			tail->next = newnode;
			newnode->next = nullptr;
			tail = newnode;
		}
	}
	friend std::ostream& operator<<(std::ostream& os, Lnode const& l)
	{
		Lnode* workptr = l.next;
		while (workptr != nullptr)
		{
			os << workptr->x << " ";
			workptr = workptr->next;
		}
		return os;
	}
};

struct CDLnode
{
	CDLnode* next;
	CDLnode* prior;
	int data;
	CDLnode(std::initializer_list<int> list = {})
		:next(this), prior(this), data(0)
	{
		for (auto i : list)
		{
			CDLnode* newnode = new CDLnode;
			newnode->data = i;
			newnode->next = this;
			newnode->prior = this->prior;
			this->prior = newnode;
			newnode->prior->next = newnode;
		}
	}
	~CDLnode()
	{
		CDLnode* workptr = next;
		while (workptr != this)
		{
			CDLnode* temp = workptr->next;
			workptr->next = workptr;
			delete workptr;
			workptr = temp;
		}
	}
	friend ostream& operator<<(ostream& os, CDLnode const& list)
	{
		CDLnode* workptr = list.next;
		while (workptr != &list)
		{
			os << workptr->data << " ";
			workptr = workptr->next;
		}
		return os;
	}
};

struct CLnode
{
	CLnode* next;
	int data;
	CLnode(std::initializer_list<int> list = {})
		:next(this), data(0)
	{
		CLnode* insertptr = this;
		for (auto i : list)
		{
			CLnode* newnode = new CLnode;
			newnode->data = i;
			insertptr->next = newnode;
			newnode->next = this;
			insertptr = newnode;
		}
	}
	~CLnode()
	{
		CLnode* workptr = next;
		while (workptr != this)
		{
			CLnode* temp = workptr->next;
			workptr->next = workptr;
			delete workptr;
			workptr = temp;
		}
	}
	friend ostream& operator<< (ostream& os, CLnode const& object)
	{
		CLnode* workptr = object.next;
		while (workptr != &object)
		{
			os << workptr->data << " ";
			workptr = workptr->next;
		}
		return os;
	}
};

template<typename T>
struct DLnode
{
	DLnode* prior;
	DLnode* next;
	T data;
	DLnode(std::initializer_list<T> list = {})
		:prior(nullptr), next(nullptr)
	{
		DLnode* insertptr = this;
		for (auto l : list)
		{
			DLnode* newnode = new DLnode;
			newnode->data = l;
			insertptr->next = newnode;
			newnode->prior = insertptr;
			insertptr = newnode;
		}
	}
	~DLnode()
	{
		DLnode* workptr = next;
		while (workptr != nullptr)
		{
			DLnode* next = workptr->next;
			workptr->next = nullptr;
			delete workptr;
			workptr = next;
		}
	}
	friend std::ostream& operator<<(std::ostream& os, DLnode const& head)
	{
		DLnode* workptr = head.next;
		while (workptr != nullptr)
		{
			os << workptr->data << " ";
			workptr = workptr->next;
		}
		return os;
	}
};

inline void print(vector<int> v)
{
	for (auto i : v)
	{
		std::cout << i << " ";
	}
}

inline void print(Lnode* l)
{
	Lnode* workptr = l->next;
	while (workptr != nullptr)
	{
		std::cout << workptr->x << " ";
		workptr = workptr->next;
	}
	std::cout << "\n";
}

inline void delete_x(Lnode* l, int x)
{
	Lnode* pre = l;
	Lnode* workptr = l->next;
	while (workptr != nullptr)
	{
		if (workptr->x == x)
		{
			Lnode* todelete = workptr;
			pre->next = workptr->next;
			delete todelete;
			workptr = pre->next;
		}
		else
		{
			workptr = workptr->next;
			pre = pre->next;
		}
	}
}

inline void delete_min(Lnode* l)
{
	Lnode* pre = l;
	Lnode* workptr = l->next;
	Lnode* min_pre = l;
	int min = numeric_limits<int>::max();
	while (workptr != nullptr)
	{
		if (workptr->x < min)
		{
			min = workptr->x;
			min_pre = pre;
		}
		pre = pre->next;
		workptr = workptr->next;
	}
	Lnode* todelete = min_pre->next;
	min_pre->next = min_pre->next->next;
	delete todelete;
}

inline void reverse(Lnode* l)
{
	Lnode* workptr = l->next;
	if (workptr == nullptr) return;
	while (workptr->next != nullptr)
	{
		Lnode* temp = workptr->next;
		workptr->next = temp->next;
		temp->next = l->next;
		l->next = temp;
	}
}

template<typename Func>
inline void erase_if(Lnode* l, Func pred)
{
	Lnode* pre = l;
	Lnode* workptr = l->next;
	while (workptr != nullptr)
	{
		if (pred(workptr->x))
		{
			pre->next = workptr->next;
			delete workptr;
			workptr = pre->next;
		}
		else
		{
			workptr = workptr->next;
			pre = pre->next;
		}
	}
}

inline pair<Lnode*, Lnode*> fork(Lnode* old)
{
	Lnode* v1 = new Lnode;
	v1->next = nullptr;
	Lnode* v2 = new Lnode;
	v2->next = nullptr;
	Lnode* workptr = old->next;
	Lnode* insertptr1 = v1;
	Lnode* insertptr2 = v2;
	while (workptr != nullptr)
	{
		Lnode* toinsert1 = workptr;
		Lnode* toinsert2 = workptr->next;
		workptr = workptr->next->next;

		toinsert1->next = insertptr1->next;
		insertptr1->next = toinsert1;
		insertptr1 = toinsert1;
		
		toinsert2->next = insertptr2->next;
		insertptr2->next = toinsert2;
	}
	delete old;
	return { v1, v2 };
}

inline void delete_multi(Lnode* l)
{
	Lnode* workptr = l->next;
	while (workptr != nullptr)
	{
		if (workptr->next != nullptr && workptr->next->x == workptr->x)
		{
			Lnode* todelete = workptr->next;
			workptr->next = todelete->next;
			delete todelete;
		}
		else
		{
			workptr = workptr->next;
		}
	}
}

inline Lnode* find_common(Lnode* l1, Lnode* l2)
{
	Lnode* it1 = l1->next;
	Lnode* it2 = l2->next;
	Lnode* v = new Lnode;
	Lnode* insertptr = v;
	while (it1 != nullptr && it2 != nullptr)
	{
		if (it1->x == it2->x)
		{
			Lnode* newnode = new Lnode;
			newnode->x = it1->x;
			insertptr->next = newnode;
			insertptr = newnode;
			newnode->next = nullptr;
			it1 = it1->next;
			it2 = it2->next;
		}
		else
		{
			if (it1->x < it2->x)
			{
				it1 = it1->next;
			}
			else
			{
				it2 = it2->next;
			}
		}
	}
	return v;
}

inline void merge(Lnode* l1, Lnode* l2)
{
	Lnode* pre = l1;
	Lnode* it1 = l1->next;
	Lnode* it2 = l2->next;
	while (it1 != nullptr && it2 != nullptr)
	{
		if (it1->x == it2->x)
		{
			pre = pre->next;
			it1 = it1->next;
			it2 = it2->next;
		}
		else if (it1->x < it2->x)
		{
			pre = pre->next;
			it1 = it1->next;
		}
		else
		{
			Lnode* newnode = new Lnode;
			newnode->x = it2->x;
			pre->next = newnode;
			newnode->next = it1;
			pre = newnode;
			it2 = it2->next;
		}
	}
	while (it2 != nullptr)
	{
		Lnode* newnode = new Lnode;
		newnode->x = it2->x;
		pre->next = newnode;
		newnode->next = it1;
		pre = newnode;
		it2 = it2->next;
	}
}

using vec = std::vector<int>;
inline bool match(vec const& text, vec const& pattern)
{
	auto gen_next = [](vec const& pattern) {
		vec next(pattern.size());
		int i = 1;
		int len = 0;
		next[0] = 0;
		while (i < pattern.size())
		{
			if (pattern[len] == pattern[i])
			{
				next[i] = len + 1;
				++i;
				++len;
			}
			else
			{
				if (len == 0)
				{
					next[i] = 0;
					++i;
				}
				else
				{
					len = next[len - 1];
				}
			}
		}
		return next;
		};
	auto next = gen_next(pattern);
	int mainit = 0;
	int patternit = 0;
	while (mainit != text.size())
	{
		if (text[mainit] == pattern[patternit])
		{
			++mainit;
			++patternit;
			if (patternit == pattern.size())
			{
				return true;
			}
		}
		else
		{
			if (patternit == 0)
			{
				++mainit;
			}
			else
			{
				patternit = next[patternit - 1];
			}
		}
	}

	return false;
}

inline bool is_symmetry(CDLnode* head)
{
	if (head->prior == head) return true;
	CDLnode* frontit = head->next;
	CDLnode* backit = head->prior;
	while (frontit != backit && frontit->prior != backit)
	{
		if (frontit->data != backit->data) return false;
		frontit = frontit->next;
		backit = backit->prior;
	}
	return true;
}

inline void merge(CLnode* l1, CLnode* l2)
{
	if (l2->next == l2) return;
	CLnode* workptr = l1;
	while (workptr->next != l1)
	{
		workptr = workptr->next;
	}
	CLnode* workptr2 = l2;
	while (workptr2->next != l2)
	{
		workptr2 = workptr2->next;
	}
	workptr->next = l2->next;
	workptr2->next = l1;
	l2->next = l2;
	delete l2;
}

template<typename T1, typename T2>
inline std::ostream& operator<<(std::ostream& os, std::pair<T1, T2> x)
{
	os << "(" << x.first << ", " << x.second << ")";
	return os;
}

inline void Locate(DLnode<std::pair<int, int>>* head, int x)
{
	auto workptr = head->next;
	while (workptr != nullptr && workptr->data.first != x)
	{
		workptr = workptr->next;
	}
	if (workptr == nullptr) return;
	++workptr->data.second;
	auto insertptr = head->next;
	while (insertptr->data.second > workptr->data.second)
	{
		insertptr = insertptr->next;
	}
	if (insertptr == workptr) return;
	workptr->prior->next = workptr->next;
	if (workptr->next != nullptr)
	{
		workptr->next->prior = workptr->prior;
	}
	insertptr->prior->next = workptr;
	workptr->prior = insertptr->prior;
	insertptr->prior = workptr;
	workptr->next = insertptr;
}

inline void right(Lnode* head, int k)
{
	Lnode* last, *first, *workptr;
	workptr = head->next;
	first = head;
	last = head;
	for (int i = 0; i < k; ++i)
	{
		workptr = workptr->next;
		last = last->next;
	}
	while (workptr != nullptr)
	{
		workptr = workptr->next;
		last = last->next;
		first = first->next;
	}
	Lnode* temp = first->next;
	first->next = nullptr;
	last->next = head->next;
	head->next = temp;
}

inline bool is_annulate(Lnode* head)
{
	Lnode* doubleptr = head;
	Lnode* singleptr = head;
	while (true)
	{
		if (doubleptr->next == nullptr || doubleptr->next->next == nullptr)
		{
			return false;
		}
		if (doubleptr->next == singleptr || doubleptr->next->next == singleptr)
		{
			return true;
		}
		doubleptr = doubleptr->next->next;
		singleptr = singleptr->next;
	}
}

inline Lnode* Locate(Lnode* head, int x)
{
	Lnode* workptr = head->next;
	while (workptr != nullptr && workptr->x != x)
	{
		workptr = workptr->next;
	}
	return workptr;
}

inline int find_last(Lnode* head, int x)
{
	Lnode* workptr = head;
	Lnode* slow = head;
	for (int i = 0; i < x; ++i)
	{
		workptr = workptr->next;
		if (workptr == nullptr) return 0;
	}
	while (workptr != nullptr)
	{
		workptr = workptr->next;
		slow = slow->next;
	}
	std::cout << slow->x;
	return 1;
}

inline Lnode* find_suffix(Lnode* l1, Lnode* l2)
{
	Lnode* pre1 = l1;
	Lnode* pre2 = l2;
	int len1 = 0;
	int len2 = 0;
	while (pre1->next != nullptr)
	{
		pre1 = pre1->next;
		++len1;
	}
	while (pre2->next != nullptr)
	{
		pre2 = pre2->next;
		++len2;
	}
	if (pre1 != pre2)
	{
		return nullptr;
	}
	if (len1 < len2)
	{
		int k = len2 - len1;
		pre1 = l1;
		pre2 = l2;
		for (int i = 0; i < k; ++i)
		{
			pre2 = pre2->next;
		}
		while (true)
		{
			if (pre1 == pre2)
			{
				return pre1;
			}
			pre1 = pre1->next;
			pre2 = pre2->next;
		}
	}
	else
	{
		int k = len1 - len2;
		pre1 = l1;
		pre2 = l2;
		for (int i = 0; i < k; ++i)
		{
			pre1 = pre1->next;
		}
		while (true)
		{
			if (pre1 == pre2)
			{
				return pre1;
			}
			pre1 = pre1->next;
			pre2 = pre2->next;
		}
	}
}

inline void retrofit(Lnode* list)
{
	Lnode* fast = list;
	Lnode* slow = list;
	while (fast != nullptr && fast->next != nullptr)
	{
		fast = fast->next->next;
		slow = slow->next;
	}
	// 倒置后半部分
	if (slow->next == nullptr) return;
	Lnode* current = slow->next;
	Lnode* next = nullptr;
	while (current != nullptr)
	{
		Lnode* temp = current->next;
		slow->next = current;
		current->next = next;
		next = current;
		current = temp;
	}
	// 后半部分往前挪
	Lnode* insertptr = list->next;
	while (slow->next != nullptr)
	{
		if (insertptr == slow) break;
		Lnode* temp = insertptr->next;
		insertptr->next = slow->next;
		slow->next = slow->next->next;
		insertptr->next->next = temp;
		insertptr = temp;
	}
}

class Cqueue
{
public:
	Cqueue(int N)
		:N(N), front(0), rear(0), tag(0), data(new int[N]) { }
	~Cqueue() { delete[]data; }
	void push_back(int x)
	{
		if (front == rear && tag)
		{
			throw "queue overflow";
		}
		data[rear] = x;
		rear = (rear + 1) % N;
		tag = 1;
	}
	void pop_front()
	{
		if (front == rear && !tag)
		{
			throw "empty queue";
		}
		front = (front + 1) % N;
		tag = 0;
	}
	int& operator[](int i)
	{
		int true_i = (i + front) % N;
		return data[true_i];
	}
	int at(int i) const
	{
		int true_i = (i + front) % N;
		int size = rear < front ? rear - front + N : rear - front;
		if (size == 0)
		{
			size = tag ? N : 0;
		}
		if (i >= size)
		{
			throw "invalid index";
		}
		return data[true_i];
	}
private:
	int front, rear, tag;
	int* data;
	const int N;
};

class CLQueue
{
public:
	struct CLQnode
	{
		int data;
		CLQnode* next;
		CLQnode(int data = 0): data(data), next(this){}
	};
	CLQueue(std::initializer_list<int> list)
		:front(nullptr), rear(nullptr)
	{
		front = new CLQnode;
		rear = front;
		for (auto i : list)
		{
			push_back(i);
		}
	}
	~CLQueue()
	{
		CLQnode* workptr = front;
		do
		{
			CLQnode* temp = workptr->next;
			delete workptr;
			workptr = temp;
		} while (workptr != front);
	}
	void push_back(int x)
	{
		CLQnode* newnode;
		if (rear->next == front)
		{
			newnode = new CLQnode(x);
			newnode->next = rear->next;
			rear->next = newnode;
			rear = newnode;
		}
		else
		{
			newnode = rear->next;
			newnode->data = x;
			rear = newnode;
		}
	}
	void pop_front()
	{
		if (front == rear)
		{
			throw "empty queue";
		}
		front = front->next;
	}
	bool empty() const
	{
		return front == rear;
	}
	friend std::ostream& operator<<(std::ostream& os, CLQueue const& x)
	{
		if (x.front != x.rear)
		{
			CLQueue::CLQnode* workptr = x.front->next;
			while (workptr != x.rear)
			{
				os << workptr->data << " ";
				workptr = workptr->next;
			}
			os << workptr->data;
		}
		return os;
	}
	bool empty()
	{
		return front == rear;
	}
private:
	CLQnode* front;
	CLQnode* rear;
};

#include <stack>

inline bool is_valid(std::string const& s)
{
	std::stack<char> st;
	for (auto i : s)
	{
		switch (i)
		{
		case '(':
		case '[':
		case '{':
		{
			st.push(i); break;
		}
		case ')':
		{
			if (st.top() == '(')
				st.pop();
			else
				return false;
			break;
		}
		case ']':
		{
			if (st.top() == '[')
				st.pop();
			else
				return false;
			break;
		}
		case '}':
		{
			if (st.top() == '{')
				st.pop();
			else
				return false;
			break;
		}
		default: return false;
		}
	}
	return st.empty();
}

#include "crtdbg.h"

template<typename Container>
inline bool KMP(Container const& text, Container const& pattern)
{
	vec next(pattern.size());
	// get next
	{ 
		int i = 0; int j = -1;
		next[0] = -1;
		while (i < pattern.size() - 1)
		{
			if (j == -1 || pattern[i] == pattern[j])
			{
				++i; ++j;
				next[i] = j;
			}
			else
			{
				j = next[j];
			}
		}
	}
	vec nextval(pattern.size());
	{
		int i = 0; int j = -1;
		nextval[0] = -1;
		while (i < pattern.size() - 1)
		{
			if (j == -1 || pattern[i] == pattern[j])
			{
				++i; ++j;
				if (pattern[i] == pattern[j])
				{
					nextval[i] = nextval[j];
				}
				else
				{
					nextval[i] = j;
				}
			}
			else
			{
				j = nextval[j];
			}
		}
	}
	// match
	{ 
		int i = 0; int j = 0;
		while (i != text.size())
		{
			if (j == -1 || text[i] == pattern[j])
			{
				++i; ++j;
				if (j == pattern.size())
				{
					return true;
				}
			}
			else
			{
				j = next[j];
			}
		}
		return false;
	}
}

inline std::ostream& operator<<(std::ostream& os, vec const& v)
{
	for (auto i : v)
	{
		os << i << " ";
	}
	return os;
}


#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

#define SIZE_OF_COMMAND 15 

typedef struct list {
	int data;
	list* xorptr;
}list;
list* beg = nullptr;
list* end = nullptr;
list* actual = nullptr;
list* next = nullptr;
list* prev = nullptr;
list* front = nullptr;
list* back = nullptr;

list* XOR(list* xorptr, list* ptr) {
	return reinterpret_cast<list*>(reinterpret_cast<uintptr_t>(xorptr) ^ reinterpret_cast<uintptr_t>(ptr));
}

int SIZE() {
	list* act_ele = beg;
	list* prev_ele = end;
	list* tmp = nullptr;
	int i = 0;
	if (act_ele != nullptr) {
		do {
			i++;
			tmp = act_ele;
			act_ele = XOR(act_ele->xorptr, prev_ele);
			prev_ele = tmp;
		} while (act_ele != beg);
	}
	return i;
}

void NEXT() {
	if (next != nullptr) {
		prev = actual;
		actual = next;
		if (next == end) next = beg;
		else next = XOR(next->xorptr, prev);
	}
}

void PREV() {
	if (prev != nullptr) {
		next = actual;
		actual = prev;
		if (prev == beg) prev = end;
		else prev = XOR(prev->xorptr, next);
	}
}

void ADD_BEG(int data) {
	list* new_ele = new list();
	new_ele->data = data;
	if (beg == nullptr) {
		new_ele->xorptr = XOR(new_ele, new_ele);
		end = new_ele;
		next = new_ele;
		actual = new_ele;
		prev = new_ele;
	}
	else {
		beg->xorptr = XOR(new_ele, XOR(beg->xorptr, end));
		end->xorptr = XOR(XOR(end->xorptr, beg), new_ele);
		new_ele->xorptr = XOR(end, beg);
	}
	if (beg == actual) prev = new_ele;
	beg = new_ele;
	if (actual == end) next = beg;
}

void ADD_END(int data) {
	list* new_ele = new list();
	new_ele->data = data;
	if (end == nullptr) {
		new_ele->xorptr = XOR(new_ele, new_ele);
		beg = new_ele;
		prev = new_ele;
		actual = new_ele;
		next = new_ele;
	}
	else {
		beg->xorptr = XOR(new_ele, XOR(beg->xorptr, end));
		end->xorptr = XOR(XOR(end->xorptr, beg), new_ele);
		new_ele->xorptr = XOR(end, beg);
	}
	if (end == actual) next = new_ele;
	end = new_ele;
	if (actual == beg) prev = end;
}

void ADD_ACT(int data) {
	list* new_ele = new list();
	new_ele->data = data;
	prev->xorptr = XOR(XOR(prev->xorptr, actual), new_ele);
	actual->xorptr = XOR(new_ele, next);
	new_ele->xorptr = XOR(prev, actual);
	prev = new_ele;
	if (actual == beg) beg = new_ele;
}

void DEL_BEG() {
	if (beg != nullptr) {
		if (beg == end) {
			end = nullptr;
			beg = nullptr;
			prev = nullptr;
			actual = nullptr;
			next = nullptr;
		}
		else {
			list* del;
			del = beg;
			beg = XOR(beg->xorptr, end);
			beg->xorptr = XOR(end, XOR(beg->xorptr, del));
			end->xorptr = XOR(XOR(end->xorptr, del), beg);
			if (prev == end || actual == beg) {
				prev = end;
				actual = beg;
				next = XOR(actual->xorptr, prev);
			}
			else if (actual == end) {
				actual = end;
				next = beg;
			}
			if (del == front) front = beg;
			else if (del == back) back = beg;
			delete del;
		}
	}
}

void DEL_END() {
	if (end != nullptr) {
		if (beg == end) {
			end = nullptr;
			beg = nullptr;
			prev = nullptr;
			actual = nullptr;
			next = nullptr;
		}
		else {
			list* del;
			del = end;
			end = XOR(end->xorptr, beg);
			end->xorptr = XOR(XOR(end->xorptr, del), beg);
			beg->xorptr = XOR(end, XOR(beg->xorptr, del));
			if (prev == end) {
				actual = end;
				prev = XOR(end->xorptr, beg);
			}
			else if (actual == beg) {
				prev = end;
			}
			else if (actual == end) {
				next = beg;
			}
			if (del == front) front = beg;
			else if (del == back) back = end;
			delete del;
		}
	}
}

void DEL_ACT() {
	if (actual != nullptr) {
		if (beg == end) {
			end = nullptr;
			beg = nullptr;
			prev = nullptr;
			actual = nullptr;
			next = nullptr;
		}
		else {
			list* del, * new_prev;
			new_prev = XOR(prev->xorptr, actual);
			del = actual;
			prev->xorptr = XOR(XOR(prev->xorptr, actual), next);
			next->xorptr = XOR(prev, XOR(next->xorptr, actual));
			actual = prev;
			prev = new_prev;
			if (actual == end) beg = next;
			else if (next == beg) end = actual;
			delete del;
		}
	}
}

void PRINT_FORWARD() {
	list* act_ele = beg;
	list* prev_ele = end;
	list* tmp = nullptr;
	if (act_ele == nullptr) printf("NULL\n");
	else {
		do {
			printf("%d ", act_ele->data);
			tmp = act_ele;
			act_ele = XOR(act_ele->xorptr, prev_ele);
			prev_ele = tmp;
		} while (act_ele != beg);
		printf("\n");
	}
}

void PRINT_BACKWARD() {
	list* act_ele = end;
	list* next_ele = beg;
	list* tmp = nullptr;
	if (act_ele == nullptr) printf("NULL\n");
	else {
		do {
			printf("%d ", act_ele->data);
			tmp = act_ele;
			act_ele = XOR(act_ele->xorptr, next_ele);
			next_ele = tmp;
		} while (act_ele != end);
		printf("\n");
	}
}

void PUSH(int data) {
	if (beg == nullptr) {
		ADD_BEG(data);
		front = beg;
		back = beg;
	}
	else if (front == nullptr) {
		end->data=data;
		front = end;
		back = end;
	}
	else {
		while (actual != front) NEXT();
		if (prev == back && actual==beg) {
			ADD_BEG(data);
			front = prev;
		}
		else if (prev == back) {
			ADD_ACT(data);
			front = prev;
		}
		else {
			PREV();
			actual->data = data;
			front = actual;
		}
	}
}

int COUNT() {
	int i = 0;
	if (front != nullptr) {
		while (actual != back) NEXT();
		i++;
		while (actual != front) {
			i++;
			PREV();
		}
	}
	return i;
}

void POP() {
	if (front == nullptr) printf("NULL\n");
	else {
		while (actual != back) NEXT();
		printf("%d\n", actual->data);
		if (front == back) {
			front = nullptr;
			back = nullptr;

		}
		else back = prev;
	}
}

void PRINT_QUEUE() {
	if(front==nullptr) printf("NULL\n");
	else {
		while (actual != back) NEXT();
		printf("%d ", actual->data);
		while (actual != front) {
			PREV();
			printf("%d ", actual->data);
		}
		printf("\n");
	}
}

void GARBAGE_SOFT() {
	if (COUNT() != 0) {
		while (actual != back) NEXT();
		NEXT();
		while (actual != front) {
			actual->data = 0;
			NEXT();
		}
	}
	else if (beg != nullptr) {
		while (actual != beg) NEXT();
		do {
			actual->data = 0;
			NEXT();
		} while (actual != end);
	}
}

void GARBAGE_HARD() {
	if (COUNT() != 0) {
		while (actual != front) NEXT();
		PREV();
		while (actual != back) DEL_ACT();
	}
	else if (beg != nullptr) {
		while (actual != end) NEXT();
		do {
			DEL_ACT();
		} while (actual != nullptr);
	}
}

int main() {
	char command[SIZE_OF_COMMAND];
	int data;
	while (std::cin >> command) {
		if (strcmp(command, "ADD_BEG") == 0) {
			scanf("%d", &data);
			ADD_BEG(data);
		}
		else if (strcmp(command, "ADD_END") == 0) {
			scanf("%d", &data);
			ADD_END(data);
		}
		else if (strcmp(command, "PUSH") == 0) {
			scanf("%d", &data);
			PUSH(data);
		}
		else if (strcmp(command, "DEL_BEG") == 0) DEL_BEG();
		else if (strcmp(command, "DEL_END") == 0) DEL_END();
		else if (strcmp(command, "PRINT_FORWARD") == 0) PRINT_FORWARD();
		else if (strcmp(command, "PRINT_BACKWARD") == 0) PRINT_BACKWARD();
		else if (strcmp(command, "PRINT_QUEUE") == 0) PRINT_QUEUE();
		else if (strcmp(command, "SIZE") == 0) printf("%d\n", SIZE());
		else if (strcmp(command, "COUNT") == 0) printf("%d\n", COUNT());
		else if (strcmp(command, "POP") == 0) POP();
		else if (strcmp(command, "GARBAGE_SOFT") == 0) GARBAGE_SOFT();
		else if (strcmp(command, "GARBAGE_HARD") == 0) GARBAGE_HARD();
	}
	return 0;
}

#include <iostream>
#if defined(DEBUG) || defined(_DEBUG) || defined(__debug)

//----------------Forward declaration-------------------
void close();
void saveData(void* p, int s, const char* f);
//class LinkedList; -> this not work !?!?!?!?

//----------------Global variables----------------------
const char* __file__ = "unknown"; //default file value
int __line__ = 0; //default line value
//Type of new calls
const char * normal = "Normal";
const char * normal_array = "Normal Array";
const char * no_error = "No error";
const char * no_error_array = "No error array";




//------------------------------------------- HELP CLASSES -----------------------------------------------

struct Node {
	Node(void* p,int s,int l,const char* f,const char* t) : ptr(p),size(s),line(l),file(f),type(t) {} 
	void* ptr;
	int size;
	int   line;
	const char* file;
	const char* type;
	Node* next;
};

class LinkedList {
public:
	LinkedList() : _head(NULL), _total_size(0),_current_size(0), _count(0) {
		std::cout << "Memory Manager init" << std::endl; 
		atexit(close); //atexit call the given function after the main function is terminated
	}
	// Inserts a node at the beginning of the list.
	void insert(Node * n) {
		//list variable update
		_total_size += n->size;
		_current_size += n->size;
		_count++;
		_new_count++;
		std::cout << "new " << n->type << " | Size: " << n->size << " | Ptr: " << n->ptr << " | #" << _new_count << std::endl;
		//Linked list population
		n->next = _head;
		_head = n;
	}

	// Removes a given node from the list.
	void remove(void*p, const char* t) {
		_count--;
		_delete_count++;
		Node** current = &_head; //double pointer -> to solve Exception thrown: read access violation current was 0xDDDDDDDD.

		while (NULL != *current) {
			if ((*current)->ptr == p) {
				Node * n = (*current);

				//list variable update
				_current_size -= n->size;
		
				//print infromation
				std::cout << "delete " << t << " | Size: " << n->size << " | Ptr: " << n->ptr << " | #" << _delete_count << std::endl;

				if (strcmp(t, n->type) != 0) { //the allocatation and the deallocation type are different
					std::cout << "-----------------WARNING: this deallocation defer from the allocation---------------------" << std::endl;
				}

				*current = n->next;
				free(n);
				return;
			}
			current = &((*current)->next);
		}
	}
	//print all information about the state of the linkedlist
	void printAll() {
		std::cout << "-------------------------------------------------------------" << std::endl;
		std::cout << "Memory Manager exit" << std::endl;
		std::cout << "No. of memory leaks: " << _count << std::endl;
		std::cout << "Total size of leaks: " << _current_size << std::endl;
		std::cout << "Max. memory used: " << _total_size << std::endl;
		
		Node* current = _head;
		if (current != NULL) {
			std::cout << "\n Memory Leaks:" << std::endl;
		}

		while (NULL != current) {
			std::cout << "Ptr: " <<  current->ptr << std::endl;
			std::cout << "Size: " << current->size << std::endl;
			std::cout << "Line: " << current->line << std::endl;
			std::cout << "File: " << current->file << std::endl;
			std::cout << "Type: " << current->type << std::endl;
			current = current->next;
		}
		std::cout << "-------------------------------------------------------------" << std::endl;
	}

private:
	Node* _head;
	int _total_size; //total size of all allocated variables
	int _current_size; //current size of the Linkedlist element
	int _count ; // Count how many memory leaks are present ------------ TODO _new_count - _delete_count 
	int _new_count; //Number of allocated variable
	int _delete_count; //Number of deallocated variable
};

// declaration of global variable list -> forward declaration do not work ?!?!?!?!?
static LinkedList list;

//-------------------------------------- NEW OPERATORS ----------------------------------------

void* operator new (size_t size) throw (std::bad_alloc) //normal new
{
	void* p = malloc(size);
	if (!p) throw std::bad_alloc();
	saveData(p, size, normal);
	return p;
};

void* operator new (std::size_t size, const std::nothrow_t& nothrow_value) throw() { // new without error
	void* p = malloc(size);
	if (!p) return NULL;
	saveData(p, size, no_error);
	
	return p;
};

void* operator new[] (size_t size) throw (std::bad_alloc) //Array new
{
	void* p = malloc(size);
	if (!p) throw std::bad_alloc();
	saveData(p, size, normal_array);
	return p;
};

void* operator new[](size_t size, const std::nothrow_t& nothrow_value) throw() // Array new without error
{
	void* p = malloc(size);
	if (!p) return NULL;
	saveData(p, size, no_error_array);
	return p;
};

//----------------------------------------------- DELETE OPERATORS -----------------------------------

void operator delete(void *ptr) throw() //normal delete
{
	list.remove(ptr, normal);
	free(ptr);
};

void operator delete(void *ptr, const std::nothrow_t& nothrow_value) throw() //delete without error
{
	list.remove(ptr, no_error);
	free(ptr);
};

void operator delete[](void *ptr) throw() {//Array delete
	list.remove(ptr, normal_array);
	free(ptr);
}

void operator delete[](void *ptr, const std::nothrow_t& nothrow_value) throw(){ //Array delete without error
	list.remove(ptr, no_error_array);
	free(ptr);
}





//------------------------------------------------HELPER-------------------------------------------- 


//Help class to not have duplicated code
void saveData(void* p, int s, const char* f) {
	//New node with malloc and variable assignement
	Node* node = (struct Node*) malloc(sizeof(struct Node));
	node->ptr = p;
	node->size = s;
	node->line = __line__;
	node->file = __file__;
	node->type = f;

	list.insert(node);
	__file__ = "unknown"; //reset the default file and line
	__line__ = 0;
};

//Function called by atexits
void close(void) {
	list.printAll();
}




#endif
//TODO.cpp
//Shreya Gowda, CISP 400
//11/17/2019

#include <iostream>
#include <random>
#include <fstream>
#include <string>
using namespace std;

//OOP
class Todo_Item{
  private:
  int id;
  string date;
  string description;
  Todo_Item* next;
  Todo_Item* prev;

  public:
 //constructors
  Todo_Item(){
    id=0;
    description="";
    date="";
    prev=nullptr;
    next=nullptr;
  }

  Todo_Item(int i, string d, string t){
    id=i;
    description=d;
    date=t;
    prev=nullptr;
    next=nullptr;
  }

  // Specification A2 - Copy Constructor
  Todo_Item(const Todo_Item &obj){
    description=obj.description;
    id=obj.id;
    date=obj.date;

    /*// Test & Debug Copy Constructor
    cout<<endl<<"Copy constructor called for "<< this->description <<" @ memory loc "<<this<<endl;
    */
  }

  // Specification A3 - Assignment
  Todo_Item& operator=(const Todo_Item &obj){
    description=obj.description;
    id=obj.id;
    date=obj.date;

    /*// Test & Debug Copy Assignment
    cout<<endl<<"Copy assignment called for "<< this->description <<" @ memory loc "<<this<<endl;
    */
    return *this;
  }

  void make_Next(Todo_Item* n){
    next=n;
  }

  Todo_Item* get_Next(){
    return next;
  }

  void make_Prev(Todo_Item* p){
    prev=p;
  }

  Todo_Item* get_Prev(){
    return prev;
  }

  /*
  ~Todo_Item(){
    // Test & Debug Destructor
    cout<<endl<<"Destructor called for "<<this->description<<" @ memory loc "<<this<<endl;
  }
  */

  // Specification A1 - Overload symbols
  friend Todo_Item operator + (Todo_Item todo);
  friend bool operator - (Todo_Item todo, int id);
  friend ostream& operator << (ostream &out, Todo_Item todo);
}; 

Todo_Item operator + (Todo_Item todo){
//fills empty object with id, description, date

  time_t date=time(0);

  tm* date_info = localtime(&date); 

  //individual strings w/ info on date
  string month=to_string((date_info->tm_mon)+1);
  string day=to_string(date_info->tm_mday);
  string year=to_string((date_info->tm_year)+1900);

  //concatenates individual strings to form whole date
  todo.date=month+"/"+day+"/"+year;

  getline(cin,todo.description);

  srand(time(0));

  todo.id=100+rand()%900;

  return todo;
  
}

bool operator - (Todo_Item todo, int id){
// tells if task exists

  if(todo.id==id){
    return true;
  }
  return false;

}

// Specification B1 - Overload «
ostream & operator << (ostream &out, Todo_Item todo){

  out << todo.id << "    " << todo.date << "    " << todo.description;

  return out;

}

// Function Prototypes
void add(Todo_Item** first, Todo_Item** current, Todo_Item** task);
void del(Todo_Item** first, Todo_Item** current);
void display(Todo_Item** first);
void write_disk(Todo_Item** first, Todo_Item** current);
void Instructions();
void del_all(Todo_Item** first);

int main(){
  Todo_Item* first=nullptr;
  Todo_Item* current=nullptr;
  Todo_Item* task=nullptr;
  string desc="";
  string str_id="";
  int num_id=0;
  string date="";
  string opt="";

  //load saved tasks
  ifstream add_obj("TodoDisk.txt", ios::in);
  if(add_obj){
    while(add_obj>>str_id){
      num_id=stoi(str_id);
      add_obj>>date;
      getline(add_obj, desc);
      desc=desc.substr(5);
      task=new Todo_Item(num_id, desc, date);
      add(&first, &current, &task);
    }
  }

  //Program Greeting
  cout<<"Welcome to TODO list program! ";
  Instructions();
  
  add_obj.close();

  // Specification B3 - Quit symbol
  while(opt!="!"){
    cout<<endl<<"Type Command: ";
    cin>>opt;

    // Specification C1 - + Symbol
    if(opt=="+"){
      task=new Todo_Item;
      *task=+*task;
      add(&first, &current, &task);
    }

    // Specification C3 - - symbol
    if(opt=="-"){
      del(&first, &current);
    }

    // Specification C2 - ? Symbol
    if(opt=="?"){
      display(&first);
    }
  }

  //Specification B2 - Save to disk
  ofstream save_obj("TodoDisk.txt",ios::out);
  task=first;
  while(task!=nullptr){
    save_obj<<*task;
    save_obj<<endl;
    task=task->get_Next();
  }
  save_obj.close();

  del_all(&first);

  first=nullptr;
  current=nullptr;
  task=nullptr;
  
}

void Instructions(){
//prints user instructions about program
  cout<<endl<<endl<<"Instructions"<<endl;
  cout<<"To add a task, type: + [insert string task here] ; ex: + Study for finals "<<endl;
  cout<<"To delete a task, type: - [ task id number ] ; ex: - 123 "<<endl;
  cout<<"To display all tasks, type: ? "<<endl;
  cout<<"To exit program, type: ! "<<endl<<endl;
  cout<<"*NOTE: Remember to put space after symbol"<<endl<<endl;
}

void add(Todo_Item** first, Todo_Item** current, Todo_Item** task){
// when task object created, links it to other objects
  Todo_Item* next=nullptr;
  Todo_Item* prev=nullptr;

  //current task is set to newly created task. 
  //previous and next task of current task is set
  if((*first)==nullptr){
  //if first task, first pointer set to current pointer
    (*current)=(*task);
    (*first)=(*current);

  //current's next and previous are null
    (*current)->make_Next(nullptr);
    (*current)->make_Prev(nullptr);
  }
  else{
  /*before making current task = new task, 
  sets previous task's next to current*/
    next=(*task);
    (*current)->make_Next(next);

  //makes current task = new task
  //sets current's next and prev to appropriate values
    prev=(*current);
    (*current)=next;
    next=nullptr;

    (*current)->make_Prev(prev);
    (*current)->make_Next(next);
  }

  //no dangling ptrs
  next=nullptr;
  prev=nullptr;
}

void del(Todo_Item** first, Todo_Item** current){
// deletes task and reassigns links
  Todo_Item* next=nullptr;
  Todo_Item* prev=nullptr;
  Todo_Item* task=(*first);
  int del_id=0;
  bool found=false;  // task id exists or not

  cin>>del_id;
 
  while((task!=nullptr)&&(!found)){
    found=*task-del_id;
    if(found){
    //if the task is found does this:
      prev=task->get_Prev();
      next=task->get_Next();

      if(prev==nullptr){
      //if task to be deleted is the first task, 
      //next task becomes first task
      //next task's previous object is nullptr (because it is the new first)
        (*first)=next;
        next->make_Prev(nullptr);
      }
      else{
      //else task.prev's next is task.next
        prev->make_Next(next);
      }

      if(next==nullptr){
      //if task to be deleted is the last task, 
      //previous task becomes last task
      //previous task's next object is nullptr (because it is the new last)
        (*current)=prev;
        prev->make_Next(nullptr);
      }
      else{
      //else task.next's prev is task.prev
        next->make_Prev(prev);
      }

      delete task;
    }
    else{
    //else if task id is not found yet, go to next task
      task=task->get_Next();
    }
  }
  
  //if task id never found, tdne
  if(!found){
    cout<<endl<<"Task Does Not Exist"<<endl<<endl;
  }

  //no dangling ptrs
  next=nullptr;
  prev=nullptr;
  task=nullptr;
}

void display(Todo_Item** first){
// displays all objects
  Todo_Item* task=(*first);

  while(task!=nullptr){
    cout<<*task;
    cout<<endl;
    task=task->get_Next();
  }

  task=nullptr;
}

void del_all(Todo_Item** first){
Todo_Item* task=nullptr;
Todo_Item* next=nullptr;
 //delete all objects
  task=(*first);
  while(task!=nullptr){
    next=task->get_Next();
    delete task;
    task=next;
  }
}

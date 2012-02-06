#include<iostream>
#include<stdio.h>
using namespace std;
#define LIMIT 10

class CmList {
   public:
	int x,y;
	int len;
	CmList* next;
	CmList* parent;
	CmList();
	int sizeofList();
	void outputList();
	CmList* insertIntoList(int,int);	
	int checkparent();	
		
};
CmList::CmList() {
	x=0;
	y=0;
	len=0;
	next=NULL;
	parent=NULL;
}

CmList* CmList::insertIntoList(int a,int b) {
	x=a;
	y=b;
	next=new CmList;
	next->parent= this;
	this->next->len=this->len+1;
	return next;
}

void CmList::outputList() {
	if(this->len>0) {
		cout<<this->parent->x <<"\t"<< this->parent->y <<endl;
		this->parent->outputList();
	}

}
int main() {
	CmList Entry,*List;
	List=&Entry;
	List=List->insertIntoList(1,2);
	List=List->insertIntoList(4,3);
	List->outputList();
	return 0;
}	

#ifndef GROUP_H
#define GROUP_H


#include "Object3D.h"
#include "Ray.h"
#include "Hit.h"
#include <iostream>

using  namespace std;

///TODO: 
///Implement Group
///Add data structure to store a list of Object* 
class Group:public Object3D
{
public:

  Group(){

  }
	
  Group( int num_objects )
  {
    Num_objects = num_objects;
  }

  ~Group(){
   
  }

  virtual bool intersect( const Ray& r , Hit& h , float tmin ) 
  {
    bool Bool = false;
    for(int i=0; i<Num_objects; i++)
    {
      if(objectList[i]->intersect(r, h, tmin)){Bool = true;}
    }
    return Bool;
  }
	
  void addObject(int index, Object3D* obj)
  {
    objectList.push_back(obj);
  }

  int getGroupSize()
  {
    return Num_objects;
  }

 private:
  int Num_objects;
  vector<Object3D*> objectList;

};

#endif
	

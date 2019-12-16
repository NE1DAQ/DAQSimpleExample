#ifndef HIT_H
#define HIT_H

#include <SerialisableObject.h>

class Hit: public SerialisableObject{

  friend class boost::serialization::access;

 public:

  Hit();  
  //  ~Hit(); 
  Hit(float timein, float chargein, int idin);
  bool Print(){};

  float time;
  float charge;
  int id;

 private:

  template <class Archive> void serialize(Archive& ar, const unsigned int version){

    ar & time;
    ar & charge;
    ar & id;
 
  }
};


#endif

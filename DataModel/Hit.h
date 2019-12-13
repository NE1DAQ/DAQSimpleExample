#ifndef HIT_H
#define HIT_H

#include <SerialisableObject.h>

class Hit: public SerialisableObject{

  friend class boost::serialization::access;

 public:

  Hit();  
  Hit(float timein, float chargein, int idin);

  float time;
  float charge;
  int id;

  bool Print(){};

 private:

  template <class Archive> void serialize(Archive& ar, const unsigned int version){

    ar & time;
    ar & charge;
    ar & id;
 
  }
};


#endif

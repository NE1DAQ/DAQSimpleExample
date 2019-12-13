#ifndef HITS_H
#define HITS_H

#include <Hit.h>
#include <vector>
#include <zmq.hpp>
#include <SerialisableObject.h>

class Hits : public SerialisableObject{

  friend class boost::serialization::access;

 public:

  Hits();
  bool Send(zmq::socket_t* sock);
  bool Receive(zmq::socket_t* sock);
  std::vector<Hit> hits; 
  bool Print(){};

 private:

  template <class Archive> void serialize(Archive& ar, const unsigned int version){

    ar & hits;
     
  }

}; 

#endif

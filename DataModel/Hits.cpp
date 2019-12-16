#include <Hits.h>

void cleanup (void *data, void *hint) {

  Hits* tmp=static_cast<Hits*>(hint);

  delete tmp;
  tmp=0;
}

Hits::Hits(){};

//Hits::~Hits(){

//  hits.clear();
  
//};

bool Hits::Send(zmq::socket_t* sock){

  bool ret=false;
  zmq::message_t message(&hits[0], sizeof(Hit)*hits.size(), cleanup, this);
  ret=sock->send(message);

  return ret;
}


bool Hits::Receive(zmq::socket_t* sock){

  bool ret=false;
  zmq::message_t message;
  ret=sock->recv(&message);
  hits.clear();
  hits.resize(message.size()/sizeof(Hit));
  std::memcpy(&hits[0], message.data(), message.size());

  return ret;
}


#include "RBUDistributor.h"

RBUDistributor_args::RBUDistributor_args(zmq::context_t* contextin, Utilities* utilsin, std::string connection, int port):Thread_args(){

  std::cout<< connection <<" "<<port<<std::endl;
  context=contextin;
  utils=utilsin;
  std::cout<<"b1"<<std::endl;
  sockin=new zmq::socket_t(*context, ZMQ_PAIR);
  sockin->bind(connection.c_str());
  std::cout<<"b2"<<std::endl;
  sockout=new zmq::socket_t(*context, ZMQ_PUSH);
  std::stringstream tmp;
  tmp<<"tcp://*:"<<port;
  sockout->bind(tmp.str().c_str());
  std::cout<<"b3"<<std::endl;
  utils->AddService("ProcessorData",port,false);
  std::cout<<"b4"<<std::endl;
  in[0].socket = *sockin;
  in[0].fd = 0;
  in[0].events = ZMQ_POLLIN;
  in[0].revents = 0;
  std::cout<<"b5"<<std::endl;
  out[0].socket = *sockout;
  out[0].fd = 0;
  out[0].events = ZMQ_POLLOUT;
  out[0].revents = 0; 
std::cout<<"b6"<<std::endl;

}

RBUDistributor_args::~RBUDistributor_args(){

  utils->RemoveService("ProcessorData");
  
  delete sockin;
  sockin=0;
  
  delete sockout;
  sockout=0;

}


RBUDistributor::RBUDistributor():Tool(){}


bool RBUDistributor::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;

  std::cout<<"d1"<<std::endl;
  if(!m_variables.Get("Processor_hits_size",numhits)) numhits=1000;
  if(!m_variables.Get("Processor_port",port)) port=66666;

  std::cout<<"d2"<<std::endl;
  m_util=new Utilities(m_data->context);
  std::cout<<"d3"<<std::endl;
  args=new RBUDistributor_args(m_data->context, m_util, "inproc://RBUDistributor",port);
  std::cout<<"d4"<<std::endl;

  m_util->CreateThread("test", &Thread, args);
  std::cout<<"d5"<<std::endl;
  sock = new zmq::socket_t(*(m_data->context), ZMQ_PAIR);
  sock->connect("inproc://RBUDistributor");
  std::cout<<"d6"<<std::endl;
  items[0].socket = *sock;
  items[0].fd = 0;
  items[0].events = ZMQ_POLLOUT;
  items[0].revents = 0;
  std::cout<<"d7"<<std::endl;

  return true;
}


bool RBUDistributor::Execute(){

  if(m_data->hits->hits.size()>=numhits){

    zmq::poll(&items[0], 1, 0);
    
    if ((items [0].revents & ZMQ_POLLOUT)) {
      if(m_util->SendPointer(sock,m_data->hits)) m_data->hits=new Hits();
    }
    
  }

  return true;
}


bool RBUDistributor::Finalise(){

  m_util->KillThread(args);

  delete args;
  args=0;

  delete m_util;
  m_util=0;

  return true;
}

void RBUDistributor::Thread(Thread_args* arg){

  RBUDistributor_args* args=reinterpret_cast<RBUDistributor_args*>(arg);

  zmq::poll(&(args->in[0]), 1, 100);

  if ((args->in[0].revents & ZMQ_POLLIN)) {

    Hits* tmp;
    args->utils->ReceivePointer(args->sockin,tmp);

    zmq::poll(&(args->out[0]), 1, 100);

    if ((args->in[0].revents & ZMQ_POLLOUT)) {
      
      tmp->Send(args->sockout);
      
    }
  }
  
  
}

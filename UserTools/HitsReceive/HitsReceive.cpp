#include "HitsReceive.h"

HitsReceive::HitsReceive():Tool(){}


bool HitsReceive::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;

  if(!m_variables.Get("Endpoint",endpoint)) return false;

  m_util=new Utilities(m_data->context);
  
  sock = new  zmq::socket_t(*(m_data->context), ZMQ_PULL);
 
  m_util->UpdateConnections (endpoint,sock,connections);
  
  last= boost::posix_time::ptime(boost::posix_time::second_clock::local_time());
  period =boost::posix_time::time_duration(0,0,1,0);

  items[0].socket = *sock;
  items[0].fd = 0;
  items[0].events = ZMQ_POLLIN;
  items[0].revents = 0;  

  m_data->hits= new Hits();

  return true;
}


bool HitsReceive::Execute(){

  std::cout<<"f1"<<std::endl;
  boost::posix_time::ptime current(boost::posix_time::second_clock::local_time());
  boost::posix_time::time_duration duration(current - last);
  if(duration>period){
    std::cout<<"f2"<<std::endl;
    last=current;
    m_util->UpdateConnections(endpoint,sock,connections);
    std::cout<<"f3"<<std::endl;  
}
  std::cout<<"f4"<<std::endl;
  zmq::poll(&items[0], 1, 100);
  std::cout<<"f5"<<std::endl;
  if ((items [0].revents & ZMQ_POLLIN)) {
    std::cout<<"f6"<<std::endl;
    Hits tmp;
    std::cout<<"f7"<<std::endl;
    tmp.Receive(sock);
    std::cout<<"f7.5 "<<tmp.hits.size()<<std::endl;
    for(int i=0;i<tmp.hits.size();i++){
      std::cout<<"i="<<i<<"  "<<tmp.hits.at(i).time<<"  "<<tmp.hits.at(i).charge<<"  "<<tmp.hits.at(i).id<<std::endl;
      Hit a(2.2,2.2,2.2);
      m_data->hits->hits.push_back(a);
    }
    std::cout<<"f8 "<< m_data->hits<<" : "<<m_data->hits->hits.size()<<std::endl;
    //m_data->hits->hits.insert( m_data->hits->hits.end(), tmp.hits.begin(), tmp.hits.end());
    std::cout<<"f9"<<std::endl;
  }
  std::cout<<"f10"<<std::endl;
  return true;
}


bool HitsReceive::Finalise(){


  delete sock;
  sock=0;

  m_data->hits->hits.clear();
  delete m_data->hits;
  m_data->hits=0;

  return true;
}

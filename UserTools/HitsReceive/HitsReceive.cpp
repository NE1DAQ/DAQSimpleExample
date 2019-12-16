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

  boost::posix_time::ptime current(boost::posix_time::second_clock::local_time());
  boost::posix_time::time_duration duration(current - last);
  if(duration>period){
    last=current;
    m_util->UpdateConnections(endpoint,sock,connections);
}

  if( m_data->hits==0) m_data->hits=new Hits();
  
  if(m_data->hits->hits.size()<50000){
    zmq::poll(&items[0], 1, 100);
    
    if ((items [0].revents & ZMQ_POLLIN)) {
      
      Hits tmp;
      tmp.Receive(sock);
      
      m_data->hits->hits.insert( m_data->hits->hits.end(), tmp.hits.begin(), tmp.hits.end());
      
    }
  
  }
  
  return true;
}


bool HitsReceive::Finalise(){


  delete sock;
  sock=0;

  if(m_data->hits!=0){

    m_data->hits->hits.clear();
    delete m_data->hits;
    m_data->hits=0;

  }

  return true;
}

#include "HitsSend.h"

HitsSend::HitsSend():Tool(){}


bool HitsSend::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;

  if(!m_variables.Get("Port",m_port)) m_port=5555;
  if(!m_variables.Get("Endpoint",endpoint)) return false;

  m_util=new Utilities(m_data->context);  

  sock = new  zmq::socket_t(*(m_data->context), ZMQ_PUSH);

  std::stringstream tmp;
  tmp<<"tcp://*:"<<m_port;

  sock->bind(tmp.str().c_str());

  items[0].socket = *sock;
  items[0].fd = 0;
  items[0].events = ZMQ_POLLOUT;
  items[0].revents = 0;

  if (!m_util->AddService(endpoint,m_port,false)) return false;
  
  return true;
}


bool HitsSend::Execute(){

  if(m_data->hits!=0){
    if(m_data->hits->hits.size()>0){
      zmq::poll(&items[0], 1, 100);
      
      if ((items [0].revents & ZMQ_POLLOUT)) {

	m_data->hits->Send(sock);
	m_data->hits=0;

      }
    }
  }

  return true;
}


bool HitsSend::Finalise(){

  bool ret=m_util->RemoveService(endpoint);

  delete sock;
  sock=0;

  if(!ret) return false;

  return true;
}

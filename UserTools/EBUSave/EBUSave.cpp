#include "EBUSave.h"

EBUSave_args::EBUSave_args(zmq::context_t* contextin, Utilities* utilsin, std::string connection, std::string outpathin, std::string outfilein):Thread_args(){

  context=contextin;
  utils=utilsin;
  outpath=outpathin;
  outfile=outfilein;

  sockin=new zmq::socket_t(*context, ZMQ_PAIR);
  sockin->bind(connection.c_str());

  in[0].socket = *sockin;
  in[0].fd = 0;
  in[0].events = ZMQ_POLLIN;
  in[0].revents = 0;

  filecount =0;
}

EBUSave_args::~EBUSave_args(){
  
  delete sockin;
  sockin=0;

}


EBUSave::EBUSave():Tool(){}


bool EBUSave::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;

  if(!m_variables.Get("output_path",outpath)) outpath="./";
  if(!m_variables.Get("output_file",outfile)) outfile="Data";
  if(!m_variables.Get("file_size",filesize)) filesize=10000;

  m_util=new Utilities(m_data->context);
  args=new EBUSave_args(m_data->context, m_util, "inproc://EBUSave",outpath, outfile);
  
  m_util->CreateThread("test", &Thread, args);

  sock = new zmq::socket_t(*(m_data->context), ZMQ_PAIR);
  sock->connect("inproc://EBUSave");

  items[0].socket = *sock;
  items[0].fd = 0;
  items[0].events = ZMQ_POLLOUT;
  items[0].revents = 0;


  return true;
}


bool EBUSave::Execute(){

  if(m_data->hits->hits.size()>=filesize){

    zmq::poll(&items[0], 1, 0);
    
    if ((items [0].revents & ZMQ_POLLOUT)) {
      if(m_util->SendPointer(sock,m_data->hits)) m_data->hits=new Hits();
    }
    
  }

  return true;
}


bool EBUSave::Finalise(){

  m_util->KillThread(args);

  delete args;
  args=0;

  delete m_util;
  m_util=0;

  return true;
}

void EBUSave::Thread(Thread_args* arg){

  EBUSave_args* args=reinterpret_cast<EBUSave_args*>(arg);

  zmq::poll(&(args->in[0]), 1, 100);

  if ((args->in[0].revents & ZMQ_POLLIN)) {

    Hits* tmp;
    args->utils->ReceivePointer(args->sockin,tmp);

    BoostStore file(0,false);
    std::stringstream out;
    out<<args->outpath << args->outfile << "p" << args->filecount;
    file.Set("Hits",tmp);
    file.Save(out.str());
    args->filecount++;

  }
  
  
}

#ifndef EBUSave_H
#define EBUSave_H

#include <string>
#include <iostream>

#include "Tool.h"
#include <zmq.hpp>

struct EBUSave_args:Thread_args{

  EBUSave_args(zmq::context_t* contextin, Utilities* untilsin, std::string connection, std::string outpathin, std::string outfilein);
  ~EBUSave_args();

  Utilities* utils;
  zmq::socket_t* sockin;
  zmq::pollitem_t in[1];
  std::string outpath;
  std::string outfile;
  unsigned long filecount;

};

/**
 * \class EBUSave
 *
 * This is a balnk template for a Tool used by the script to generate a new custom tool. Please fill out the descripton and author information.
*
* $Author: B.Richards $
* $Date: 2019/05/28 10:44:00 $
* Contact: b.richards@qmul.ac.uk
*/
class EBUSave: public Tool {


 public:

  EBUSave(); ///< Simple constructor
  bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
  bool Execute(); ///< Executre function used to perform Tool perpose. 
  bool Finalise(); ///< Finalise funciton used to clean up resorces.


 private:

  static void Thread(Thread_args* arg);
  Utilities* m_util;
  Thread_args* args;
  zmq::socket_t* sock;
  zmq::pollitem_t items[1];
  std::string outpath;
  std::string outfile;
  int filesize;

};


#endif

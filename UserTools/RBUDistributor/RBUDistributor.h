#ifndef RBUDistributor_H
#define RBUDistributor_H

#include <string>
#include <iostream>

#include "Tool.h"
#include <zmq.hpp>

struct RBUDistributor_args:Thread_args{

  RBUDistributor_args(zmq::context_t* contextin, Utilities* untilsin, std::string connection, int port);
  ~RBUDistributor_args();

  Utilities* utils;
  zmq::socket_t* sockin;
  zmq::socket_t* sockout;
  zmq::pollitem_t in[1];
  zmq::pollitem_t out[1];

};

/**
 * \class RBUDistributor
 *
 * This is a balnk template for a Tool used by the script to generate a new custom tool. Please fill out the descripton and author information.
*
* $Author: B.Richards $
* $Date: 2019/05/28 10:44:00 $
* Contact: b.richards@qmul.ac.uk
*/
class RBUDistributor: public Tool {


 public:

  RBUDistributor(); ///< Simple constructor
  bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
  bool Execute(); ///< Executre function used to perform Tool perpose. 
  bool Finalise(); ///< Finalise funciton used to clean up resorces.


 private:

  static void Thread(Thread_args* arg);
  Utilities* m_util;
  Thread_args* args;
  zmq::socket_t* sock;
  int numhits;
  int port;
  zmq::pollitem_t items[1];

};


#endif

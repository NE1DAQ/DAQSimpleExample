#ifndef HitsReceive_H
#define HitsReceive_H

#include <string>
#include <iostream>

#include "Tool.h"
#include <map>
#include "boost/date_time/gregorian/gregorian.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>

/**
 * \class HitsReceive
 *
 * This is a balnk template for a Tool used by the script to generate a new custom tool. Please fill out the descripton and author information.
*
* $Author: B.Richards $
* $Date: 2019/05/28 10:44:00 $
* Contact: b.richards@qmul.ac.uk
*/
class HitsReceive: public Tool {


 public:

  HitsReceive(); ///< Simple constructor
  bool Initialise(std::string configfile,DataModel &data); ///< Initialise Function for setting up Tool resorces. @param configfile The path and name of the dynamic configuration file to read in. @param data A reference to the transient data class used to pass information between Tools.
  bool Execute(); ///< Executre function used to perform Tool perpose. 
  bool Finalise(); ///< Finalise funciton used to clean up resorces.


 private:

  Utilities* m_util;
  zmq::socket_t* sock;
  int m_port;
  std::map<std::string,Store*> connections;
  boost::posix_time::ptime last;
  boost::posix_time::time_duration period;
  zmq::pollitem_t items[1];
  std::string endpoint;

};


#endif

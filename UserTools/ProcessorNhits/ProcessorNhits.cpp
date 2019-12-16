#include "ProcessorNhits.h"

ProcessorNhits::ProcessorNhits():Tool(){}


bool ProcessorNhits::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;

  if(!m_variables.Get("threshold",threshold)) threshold=50.0;
  if(!m_variables.Get("numhits",numhits)) numhits=600;

  return true;
}


bool ProcessorNhits::Execute(){

 

  if(m_data->hits->hits.size()>0){
    int count=0;
    std::vector<std::vector<Hit>::iterator> remove;
    for (std::vector<Hit>::iterator it = m_data->hits->hits.begin() ; it != m_data->hits->hits.end(); ++it){
      if (it->charge>threshold) count++;
      else remove.push_back(it);
    }
    
    if(count<numhits) m_data->hits->hits.clear();
    else{
      for( int i=0; i<remove.size();i++){
	m_data->hits->hits.erase(remove.at(i));
      }
    }

  }
  
  return true;
}


bool ProcessorNhits::Finalise(){

  return true;
}

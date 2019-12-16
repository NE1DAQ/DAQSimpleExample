#include "FEESimData.h"

FEESimData::FEESimData():Tool(){}


bool FEESimData::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;

  if(!m_variables.Get("Hits_per_transfer",numhits)) numhits=100;

  srand (time(NULL));
 
  m_data->hits=0;
  
  return true;
}


bool FEESimData::Execute(){
  
  if( m_data->hits==0) m_data->hits =new Hits();
  
  if(m_data->hits->hits.size()<10000){

    for (int i=0; i<numhits;i++){
      
      Hit tmp(rand()% 10000 +1 ,(rand() % 10000 + 1)/100.00, (rand() % 100 +1));
      
      m_data->hits->hits.push_back(tmp); // this is a crappy coppy but could easily be done without but no point for a demo 
      
    }
    
  }
  
  return true;
}


bool FEESimData::Finalise(){

  if( m_data->hits!=0){
  
    m_data->hits->hits.clear();
    delete  m_data->hits;
    m_data->hits=0;
  
  }
  
  return true;
}

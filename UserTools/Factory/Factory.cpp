#include "Factory.h"

Tool* Factory(std::string tool){
Tool* ret=0;

// if (tool=="Type") tool=new Type;
if (tool=="DummyTool") ret=new DummyTool;

if (tool=="FEESimData") ret=new FEESimData;
if (tool=="HitsSend") ret=new HitsSend;
if (tool=="HitsReceive") ret=new HitsReceive;
if (tool=="RBUDistributor") ret=new RBUDistributor;
if (tool=="ProcessorNhits") ret=new ProcessorNhits;
if (tool=="EBUSave") ret=new EBUSave;
return ret;
}


//:: # Generate the handler routines for the interfaces

//:: pd_prefix = "p4_pd_" + p4_prefix + "_"
//:: pd_static_prefix = "p4_pd_"
//:: api_prefix = p4_prefix + "_"

#include <iostream>
#include "bfn_pd_rpc_server.h"
#include <thrift/processor/TMultiplexedProcessor.h>

using namespace ::apache::thrift;

using ::std::shared_ptr;

#include "p4_pd_rpc_server.ipp"

// processor needs to be of type TMultiplexedProcessor,
// I am keeping a void * pointer for 
// now, in case this function is called from C code
int add_to_rpc_server(void *processor) {
  std::cerr << "Adding Thrift service for P4 program ${p4_prefix} to server\n";

  shared_ptr<${p4_prefix}Handler> ${p4_prefix}_handler(new ${p4_prefix}Handler());

  TMultiplexedProcessor *processor_ = (TMultiplexedProcessor *) processor;
  processor_->registerProcessor(
    "${p4_prefix}",
    shared_ptr<TProcessor>(new ${p4_prefix}Processor(${p4_prefix}_handler))
  );

  return 0;
}
int rmv_from_rpc_server(void *processor) {
  std::cerr << "Removing Thrift service for P4 program ${p4_prefix} from server\n";

  TMultiplexedProcessor *processor_ = (TMultiplexedProcessor *) processor;
  processor_->registerProcessor(
    "${p4_prefix}",
    shared_ptr<TProcessor>()
  );

  return 0;
}

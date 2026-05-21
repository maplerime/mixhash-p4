/*******************************************************************************
 *  INTEL CONFIDENTIAL
 *
 *  Copyright (c) 2021 Intel Corporation
 *  All Rights Reserved.
 *
 *  This software and the related documents are Intel copyrighted materials,
 *  and your use of them is governed by the express license under which they
 *  were provided to you ("License"). Unless the License provides otherwise,
 *  you may not use, modify, copy, publish, distribute, disclose or transmit
 *  this software or the related documents without Intel's prior written
 *  permission.
 *
 *  This software and the related documents are provided as is, with no express
 *  or implied warranties, other than those that are expressly stated in the
 *  License.
 ******************************************************************************/

#include <iostream>
#include "diag_rpc_server.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/processor/TMultiplexedProcessor.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using ::std::shared_ptr;

#include <pthread.h>

// All the supported Thrift service handlers :
#include "diag_rpc_server.ipp"

static pthread_mutex_t cookie_mutex;
static pthread_cond_t cookie_cv;
static void *cookie;

/*
 * Thread wrapper for starting the server
 */

static void *diag_rpc_server_thread(void *) {
  int port = DIAG_RPC_SERVER_PORT;

  shared_ptr<diag_rpcHandler> diag_handler(new diag_rpcHandler());

  shared_ptr<TProcessor> processor(new diag_rpcProcessor(diag_handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(
      new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TThreadedServer server(
      processor, serverTransport, transportFactory, protocolFactory);

  pthread_mutex_lock(&cookie_mutex);
  cookie = (void *)processor.get();
  pthread_cond_signal(&cookie_cv);
  pthread_mutex_unlock(&cookie_mutex);

  pthread_setname_np(pthread_self(), "diag_thrift");
  server.serve();

  return NULL;
}

static pthread_t rpc_thread;

extern "C" {
int start_diag_rpc_server(void **server_cookie) {
  pthread_mutex_init(&cookie_mutex, NULL);
  pthread_cond_init(&cookie_cv, NULL);

  std::cerr << "Starting DIAG RPC server on port " << DIAG_RPC_SERVER_PORT
            << std::endl;

  *server_cookie = NULL;

  int status = pthread_create(&rpc_thread, NULL, diag_rpc_server_thread, NULL);

  if (status) return status;

  pthread_mutex_lock(&cookie_mutex);
  while (!cookie) {
    pthread_cond_wait(&cookie_cv, &cookie_mutex);
  }
  pthread_mutex_unlock(&cookie_mutex);

  *server_cookie = cookie;

  pthread_mutex_destroy(&cookie_mutex);
  pthread_cond_destroy(&cookie_cv);

  return 0;
}

int stop_diag_rpc_server() {
  int ret = 0;

  if (rpc_thread) {
    ret = pthread_cancel(rpc_thread);
    if (ret == 0) {
      pthread_join(rpc_thread, NULL);
    }
    rpc_thread = 0;
  }
  return 0;
}
}

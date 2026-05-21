namespace py res_pd_rpc
namespace cpp res_pd_rpc
namespace c_glib res_pd_rpc

typedef i32 SessionHandle_t
struct DevTarget_t {
  1: required i32 dev_id;
  2: required i16 dev_pipe_id;
}

struct DevPortTarget_t {
  1: required i16 dev_pipe_id;
  2: required i16 port_id;
}

struct DevParserTarget_t {
  1: required i32 dev_id;
  2: required i8  gress_id;
  3: required i16 dev_pipe_id;
  4: required i8  parser_id;
}

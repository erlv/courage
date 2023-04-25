import torch
import triton
import logging
logging.basicConfig(level=logging.INFO)

logging.info("torch path: %s"%(torch.__file__))
logging.info("triton path: %s"%(triton.__file__))


@torch.compile(options={"verbose_progress":True, "debug": True, "trace.enabled":True, 
                        "trace.debug_log":True, "trace.info_log":True, 
                        "trace.fx_graph":True,"triton.cudagraphs":True, 
                        "triton.debug_sync_graph":True, "triton.debug_sync_kernel":True}, 
                fullgraph=True)
def foo(x):
    return torch.sin(x) + torch.cos(x)

x=torch.rand(5,3)
print(foo(x))
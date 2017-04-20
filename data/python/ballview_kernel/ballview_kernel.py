from ipykernel.kernelbase import Kernel

import socket, json

__version__ = '0.1'

class BALLViewKernel(Kernel):
	"""BALLView kernel for Jupyter"""
	
	implementation = 'ballview_kernel'
	implementation_version = __version__
	
	banner = 'BALLView'
	
	language_info = {
		'mimetype':			'text/x-python',
		'name':				'python',
		'file_extension':	'.py',
	}
	
	def do_execute(self, code, silent, store_history=True, user_expressions=None, allow_stdin=False):
		
		code = json.dumps({
			'msg_type': 'execute_request',
			'content':   code
		})
		
		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		s.connect(('127.0.0.1', 8897))
		s.sendall(code)
		data = s.recv(1024)
		s.close()
		
		data = json.loads(data)
		
		content = {
			'name': 'stdout' if data['msg_type'] == 'execute_result' else 'stderr',
			'text': data['content']
		}
		self.send_response(self.iopub_socket, 'stream', content)
		
		return {
			'status': 			'ok',
			'execution_count':	self.execution_count,
			'payload':			[],
			'user_expressions':	{},
		}

if __name__ == '__main__':
	from ipykernel.kernelapp import IPKernelApp
	IPKernelApp.launch_instance(kernel_class=BALLViewKernel)

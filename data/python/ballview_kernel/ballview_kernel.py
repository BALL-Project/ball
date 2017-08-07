from ipykernel.kernelbase import Kernel
from ipython_genutils.py3compat import safe_unicode

import json, socket, sys, traceback

__version__ = '0.1'

class BALLViewKernel(Kernel):
	"""BALLView kernel for Jupyter"""

	implementation = 'ballview_kernel'
	implementation_version = __version__

	banner = 'BALLView'

	language_info = {
		'mimetype':       'text/x-python',
		'name':           'python',
		'file_extension': '.py',
	}

	pyserver_host = '127.0.0.1'
	pyserver_port = 8897

	def do_execute(self, code, silent, store_history=True, user_expressions=None, allow_stdin=False):

		code = json.dumps({
			'msg_type': 'execute_request',
			'content':   code
		})

		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		try:
			s.connect((self.pyserver_host, self.pyserver_port))
		except socket.error:
			exc_type, exc_value, exc_traceback = sys.exc_info()

			content = {
				'name': 'stderr',
				'text': 'ERROR: Cannot connect to BALL PyServer. Please make sure PyServer is running!'
			}
			self.send_response(self.iopub_socket, 'stream', content)

			content.update({
				'status':          'error',
				'ename':           type(exc_type).__name__,
				'evalue':          safe_unicode(traceback.format_exception_only(exc_type, exc_value)),
				'traceback':       traceback.format_tb(exc_traceback),
				'execution_count': self.execution_count - 1,
				'payload':         [],
				'user_expression': {}
			})
			return content

		s.sendall(code)
		data = self.recvall(s)
		s.close()

		data = json.loads(data)

		content = {
			'name': 'stdout' if data['msg_type'] == 'execute_result' else 'stderr',
			'text': data['content']
		}
		self.send_response(self.iopub_socket, 'stream', content)

		return {
			'status':           'ok',
			'execution_count':  self.execution_count,
			'payload':          [],
			'user_expressions': {},
		}

	def recvall(self, sock):
		"""Reads all data from the given socket"""
		buf = 4096
		dat = []
		while True:
			dat.append(sock.recv(buf))
			if len(dat[-1]) < buf:
				break
		return ''.join(dat)

if __name__ == '__main__':
	from ipykernel.kernelapp import IPKernelApp
	IPKernelApp.launch_instance(kernel_class=BALLViewKernel)

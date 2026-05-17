"""
PyBALL Jupyter kernel.

NOTE (v1.7): This Jupyter kernel relies on TWO subsystems that are
DISABLED / non-existent in the v1.7 build:

1. The SIP Python bindings for BALL/VIEW
   (``BALL_PYTHON_SUPPORT=OFF`` in every ``ci-*`` CMake preset).

2. A Python server socket inside BALLView listening on
   ``127.0.0.1:8897``. That socket was provided by the old PyBALL
   embedded interpreter, which was disabled together with the SIP
   bindings. v1.7 BALLView does not start a server on this port.

The kernel is preserved as the architecture reference for the
successor work:

  - Phase 999.15 (targeted for v2.1) — rewrites the BALL/VIEW Python
    binding surface with a modern generator.
  - Phase 999.10 (targeted for v2.2) — replaces the in-process Python
    socket with a proper REST API hosted by BALLView, accompanied by
    a PyBALL SDK that wraps the REST calls. Once 999.10 lands, this
    kernel becomes obsolete and should be deleted or rewritten as a
    thin REST-client wrapper.
"""

from ipykernel.kernelbase import Kernel
from ipython_genutils.py3compat import safe_unicode

import json, socket, sys, traceback

__version__ = '0.1'

class BALLViewKernel(Kernel):
	"""PyBALL kernel for Jupyter"""

	implementation = 'pyball_kernel'
	implementation_version = __version__

	banner = 'PyBALL'

	language_info = {
		'mimetype':       'text/x-python',
		'name':           'python',
		'file_extension': '.py',
	}

	pyserver_host = '127.0.0.1'
	pyserver_port = 8897

	def do_execute(self, code, silent, store_history=True, user_expressions=None, allow_stdin=False):

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

		code = json.dumps({
			'msg_type': 'execute_request',
			'content':   code
		})

		s.sendall(code.encode('utf-8'))
		data = self.recvall(s)
		s.close()

		content = {}
		if data is None:
			content.update({
				'name': 'stderr',
				'text': 'WARNING: Response from BALL PyServer is incomplete!\n'
				        'Your code was evaluated by the BALL PyServer but the output cannot be shown!'
			})

		else:
			content.update({
				'name': 'stdout' if data['msg_type'] == 'execute_result' else 'stderr',
				'text': data['content']
			})

		self.send_response(self.iopub_socket, 'stream', content)
		content.update({
			'status':           'ok',
			'execution_count':  self.execution_count,
			'payload':          [],
			'user_expressions': {},
		})
		return content

	def recvall(self, sock):
		"""Reads all JSON data from the given socket and returns the dict representation of the data. Returns None
		if the data is not in JSON format."""
		nbytes = 4096
		dat = []
		while True:
			dat.append(sock.recv(nbytes).decode('utf-8'))
			if(len(dat[-1])) < nbytes:
				break
		try:
			return json.loads(''.join(dat))
		except ValueError:
			return None

if __name__ == '__main__':
	from ipykernel.kernelapp import IPKernelApp
	IPKernelApp.launch_instance(kernel_class=BALLViewKernel)

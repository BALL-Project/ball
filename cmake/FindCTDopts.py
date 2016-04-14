from CTDopts.CTDopts import CTDModel

model = CTDModel(
    name='exampleTool',  # required
    version='1.0',  # required
    description='This is an example tool presenting CTDopts usage',
    manual='manual string',
    docurl='http://dummy.url/docurl.html',
    category='testing',
    executableName='exampletool',
    executablePath='/path/to/exec/exampletool-1.0/exampletool'
    )

# if here, it's all OK!
# do NOT change this output string! FindCTDopts.cmake relies on stdout being exactly this string!
print("CTDopts seems to be working.")

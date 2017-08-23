# BALL Workflow Support
## Generating Task Descriptors

All BALL TOOLS are able to generate a CTD file describing their inputs, outputs, parameters. These descriptors can then be transformed to other formats to be used in different workflow engines.

Each supported format has a separate folder with specific information. Currently, [Galaxy] and [CWL] are supported. 

The following common configure options are provided as `CMake` flags:
- `CTDSCHEMA_VERSION` (default: `v1.7`): The tag/branch from the [CTDSchema GitHub repository](https://github.com/WorkflowConversion/CTDSchema) to use.  
- `CTDCONVERTER_VERSION` (default: `v2.0`):  The tag/branch from the [CTDConverter GitHub repository](https://github.com/WorkflowConversion/CTDSConverter) to use.
  

[CWL]: https://github.com/common-workflow-language
[Galaxy]: https://usegalaxy.org/

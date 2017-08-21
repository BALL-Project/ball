# CWL project
## Generating Task Descriptors for CWL

This folder contains a BALL dependent subproject to generate [CWL] task definitions for the BALL TOOLS. The project is configured and included by the top level BALL project. It is available as build target *cwl* and excluded from default build targets.

The following configure options are provided as CMake flags:

- `BALL_BUILD_CWL` (default: `ON`): 

  Builds required [CWL] task descriptors to include BALL TOOLS in [CWL] workflows.
  

[CWL]: https://github.com/common-workflow-language

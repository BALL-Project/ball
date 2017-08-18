# BALLAXY project
### Using BALL in Galaxy

This folder contains a BALL dependent subproject to enable [Galaxy] workflow support for BALL TOOLS.
The project is configured and included by the top level BALL project. It is available as build target *ballaxy* 
and excluded from default build targets.

The following configure options are provided as CMake flags:

- BALL_BUILD_BALLAXY (default: ON): 

  Builds required Galaxy [ToolConfig]s to include BALL TOOLS in a Galaxy instance.
  Additionally, build the BALLAXY plugin for BALLView, if required dependencies are met 
  (Qt5 WebEngine, BALL VIEW target available) 

- BALLAXY_INSTANCE (default: OFF):

  Additionally, create a fully configured Galaxy instance that includes BALLAXY.
  If ballaxy target has successfully been built, the Galaxy instance can be started
  using the bash script run_ballaxy.sh from the projects binary dir 
  (usually 'ball_build_dir/workflow_support/galaxy/').

- GALAXY_RELEASE (default: 'release_16.10'):

  If a full Galaxy instance is created (BALLAXY_INSTANCE=ON), this parameter can be used 
  to specify the Galaxy release to be used. In fact, this string has to be a valid 
  branch name of the [Galaxy GitHub repository].

For a detailed description how a working Ballaxy instance can be setup using the BALL Galaxy ToolConfigs,
please have a look at the corresponding [Galaxy documentation] and a configured BALLAXY setup script 
for a full ballaxy instance (ball_build_dir/workflow_support/galaxy/ballaxy_generator.cmake).


[Galaxy]: https://galaxyproject.org
[Galaxy documentation]: https://new.galaxyproject.org/Admin/Tools/AddToolTutorial
[Galaxy GitHub repository]: https://github.com/galaxyproject/galaxy
[ToolConfig]: https://wiki.galaxyproject.org/Admin/Tools/ToolConfigSyntax

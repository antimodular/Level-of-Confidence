# Attempt to load a config.make file.
# If none is found, project defaults in config.project.make will be used.
ifneq ($(wildcard config.make),)
	include config.make
endif

# make sure the the OF_ROOT location is defined
ifndef OF_ROOT
	OF_ROOT=$(realpath ../../..)
endif

# Generate version before building
.PHONY: version
version:
	@bash generate_version.sh

# Load version environment if it exists
-include version.env

# Export VERSION to build process
ifdef VERSION
export VERSION
endif

# Make builds depend on version generation
Release: version
Debug: version

# call the project makefile!
include $(OF_ROOT)/libs/openFrameworksCompiled/project/makefileCommon/compile.project.mk

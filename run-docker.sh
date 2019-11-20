#!/bin/bash

VERSION="1.0"

set -e # exit when any command fails

(
	cd "$(dirname "$0")"

	if test -z "$(docker images -q afip:$VERSION)" ; then
		echo "Creating image..."
		docker build -t afip:$VERSION .
	fi

	echo "Running container..."
	docker run --cap-add=SYS_PTRACE --security-opt seccomp=unconfined -it -p 6969:6969 -v $PWD:/afip --rm afip:1.0 /bin/sh -c "cd /afip/docker_build; /bin/sh"
)